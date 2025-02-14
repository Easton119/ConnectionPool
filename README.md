# ConnectionPool
ConnectionPool.cpp和ConnectionPool.h：连接池代码实现  
Connection.cpp和Connection.h：数据库操作代码、增删改查代码实现  
mysql.cnf：数据库连接配置  
main.cpp：分别进行使用连接池和未使用连接池，单线程与多线程的性能测试

关于项目与测试详情可查看博客：

## 项目结构
.  
├── include  
│   ├── Connection.h  
│   ├── ConnectionPool.h  
│   └── public.h  
├── main  
├── mysql.cnf  
├── README.md  
└── src  
    ├── Connection.cpp  
    ├── ConnectionPool.cpp  
    └── main.cpp  
  
## 项目环境与编译  
Ubuntu 22.04
编译命令：`g++ src/*.cpp -o main -Iinclude -lmysqlclient`  
运行：`./main`