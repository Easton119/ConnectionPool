#include<iostream>
// #include "Connection.h"
#include "ConnectionPool.h"
using namespace std;

#define FlAG 0// 0 表示禁用，1 表示启用

#ifdef FLAG
int main(){
    clock_t begin = clock();
    // 单线程：
    cout<<"单线程"<<endl;
    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    for(int i=0;i<10000;i++){
        /*Connection conn;
        char sql[1024]={0};
        string s = "testt"+to_string(i);
        // cout<<s.c_str()<<endl;
        sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
        conn.connect("127.0.0.1",3306,"root","123456","chat");
        conn.update(sql);*/
        
        shared_ptr<Connection>sp= cp->getConnection();
        char sql[1024]={0};
        string s = "testt"+to_string(i);
         // cout<<s.c_str()<<endl;
        sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
        sp->update(sql);

    }

    // 多线程
    // cout<<"多线程"<<endl;
    // thread t1([](){
    //     ConnectionPool *cp = ConnectionPool::getConnectionPool();
    //     for(int i=0;i<2500;i++){
    //          shared_ptr<Connection>sp= cp->getConnection();
    //          char sql[1024]={0};
    //          string s = "testt"+to_string(i);
    //          // cout<<s.c_str()<<endl;
    //          sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
    //          sp->update(sql);
    //     }
    // });
    // thread t2([](){
    //     ConnectionPool *cp = ConnectionPool::getConnectionPool();
    //     for(int i=0;i<2500;i++){
    //          shared_ptr<Connection>sp= cp->getConnection();
    //          char sql[1024]={0};
    //          string s = "testt"+to_string(i);
    //          // cout<<s.c_str()<<endl;
    //          sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
    //          sp->update(sql);
    //     }
    // });
    // thread t3([](){
    //     ConnectionPool *cp = ConnectionPool::getConnectionPool();
    //     for(int i=0;i<2500;i++){
    //          shared_ptr<Connection>sp= cp->getConnection();
    //          char sql[1024]={0};
    //          string s = "testt"+to_string(i);
    //          // cout<<s.c_str()<<endl;
    //          sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
    //          sp->update(sql);
    //     }
    // });
    // thread t4([](){
    //     ConnectionPool *cp = ConnectionPool::getConnectionPool();
    //     for(int i=0;i<2500;i++){
    //          shared_ptr<Connection>sp= cp->getConnection();
    //          char sql[1024]={0};
    //          string s = "testt"+to_string(i);
    //          // cout<<s.c_str()<<endl;
    //          sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
    //          sp->update(sql);
    //     }
    // });
    // t1.join();
    // t2.join();
    // t3.join();
    // t4.join();
    clock_t end =clock();
    double endtime=(double)(end-begin)/CLOCKS_PER_SEC;
    cout<<"Over time:"<< endtime * 1000<< "ms" << endl;
    return 0;
}
#endif


#ifndef FLAG
int main(){
    clock_t begin = clock();
    //测试不使用连接池的性能
    // cout<<"不使用连接池,单线程"<<endl;
    
    // for(int i=0;i<1000;i++){
    //     Connection conn;
    //     char sql[1024]={0};
    //     string s = "testt"+to_string(i);
    //     // cout<<s.c_str()<<endl;
    //     sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
    //     conn.connect("127.0.0.1",3306,"root","123456","chat");
    //     conn.update(sql);
    // }

    cout<<"不使用连接池,4线程"<<endl;
    thread t1([](){
        for(int i=0;i<250;i++){
            Connection conn;
            char sql[1024]={0};
            string s = "testt"+to_string(i);
            // cout<<s.c_str()<<endl;
            sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
            conn.connect("127.0.0.1",3306,"root","123456","chat");
            conn.update(sql);
        }
    });
    thread t2([](){
        for(int i=0;i<250;i++){
            Connection conn;
            char sql[1024]={0};
            string s = "testt"+to_string(i);
            // cout<<s.c_str()<<endl;
            sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
            conn.connect("127.0.0.1",3306,"root","123456","chat");
            conn.update(sql);
        }
    });
    thread t3([](){
        for(int i=0;i<250;i++){
            Connection conn;
            char sql[1024]={0};
            string s = "testt"+to_string(i);
            // cout<<s.c_str()<<endl;
            sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
            conn.connect("127.0.0.1",3306,"root","123456","chat");
            conn.update(sql);
        }
    });
    thread t4([](){
        for(int i=0;i<250;i++){
            Connection conn;
            char sql[1024]={0};
            string s = "testt"+to_string(i);
            // cout<<s.c_str()<<endl;
            sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
            conn.connect("127.0.0.1",3306,"root","123456","chat");
            conn.update(sql);
        }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    clock_t end = clock();
    double endtime = (double)(end-begin)/CLOCKS_PER_SEC;
    cout<<"Over time:"<< endtime * 1000<< "ms" << endl;
}
#endif