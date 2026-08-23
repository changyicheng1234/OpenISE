#pragma once
#include"User.h"

class Admi : public User {
public:
    //Admi();
    //管理员密码
    static string pw;
    // 管理员查看个人信息
    void info() override {};
    // 管理员添加学生
    void addStu();
    //管理员删除学生
    void deteStu();
    //管理员查看学生列表
    void viewStuList();
    //管理学生作业成绩
    void mangeSocres();
    //管理员添加教师
    void addTeacher();
    //管理员删除教师
    void deteTeacher();
    //管理员查看全体教师信息
    void viewTlist();
    //管理员发布公告
    int add_Notification();
    //管理员查看全校课室预约情况
    void viewBooking();
};

