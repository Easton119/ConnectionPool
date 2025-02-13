#include<iostream>
// #include "Connection.h"
#include "ConnectionPool.h"
using namespace std;

int main(){
    
    //单线程：
    clock_t begin = clock();
    // ConnectionPool *cp = ConnectionPool::getConnectionPool();
    // for(int i=0;i<10000;i++){
        /*Connection conn;
        char sql[1024]={0};
        string s = "testt"+to_string(i);
        // cout<<s.c_str()<<endl;
        sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
        conn.connect("127.0.0.1",3306,"root","123456","chat");
        conn.update(sql);*/
        
    //     shared_ptr<Connection>sp= cp->getConnection();
    //     char sql[1024]={0};
    //     string s = "testt"+to_string(i);
    //     // cout<<s.c_str()<<endl;
    //     sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
    //     sp->update(sql);

    // }

    //多线程
    thread t1([](){
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for(int i=0;i<5000;i++){
             shared_ptr<Connection>sp= cp->getConnection();
             char sql[1024]={0};
             string s = "testt"+to_string(i);
             // cout<<s.c_str()<<endl;
             sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
             sp->update(sql);
        }
    });
    thread t2([](){
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for(int i=0;i<5000;i++){
             shared_ptr<Connection>sp= cp->getConnection();
             char sql[1024]={0};
             string s = "testt"+to_string(i);
             // cout<<s.c_str()<<endl;
             sprintf(sql,"insert into user(name,password) values('%s','%s')",s.c_str(),"123");
             sp->update(sql);
        }
    });
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
    t1.join();
    t2.join();
    // t3.join();
    // t4.join();
    clock_t end =clock();
    double endtime=(double)(end-begin)/CLOCKS_PER_SEC;
    cout<<"Over time:"<< endtime * 1000<< "ms" << endl;
    return 0;
}