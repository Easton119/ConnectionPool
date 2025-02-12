#pragma once
//防止 头文件被多次包含

#define LOG(str) \
    cout<<__FILE__<<":"<<__LINE__<<" "<<\
    __TIMESTAMP__<<" : "<<str<<endl;
