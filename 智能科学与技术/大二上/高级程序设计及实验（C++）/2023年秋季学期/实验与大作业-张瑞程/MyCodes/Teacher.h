#pragma once
#include"User.h"

class Teacher : public User {
public:
    //教师的个人信息
    string NetID = "未定义";
    string pword;//登录密码
    string name = "未定义";
    string sex = "未定义";
    string pm = "智能工程学院"; //工作单位
    string subject = "未定义";//教授的课程
    string jobtitle = "未定义";//职位：教授、副教授、助理教授、讲师、研究员、访问学者
    string email = "未定义";
    string phone = "未定义";

    //教师登录函数
    //此函数需要在一个Teacher对象创建前调用
    //且需要访问Teacher的成员数据和成员函数，故设计为友元函数
    friend int tlogin();  
    //发布作业
    int add_w();
    //教师查看个人信息
    void info() override;
    //教师更改个人信息
    void setinfo();
    //教师查看学生作业成绩
    void viewScores();
    //教师评定学生作业成绩
    void setScores();
    //教室查看学生的作业反馈信息
    int view_fb();
    //学生完成作业的学习专注度评估
    void pinggu();
};

