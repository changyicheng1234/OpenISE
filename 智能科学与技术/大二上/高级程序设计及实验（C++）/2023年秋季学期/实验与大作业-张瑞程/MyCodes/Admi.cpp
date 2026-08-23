#include "Admi.h"
#include "Students.h"
#include"User.h"
#include"Teacher.h"
#include<string.h>
#include <conio.h>
#include <windows.h>
#include <vector>
#include<algorithm>
#include<fstream>// 文件操作 
using namespace std;

extern void title();
extern void menu();
extern void printM();
extern vector<Student> stu_v;
extern vector<Teacher> tea_v;

//登陆函数
int alogin()
{
	static int err;
	system("cls");  //清屏
	title();//打印页眉
	string NetID;
	string pword;
	//输入NetID
	cout << "\t登录模式 : 管理员" << endl;
	cout << endl << "\t请输入你的管理员账号:  ";
	cin >> NetID;
	getchar();//这个用来接受最后一个回车

	//输入密码:我在这里进行了优化处理，每输入一个字符，立刻在控制台中打印一个*
	cout << "\n\t请输入你的密码 :  ";
	char ch = 0;
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
	//初始密码为123456,检查是否相等
	if (NetID != "Cpp\0" || pword!="123456\0")
	{
		err++;
		if (3 - err > 0) {//设定只有三次输入机会，否则要退出重新登录
			cout << "\n\n" << "\tNetID或密码无效！\n" << "\t您还有" << 3 - err << "次输入机会" << "请在5秒后重新输入。" << endl;
			Sleep(5000);//5秒后重新输入
			alogin();
			return err;
		}
		else {
			cout << "您的机会已用光。" << endl;
			return err;
		}
	}

	//如果登录成功，则可以进入选项菜单
	if (1)
	{
		cout << "\n\n\t登录成功！页面加载中······" << endl;
		Admi a1;//创建一个管理员对象，以调用Admi类中的成员函数
	sig2:
		Sleep(2000);//2秒后界面跳转
		system("cls");  //清屏
		title();//打印页眉
		cout << "\t登录模式 : 管理员" << endl;
		cout << "\t\t《菜单》" << endl;
		cout << "\t\t1 : 添加学生\n\t\t2 : 删除学生\n\t\t3 : 查看学生信息";
		cout << "\n\t\t4 : 管理学生成绩\n\t\t5 : 添加老师\n\t\t6 : 删除老师";
		cout<<" \n\t\t7 : 查看教师列表\n\t\t8 : 发布公告\n\t\t9 : 查看课室预约 \n\t\t10 : 退出" << endl; 
		cout << "\t请输入你的选择  : ";
		int choice = 0;
		cin >> choice;
		switch (choice)
		{
		case 1:
			a1.addStu();
			goto sig2;  //本次操作结束后转回本函数的开始，重新进行菜单选择
			break;
		case 2:
			a1.deteStu();
			goto sig2;
			break;
		case 3:
			a1.viewStuList();
			goto sig2;
			break;
		case 4:
			a1.mangeSocres();
			goto sig2;
			break;
		case 5:
			a1.addTeacher();
			goto sig2;
			break;
		case 6:
			a1.deteTeacher();
			goto sig2;
			break;
		case 7:
			a1.viewTlist();
			goto sig2;
			break;
		case 8:
			a1.add_Notification();
			goto sig2;
			break;
		case 9:
			a1.viewBooking();
			goto sig2;
			break;
		case 10:
			cout << "\n\t正在退出···";
			Sleep(3000);
			system("cls");  //清屏
			menu();
			return err;
			break;
		default: cout << "\t无效选项！程序退出！ ";
			Sleep(1000);
		}
	}
	return err;
}


//发布公告
int Admi::add_Notification()
{
    system("cls");
    ofstream putf("notification.dat", ios::binary | ios::out | ios::app);
    if (!putf) {
        cerr << "\t无法打开公告文件notification!" << endl;   //这里使用了最后一次理论课学的cerr输出流
        return -1;
    }
    title();//打印页眉
	cout << "\t登录模式 : 管理员\n" << endl;
    string date;
    cout << "\t请输入公告发布时间：";   
    cin >> date;
    cout << "\n\t\t   发布日期 : " << date << endl;

    string note;
    cout << "\t请输入公告内容: \n\t\t";
    cin.ignore(); // 清除之前的输入缓冲
    getline(cin, note);
    putf << "_________________________________________________________________   " << date << "\n" << note << "\n\n\n";
    putf.close();//关闭文件
    cout << "\n\t公告发布完成..." << endl;
    cin.get();
    system("cls");
    return 0;
}

