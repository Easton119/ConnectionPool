#include<iostream>
#include "ConnectionPool.h"


//线程安全的懒汉单例函数接口
ConnectionPool* ConnectionPool::getConnectionPool(){
    static ConnectionPool pool;//编译器 自动进行 lock 和 unlock，天然的线程安全
    return &pool;
}

bool ConnectionPool::loadConfigFile(){
    FILE *pf = fopen("mysql.cnf","r");
    if(pf==nullptr){
        LOG("mysql.cnf file is not exist!");
        return false;
    }

    while(!feof(pf))//文件没到末尾
    {
        char line[1024]={0};
        fgets(line,1024,pf);
        string str = line;
        int idx = str.find('=',0);
        if(idx==-1)//无效的配置项
        {
            continue;
        }

        int endidx = str.find('\n',idx);
        string key = str.substr(0,idx);
        string value = str.substr(idx+1,endidx-idx-1);

        cout<<key<<":"<<value<<endl;
        if(key == "ip"){
            _ip = value;
        }else if (key=="port"){
            _port = atoi(value.c_str());
        }else if (key=="username"){
            _username = value;
        }else if (key=="password"){
            _password=value;
        }else if(key=="dbname"){
            _dbname =value;
        }
        else if (key=="initSize"){
            _initSize=atoi(value.c_str());
        }else if(key=="maxSize"){
            _maxSize=atoi(value.c_str());
        }else if(key=="maxIdleTime"){
            _maxIdleTime = atoi(value.c_str());
        }else if(key=="connectionTimeOut"){
            _connectionTimeout = atoi(value.c_str());
        }
    }
    return true;
}

ConnectionPool::ConnectionPool(){
    if(!loadConfigFile()){
        return;
    }
    //创建初始数量的连接
    for(int i=0;i<_initSize;++i){
        Connection *p = new Connection();
        p->connect(_ip,_port,_username,_password,_dbname);
        p->refreshAliveTime();//刷新一下进入空闲状态的起始时间
        //添加到队列里
        _connectionQue.push(p);//此时是系统启动，不涉及线程安全
        _connectionCnt++;
    }

    //启动一个新的线程，作为连接的生产者 linux thread => pthread_create
    thread produce(std::bind(&ConnectionPool::produceConnectionTask,this));
    produce.detach();

    //启动一个新的定时线程，扫描超过maxIdleTime时间的空闲连接，进行连接回收
    thread scanner(std::bind(&ConnectionPool::scannerConnectionTask,this));
    scanner.detach();
}
 //运行在独立的线程中，专门负责生产新连接
void ConnectionPool::produceConnectionTask(){
    // cout<<"启动一个新的线程，作为连接的生产者，专门负责生产新连接..."<<endl;
    for(;;){
        unique_lock<mutex> lock(_queueMutex);
        // 加锁，消费者就拿不到锁了

        while(!_connectionQue.empty()){
            cv.wait(lock);//队列不空，此处生产线程进入等待状态
            //把锁进行一个释放，消费者线程拿到这个锁，从队列取东西
        }
        //未到上限，继续创建新的创建
        if(_connectionCnt<_maxSize){
            Connection *p = new Connection();
            p->connect(_ip,_port,_username,_password,_dbname);
            p->refreshAliveTime();
            //添加到队列里
            _connectionQue.push(p);
            _connectionCnt++;
        }
        
        //通知消费者线程，可以消费连接了
        cv.notify_all();//消费者从等待到阻塞
    }//锁释放，消费者拿到锁
}

//给外部提供接口，从连接池中获取一个可用的连接
shared_ptr<Connection> ConnectionPool::getConnection(){
    // cout<<"getConnection，从连接池中获取一个可用的连接..."<<endl;
    unique_lock<mutex> lock(_queueMutex);
    while(_connectionQue.empty()){
        
        if(cv_status::timeout==cv.wait_for(lock,chrono::milliseconds(_connectionTimeout))){
            //超时醒来，发现是空
            if(_connectionQue.empty()){
                LOG("获取空闲连接超时了..获取失败");
                return nullptr;
            }    
        }
        
    }

    /*
    shared_ptr析构时，会把connection资源直接delete掉，相当于
    调用connection的析构函数，connection就被close掉了。
    这里需要自定义shared_ptr的释放资源的方式，把connection直接归还到queue中
    */

    shared_ptr<Connection> sp(_connectionQue.front(),
        [&](Connection* pcon){
            //自定义删除器
            // 这里是在服务器应用线程中调用的，所以一定要考虑队列的线程安全操作
            unique_lock<mutex> lock(_queueMutex);
            pcon->refreshAliveTime();//刷新一下
            _connectionQue.push(pcon);
        }
    );
    _connectionQue.pop();
    // if(_connectionQue.empty()){
    //     cv.notify_all();
    //     谁消费了队列中的最后一个连接，谁负责通知一下生产者生产连接
    // }
    cv.notify_all();//消费完连接以后，通知生产者线程检查一下，如果队列为空了，赶紧生产连接
    return sp;
}

//扫描超过maxIdleTime时间的空闲连接，进行连接回收
void ConnectionPool::scannerConnectionTask(){
    // cout<<"启动一个新的线程，扫描超过maxIdleTime时间的空闲连接，进行连接回收..."<<endl;
    for(;;){
        //通过sleep模拟定时效果
        this_thread::sleep_for(chrono::seconds(_maxIdleTime));
        //扫描整个队列，释放多余的连接
        unique_lock<mutex> lock(_queueMutex);
        while(_connectionCnt>_initSize){
            Connection *p = _connectionQue.front();
            if(p->getAliveTime()>=_maxIdleTime*1000){
                _connectionQue.pop();
                _connectionCnt--;
                delete p;//调用~Connection()释放连接
            }else{
                break;//队头的连接没有超过maxIdleTime,其他连接肯定没有
            }
        }
    }
}