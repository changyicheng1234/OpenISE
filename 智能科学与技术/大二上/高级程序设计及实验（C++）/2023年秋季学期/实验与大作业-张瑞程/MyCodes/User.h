#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class User {
protected://被保护，允许子类使用
    string name;
    string ID;
    string pword; 
public:
    virtual void info() = 0; // 纯虚函数，子类必须实现
    
};