//添加学生：仅允许管理员创建学生的NetID
void Admi::addStu()
{
	system("cls");
	title();
	cout << "   登录模式 : 管理员\n" << endl;
	cout << " 请填写添加学生信息：" << endl;
	cout << endl << "_______________________________________________________________________________" << endl;
	Student s2;
	cout << "\n\t 请输入学生NetID           : "; cin >> s2.NetID;
	cout << "\n\t 请输入学生姓名             : "; cin>>s2.name;
	cout << "\n\t 请输入学生性别             : "; cin >> s2.sex;
	cout << "\n\t 请输入学生学院             : "; cin >> s2.pm;
	cout << "\n\t 请输入学生年级             : "; cin >> s2.grade;
	cout << "\n\t 请输入学生电话             : "; cin >> s2.phone;
	cout << "\n\t 请输入学生邮箱             : "; cin >> s2.email;
	cout << endl << "_______________________________________________________________________________" << endl;
	//将创建的对象加入数据库中
	stu_v.push_back(s2);
	cout << "正在添加学生"<<s2.name<<"······"; 
	Sleep(2000);//让子弹飞一会
	cout << endl << " 学生添加成功！"; cin.get();
}

//删除学生：仅允许管理员删除学生
void Admi::deteStu()
{
	system("cls");
	title();
	cout << "    登录模式 : 管理员\n" << endl;
	cout << endl << "_______________________________________________________________________________" << endl;
	string id;
	cout << "请输入要删除学生的NetID:";
	cin >> id;
	//找到数据库中的对于元素并删除
	auto iter = stu_v.begin();
	int flag = 0;//flag用于表示此学生是否在数据库中
	while (iter != stu_v.end())
	{
		if (iter->NetID == id)
		{
			flag = 1;
			iter = stu_v.erase(iter);//用iter接收返回值
			break;
		}
		else
		{
			iter++;//迭代器递增
		}
	}
	if (flag == 1) {
		cout << "\t已删除此学生" << iter->name << "。" << endl;
	}
	else {
		cout << "\t此学生不存在！";
	}
	_getch();
	cout << "请按任意键返回···" << endl;
	Sleep(2000);
}

//查看学生列表：管理员有权限查看所有学生的情况
void Admi::viewStuList()
{
	system("cls");
	title();
	cout << "   登录模式 : 管理员\n" << endl;
	cout << "\t\t\t  《学生信息表》" << endl;
	cout << "_______________________________________________________________" << endl;
	cout << "\t姓名\t   NetID   \t性别\t年级";
	cout << endl << "_______________________________________________________________" << endl;
	auto iter = stu_v.begin();
	while (iter != stu_v.end())
	{
		cout << "\t" << iter->name << "\t   " << iter->NetID << "\t" << iter->sex << "\t" << iter->grade <<endl;
		iter++;//迭代器递增
	}
	cout <<endl<< "\n\t请按任意键返回···";
	_getch();
	Sleep(100);
}

// 格式化打印函数
void print(const std::string& menuItem) {
	const int width = 60; // 宽度
	std::string border(width, '-'); // 由 '-' 符号组成的边框
	int padding = (width - menuItem.size()) / 2 - 1; // 计算两边的填充空间
	std::string paddingSpaces(padding, ' ');

	std::cout << "|" << paddingSpaces << menuItem << paddingSpaces;
	// 如果长度为奇数，右边填充空间比左边多一个空格
	if (menuItem.size() % 2 != 0) {
		std::cout << " ";
	}
	std::cout << "|" << std::endl;
}

//管理学生成绩
void Admi::mangeSocres()
{
	system("cls");
	title();
	cout << "\t登录模式 : 管理员\n" << endl;
	string id;
	cout << "\t请输入要更改成绩学生的NetID : ";
	cin >> id;
	auto it = stu_v.begin();
	int flag = 0;//flag用于判断是否存在此学生
	while (it != stu_v.end())  //使用STL迭代器在数据库中找到需要更改成绩的学生
	{
		if (it->NetID == id)
		{
			flag = 1;
			break;
		}
		it++;
	}
	if (!flag) //若数据库中不存在此学生，退出程序
	{
		cout << endl << "\t此学生不存在！程序退出···" << endl;
		Sleep(2000);
		return;
	}
	//若数据库中存在此学生,则进一步更改成绩
	const int menuWidth = 60;
	string border(menuWidth, '-'); // 使用 '-' 创建边框
	// 打印菜单的顶部边框
	cout <<endl << border << endl;
	// 打印菜单项
	print("学生当前成绩");
	// 打印菜单的底部边框
	cout << border << endl;
	cout << "\t\tC++程序设计\t\t" << it->cpp[0];
	cout << "\n\t\t数据结构\t\t" << it->data_struct[0];
	cout << "\n\t\t离散数学\t\t" << it->dis_math[0];
	cout << "\n\t\tPython\t\t\t" << it->python[0];
	cout << "\n\t\t机器学习\t\t" << it->m_learning[0];
	cout << endl << "\n\t请输入新的成绩：\n";
	cout << "\t\tC++程序设计 : ";      cin >> it->cpp[0];
	cout << "\n\t\t数据结构 : ";          cin >> it->data_struct[0];
	cout << "\n\t\t离散数学 : ";          cin >> it->dis_math[0];
	cout << "\n\t\tPython : ";              cin >> it->python[0];
	cout << "\n\t\t机器学习 : ";          cin >> it->m_learning[0];
	cout << endl << "\n\t作业成绩更改完成···" << endl;
	cin.get();
	return;
}

