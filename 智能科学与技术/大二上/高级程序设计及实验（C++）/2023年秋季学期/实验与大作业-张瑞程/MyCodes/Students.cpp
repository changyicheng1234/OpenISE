#include "Students.h"
#include"User.h"
#include "Management.h"
#include <random>
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
//导入外部文件函数
extern void title();
extern void menu();
//导入外部文件变量
extern vector<Student> stu_v;


//函数声明
int slogin();
bool findID(vector<Student>,string);

//构造函数
	Student::Student() { 

		//slogin(); 
	};

//登陆函数
int slogin()
{
	static int err;
	system("cls");  //清屏
	title();//打印页眉
	string NetID;
	string pword;
	//输入NetID
	cout << "\t登录模式 : 学生" << endl;
	cout << endl << "\t请输入你的NetID :  ";
	cin >> NetID;
	getchar();//这个用来接受最后一个回车
	
	//输入密码:我在这里进行了优化处理，每输入一个字符，立刻在控制台中打印一个*
	cout << "\n\t请输入你的密码 :  ";
	char ch=0;
	while ((ch = _getch()) != '\r') { // 回车键是 '\r'
		if (ch != '\b') { // 如果不是退格键
			pword.push_back(ch);
			cout << '*';
		}
		else { // 处理退格键
			if (!pword.empty()) {
				pword.pop_back();
				cout << "\b \b"; // 删除控制台上的一个字符
			}
		}
	}

	//初始密码为NetID,检查是否相等
	if (NetID != pword)
	{
		err++;
		if (3-err > 0) {//设定只有三次输入机会，否则要退出重新登录
			cout <<endl<<"\n" << "\tNetID或密码无效！\n" << "\t您还有" << 3 - err << "次输入机会" << "请在5秒后重新输入。" << endl;
			Sleep(5000);//5秒后重新输入
			slogin();
			return err;
		}
		else {
			cout << "您的机会已用光。程序退出中···" << endl;
			Sleep(3000);//3s后退出
			return err;
		}
	}

	//判断该NetID是否存在于数据库中,NetID只能管理员创建，学生不能自己创建
	int flag = 0;
	if (!findID(stu_v, NetID))
	{
		cout << "\n\n\t该NetID不存在！请重新输入！2秒后自动跳转。";
		Sleep(2000);//3秒后重新输入
		slogin();
		return err;
	}
	else {
		flag = 1;
	}

	//如果登录成功，则可以进入选项菜单
	if (flag)
	{
		cout << "\n\n\t登录成功！页面加载中······" << endl;
		Sleep(1000);
		//找到数据库中的对于元素
		auto iter = stu_v.begin();
		while (iter != stu_v.end())
		{
			if (iter->NetID == NetID)
			{
				break;
			}
			else 
			{
				++iter;//迭代器递增
			}
		}

	sig1:
		Sleep(2000);//2秒后界面跳转
		system("cls");  //清屏
		title();//打印页眉
		cout << "\t登录模式 : 学生" << endl;
		cout << "\t\t菜单" << endl;
		cout << "\t\t1 : 进入学习系统\n\t\t2 : 修改个人信息\n\t\t3 : 查看作业成绩\n\t\t4 : 查看作业公告  \n\t\t5 : 退出" << endl;
		cout << "\t请输入你的选择  : ";
		int choice = 0;
		cin >> choice;
		switch (choice)
		{
		case 1:
			//iter->info();
			iter->to_study();
			goto sig1;  //转会本函数的开始，重新进行菜单选择
			break;
		case 2:
			iter->setinfo();
			goto sig1;
			break;
		case 3:
			iter->showScores();
			goto sig1;
			break;
		case 4:
			iter->notification();
			goto sig1;
			break;
		case 5:
			cout << "\n\t正在退出···";
			Sleep(3000);
			system("cls");  //清屏
			menu();
			return err;
			break;
		default: cout << "\t无效选项！程序退出！ ";
			Sleep(2000);
		}
	}
	return err;
}

//查找数据库中是否存在该学生
bool findID(vector<Student>  students, string NetID) {
	for (const auto& student : students) {
		if (student.NetID == NetID) {
			return true;
		}
	}
	return false;
}

