#include <iostream>
#include <string>
#include <vector>
#include<stdlib.h>
#include<conio.h>
#include<cstring>
#include<iostream>
#include<fstream>// 文件操作 
#include<windows.h>
#include<time.h>
#include<cstdio>

#include"user.h"
#include"Students.h"
#include"Admi.h"
#include"Teacher.h"
//#include"Assit.h"
#include "../../../../../C++_Code/大作业/大作业/Assit.h"

using namespace std;

//用户数据库
vector<Student> stu_v;
vector<Teacher>tea_v;
vector<Assit>assit_v;

//用户登录函数
extern int slogin();//学生登录
extern int alogin();//管理员登录
extern int tlogin();//教师登录
extern int aslogin();//助教登录

//打印页眉
void title() {
    cout << " ****   *   *  ****   *   *" << "   ==========================================================================" << endl;
    cout << "*       *   * *       *   *" << endl;
    cout << " ****    * *   ****   *   *" << "   ****************************作业信息管理系统 *****************************" << endl;
    cout << "     *    *        *  *   *" << endl;
    cout << " ****     *    ****   ***** " << "  ==========================================================================" << endl;
    cout << "\n" << endl;
}

// 格式化打印函数
void printM(const std::string& menuItem) {
    const int width = 106; // 宽度
    string border(width, '-'); // 由 '-' 符号组成的边框
    int padding = (width - menuItem.size()) / 2 - 1; // 计算两边的填充空间
    string paddingSpaces(padding, ' ');

    cout << "|" << paddingSpaces << menuItem << paddingSpaces;
    // 如果长度为奇数，右边填充空间比左边多一个空格
    if (menuItem.size() % 2 != 0) {
        cout << " ";
    }
    cout << "|" << std::endl;
}

//菜单界面
void menu() {
    title();
    const int menuWidth = 106;
    string border(menuWidth, '-'); // 使用 '-' 创建边框

    // 打印菜单的顶部边框
    cout << border << endl;

    // 打印菜单项
    printM("欢迎使用学籍管理系统");
    printM("1. 学生登录");
    printM("2. 教师登录");
    printM("3. 助教登录");
    printM("4. 管理员登录");
    printM("5. 退出系统");

    // 打印菜单的底部边框
    cout << border << endl;

    // 提示用户输入
    cout << "\t请输入选项: ";
    int choice;
    cin >> choice;

    // 根据用户输入进入不同的登录界面
    switch (choice) {
    case 1:
        cout << "学生登录选项被选中" << endl;
        slogin();//学生登录函数
        break;
    case 2:
        cout << "教师登录选项被选中" << endl;
        tlogin();//教师登录函数
        break;
    case 3:
        cout << "助教登录选项被选中" << endl;
        aslogin();//管理员登录函数
        break;
    case 4:
        cout << "管理员登录选项被选中" << endl;
        alogin();//管理员登录函数
        break;
    case 5:
        cout << "退出系统中······" << endl;
        Sleep(3000);//让子弹飞一会~
        break;
    default:
        cout << "无效选项!" << endl;
        break;
    }
}

int main() {
    //创建测试变量，也可以文件输入
    //学生
    Student s1;
    s1.name = "张瑞程";
    s1.NetID = "22354189";
    s1.cpp[0] = 99;
    stu_v.push_back(s1);
    Student s2;
    s2.name = "王五";
    s2.NetID = "22354666";
    stu_v.push_back(s2);
    Student s3;
    s3.name = "赵四";
    s3.NetID = "22354888";
    stu_v.push_back(s3);

    //教师
    Teacher t1;
    t1.NetID = "123";
    t1.subject = "C++";
    tea_v.push_back(t1);

    //助教
    Assit as1;
    as1.NetID = "666";
    assit_v.push_back(as1);
    // 显示主菜单
    menu();
    return 0;

}




