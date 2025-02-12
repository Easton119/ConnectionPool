#include<iostream>
#include "Connection.h"
#include "ConnectionPool.h"
using namespace std;

int main(){
    // Connection conn;
    // char sql[1024]={0};
    // sprintf(sql,"insert into user(name,password) values('%s','%s')","test2","123");
    // conn.connect("127.0.0.1",3306,"root","123456","chat");
    // conn.update(sql);
    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    
    return 0;
}