//管理员添加老师
void Admi::addTeacher()
{
	system("cls");
	title();
	cout << "  登录模式 : 管理员\n" << endl;
	cout << " 请填写添加教师信息：" << endl;
	cout << endl << "_______________________________________________________________________________" << endl;
	Teacher t2;
	cout << "\n\t 教师NetID          : "; cin >> t2.NetID;
	cout << "\n\t 教师姓名             : "; cin >> t2.name;
	cout << "\n\t 教师性别             : "; cin >> t2.sex;
	cout << "\n\t 教师部门             : "; cin >> t2.pm;
	cout << "\n\t 讲授课程             : "; cin >> t2.subject;
	cout << "\n\t 教师职称             : "; cin >> t2.jobtitle;
	cout << "\n\t 教师电话             : "; cin >> t2.phone;
	cout << "\n\t 教师邮箱             : "; cin >> t2.email;
	cout << endl << "_______________________________________________________________________________" << endl;
	//将创建的对象加入数据库中
	tea_v.push_back(t2);
	cout << "   正在添加教师" << t2.name << "······";
	Sleep(2000);//让子弹飞一会
	cout << endl << "   教师添加成功！"; cin.get();
}

//管理员有权限删除教师
void Admi::deteTeacher()
{
	system("cls");
	title();
	cout << "    登录模式 : 管理员\n" << endl;
	cout << endl << "_______________________________________________________________________________" << endl;
	string id;
	cout << "\t请输入要删除教师的NetID:";
	cin >> id;
	//找到数据库中对应的教师并删除
	auto iter = tea_v.begin();
	int flag = 0;//flag用于表示此学生是否在数据库中
	while (iter != tea_v.end())
	{
		if (iter->NetID == id)
		{
			flag = 1;
			iter = tea_v.erase(iter);//用iter接收返回值
			break;
		}
		else
		{
			iter++;//迭代器递增
		}
	}
	if (flag == 1) {
		cout << "\t已删除此教师。" << iter->name << "。" << endl;
	}
	else {
		cout << "\t此教师不存在！";
	}
	cout << "\n\n\t请按任意键返回···"<<endl;
	_getch();
	Sleep(2000);
}

//管理员有权限查看全体教师信息
void Admi::viewTlist()
{
	system("cls");
	title();
	cout << "   登录模式 : 管理员\n" << endl;
	cout << "\t\t\t     《教师信息表》" << endl;
	cout << "_________________________________________________________________________________" << endl;
	cout << "\t姓名\t   工号 \t性别\t      部门  \t  教授课程  \t职称";
	cout << endl << "_________________________________________________________________________________" << endl;
	auto iter = tea_v.begin();
	while (iter != tea_v.end())
	{
		cout << "\t" << iter->name << "\t   " << iter->NetID << "\t       " << iter->sex << "      " <<   iter->pm  << "  " <<   iter->subject  <<" \t"<<iter->jobtitle;
		iter++;//迭代器递增
	}
	cout << endl << "\n\t请按任意键返回···";
	_getch();
	Sleep(100);
}


//管理员有权限查看全校课室预约情况
void Admi::viewBooking()
{
		system("cls");
		title();//打印页眉
		//读取文件
		ifstream readfile("bookingroom1.dat");
		if (!readfile.is_open()) {
			cout << "\n\t没有预约信息。3秒后自动返回。";
			Sleep(3000);
			return ;
		}

		//打印标题
		const int menuWidth = 60;
		string border(menuWidth, '-'); // 使用 '-' 创建边框
		// 打印菜单的顶部边框
		cout << endl << border << endl;
		// 打印菜单项
		print("闲置场地预约信息");
		// 打印菜单的底部边框
		cout << border << endl;
		cout << "     校区 " << "   预约人" << "  场地类型 " << " 预约时间"<<endl;
		string line;//逐行读取
		while (getline(readfile, line)) {
			cout << "  " << line << "\n ";
		}

		cout << "\n\t请按任意键退出。";
		_getch();
		readfile.close();  //关闭文件
		return;
}