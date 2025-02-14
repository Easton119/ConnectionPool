# ConnectionPool
ConnectionPool.cpp和ConnectionPool.h：连接池代码实现  
Connection.cpp和Connection.h：数据库操作代码、增删改查代码实现  

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
  
## 环境配置与编译  

编译命令：`g++ src/*.cpp -o main -Iinclude -lmysqlclient`  
运行：`./main`