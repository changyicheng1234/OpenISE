#pragma once
//#include<User.h>
//#include "Students.h"
#include<string.h>//用于字符串操作
#include <conio.h>//控制输出台
#include <windows.h>//用于控制操作台悬停时间
#include <vector>//使用vector容器操作
#include<algorithm>//使用STL迭代器
#include<fstream>// 文件操作 
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Assit
{
public:
    //个人信息数据成员
    string NetID = "20354111";
    string pword;
    string name = "沈助教";
    string sex = "男";
    int grade = 2020;//年级
    string pm = "智能工程学院"; //学院
    string email = "shenzhj@mail2.sysu.edu.cn";
    string phone = "13939368767";
    string subject = "C++程序设计";//课程

    //助教修改个人信息
    void setinfo();

    //助教批改作业，录入作业成绩
    void setScores();

    //查看学生反馈的需要讲的题：使用文件读取操作，允许学生写入、助教和老师读取
    int view_fb();  //fb:feedback
  
    //预约教室进行习题课讲解
    void bookingroom();
    void shenzhen();//深圳校区场地预约系统
    void gzn();//广州南校区场地预约系统
    void gzb();//广州北校区场地预约系统
    void gzd();//广州东校区场地预约系统
    void zhuhai();//珠海校区场地预约系统

    //助教在预定好习题课课室后，向学生发布习题课公告
    int xitike();
};

