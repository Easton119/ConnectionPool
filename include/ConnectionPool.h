#include<string>
#include<queue>
#include<mutex>
#include<atomic>
#include<thread>
#include<memory>
#include<functional>
#include<condition_variable>
using namespace std;
#include "Connection.h"
#include "public.h"
/*
实现连接池功能模块
1、连接池只需要一个实例->单例模式
2、服务端是多线程的->需要线程安全
*/

class ConnectionPool{
public:
    //获取连接池对象实例
    static ConnectionPool* getConnectionPool();
    
    //给外部提供接口，从连接池中获取一个可用的连接
    //然后再调用backConnection归还到连接池？->实际上选择返回智能指针
    //智能指针出作用域自动析构
    //重定义析构，把连接归还
    shared_ptr<Connection> getConnection();


private:
    ConnectionPool(); //构造函数私有化

    bool loadConfigFile();//从配置文件加载配置项

    //运行在独立的线程中，专门负责生产新连接
    //this指针绑定
    void produceConnectionTask();

    //扫描超过maxIdleTime时间的空闲连接，进行连接回收
    void scannerConnectionTask();

    string _ip; //mysql的ip
    unsigned short _port;//端口号 3306
    string _username;//mysql登录用户名
    string _password;//登录密码
    string _dbname;
    int _initSize;//初始连接量
    int _maxSize;//最大连接量
    int _maxIdleTime;//最大空闲时间
    int _connectionTimeout;//获取连接的超时时间


    queue<Connection*> _connectionQue; //存储mysql连接的队列
    mutex _queueMutex; //维护连接队列的线程安全互斥锁
    atomic_int _connectionCnt; //记录连接所创建的connection连接的总数量
    condition_variable cv;//设置条件变量，用于连接生产线程和连接消费线程的通信
};