//开始学习
void Student::to_study()
{
	//创建一个Management类的指针
	Management* new_M = new Management;
	while (new_M->run()) {

	}
	new_M->close();
}


//打印学生信息
void Student::info() {
	system("cls");  //清屏
	title();//打印页眉
	cout << "\t登录模式 : 学生" << endl;
	cout << "\t\t个人信息" << endl;
	cout << "\t\t姓名:"<<name;
	cout << "\n\t\t性别" << sex;
	cout << "\n\t\tNetID:" << NetID;
	cout << "\n\t\t年级:" << grade;
	cout << "\n\t\t学院:" << pm;
	cout << "\n\t\t邮箱：" << email;
	cout << "\n\t\t电话：" << phone;
	cout << "\n\n\t\t请按任意键退出。"<<endl;
	_getch();//当用户按下一个键后程序继续进行
	Sleep(3000);
}

//更改信息
void Student::setinfo() {
	system("cls");  //清屏
	title();//打印页眉
	cout << "\t登录模式 : 学生" << endl;
	cout << "\t\t当前个人信息" << endl;
	cout << "\t\t姓名: " << name;
	cout << "\n\t\t性别: " << sex;
	cout << "\n\t\tNetID: " << NetID;         //显示旧数据
	cout << "\n\t\t年级: " << grade;
	cout << "\n\t\t学院: " << pm;
	cout << "\n\t\t邮箱：" << email;
	cout << "\n\t\t电话：" << phone;
	cout << "\n-----------------------------------------------------------------------------------------------------------------------\n"<<endl;
	cout << "请输入新的个人信息：";
	cout << "\n\t\t姓名: " ;  cin >> name;
	cout << "\n\t\t性别: " ;  cin >> sex;
	cout << "\n\t\tNetID: " ;  cin >> NetID;
	cout << "\n\t\t年级: ";  cin >> grade;     //更改数据
	cout << "\n\t\t学院: ";  cin >> pm;
	cout << "\n\t\t邮箱：";  cin >> email;
	cout << "\n\t\t电话：";  cin >> phone;
	cout << "\n\t\t修改完成！正在更新数据库···" << endl;
	//STL容器可以自动维护数据库中信息,
	Sleep(4000);//
}

//查看成绩、学分、绩点
void Student::showScores()
{
	system("cls");  //清屏
	title();//打印页眉
	cout << "   登录模式 : 学生" << endl;
	cout << "\n\t\t\t\t     已批改作业成绩"<<endl;
	cout << "\t\t----------------------------------------------------------" << endl;
	cout << "\t\t   科目  \t|\t作业成绩 \t|\t 作业等级"<<endl;
	cout << "\t\tC++程序设计：\t|\t   " << cpp[0]<<"\t\t|\t   A+";
	cout << "\n\t\t离散数学：\t|\t   " << dis_math[0]<<"\t\t|\t   A";
	cout << "\n\t\t数据结构：\t|\t   " << data_struct[0]<<"\t\t|\t   A+";
	cout << "\n\t\t机器学习：\t|\t   " << m_learning[0]<<"\t\t|\t   A-";
	cout << "\n\t\tPython：\t|\t   " <<  python[0]<<"\t\t|\t   B+";
	cout << "\n\t\t信号与系统：\t|\t   " << sig_sys[0] << "\t\t|\t   A+" << endl;
	cout << "\t\t----------------------------------------------------------" << endl;
	cout << "\n" << endl;
	cout << "\n\n\t请按任意键退出。";
	_getch();
	Sleep(500);
}

//查看学校公告：使用文件读取操作更加高效
int Student::notification() {
	system("cls");
	//读取文件
	ifstream readfile("notification.txt", ios::in | ios::binary);
	if (!readfile.is_open()) {
		cout << "\t没有公告。  3秒后自动返回。";
		Sleep(2000);
		return -1;
	}

	title();//打印页眉
	cout << "\n\t作业公告: \n\n";

	string line;//逐行读取
	while (getline(readfile, line)) {
		cout << "\t\t" << line << "\n ";
	}

	cout << "\n\t开始完成你的作业吧！   请按任意键退出。";
	_getch();
	readfile.close();  //关闭文件
	return 0; 
}



