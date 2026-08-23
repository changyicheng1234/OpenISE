#pragma once
#include"User.h"

class Student : public User {
public:
    //个人信息数据成员
    string NetID = "未定义";
    string pword;
    string name= "未定义";
    string sex = "未定义";
    int grade = 2022;//年级
    string pm = "智能工程学院"; //学院
    string email = "未定义";
    string phone = "未定义";

    //作业成绩数据成员:数组第一个位置为成绩(初始化为0)，第二个位置为学分
    int cpp[2] = { 100 ,3};//C++程序设计
    int data_struct[2] = { 97 ,4};//数据结构
    int dis_math[2] = { 94 ,2};//离散数学
    int m_learning[2] = { 96 ,3};//机器学习
    int python[2] = { 89 ,3};//Python
    int sig_sys[2] = { 97,2 };//信号与心态
    int credits = cpp[1] + data_struct[1] + dis_math[1] + m_learning[1] + python[1]+sig_sys[1]; //总学分

    //构造函数
    Student();

    //静态变量和静态函数用于统计系统中学生个数
    static int sCount;
    static int getStuCount() { return sCount; }

    // 学生查看个人信息
    void info() override;
    //学生修改个人信息
    void setinfo();

    //开始写作业！
    void to_study();

    //查看个人作业成绩
    void showScores();

    //查看作业公告
    int notification();
    
};

