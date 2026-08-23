#include "Teacher.h"
#include "Students.h"
#include"User.h"
#include<iostream>//实现程序基本操作
#include<string.h>//用于字符串操作
#include <conio.h>//控制输出台
#include <windows.h>//用于控制操作台悬停时间
#include <vector>//使用vector容器操作
#include<algorithm>//使用STL迭代器
#include<fstream>// 文件操作 
#include <numeric>
#include <iomanip> // 用于格式化输出
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
//导入外部文件函数
extern void title();
extern void menu();
//导入外部文件变量
extern vector<Student> stu_v;
extern vector<Teacher>tea_v;
//函数声明
bool findID(vector<Teacher>  , string );
void analyzeHomeworkData(const std::string& fileName);

// 格式化打印函数
void printT(const std::string& m) {  //m:menuItem
	const int width = 60; // 宽度
	string border(width, '-'); // 由 '-' 符号组成的边框
	int padding = (width - m.size()) / 2 - 1; // 计算两边的填充空间
	string kongge(padding, ' '); //kongge:空格长度

	cout << "|" << kongge << m << kongge;
	// 如果长度为奇数，右边填充空间比左边多一个空格
	if (m.size() % 2 != 0) {
		cout << " ";
	}
	cout << "|" << std::endl;
}

//登陆函数
int tlogin()
{
	static int err;//静态变量err用于记录用户输入错误次数（每人最多输入3次）
	system("cls");  //清屏
	title();//打印页眉
	string NetID;
	string pword;
	//输入NetID
	cout << "\t登录模式 : 教师" << endl;
	cout << endl << "\t请输入你的NetID :  ";
	cin >> NetID;
	getchar();//这个用来接受最后一个回车

	//输入密码:我在这里进行了优化处理，每输入一个字符，立刻在控制台中打印一个*
	std::cout << "\n\t请输入你的密码 :  ";
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

	//初始密码为NetID,检查是否相等
	if (NetID != pword)
	{
		err++;
		if (3 - err > 0) {//设定只有三次输入机会，否则要退出重新登录
			cout << endl << "\n" << "\tNetID或密码无效！\n" << "\t您还有" << 3 - err << "次输入机会" << "请在5秒后重新输入。" << endl;
			Sleep(5000);//5秒后重新输入
			tlogin();
			return err;
		}
		else {
			cout << "您的机会已用光。程序退出中···" << endl;
			Sleep(1000);//1s后退出
			return err;
		}
	}

	//判断该NetID是否存在于数据库中,NetID只能管理员创建，教师没有权限创建
	int flag = 0;
	if (!findID(tea_v, NetID))
	{
		cout << "\n该NetID不存在！请重新输入！3秒后自动跳转。";
		Sleep(2000);//3秒后重新输入
		tlogin();
		return err;
	}
	else {
		flag = 1;
	}

	//如果登录成功，则可以进入选项菜单
	if (flag)
	{
		cout << "\n\n\t登录成功！页面加载中······" << endl;
		//找到数据库中的对于元素
		auto iter = tea_v.begin();
		while (iter != tea_v.end())
		{
			if (iter->NetID == NetID)
			{
				break;
			}
			else
			{
				iter++;//迭代器递增
			}
		}

	sig1:
		Sleep(2000);//2秒后界面跳转
		system("cls");  //清屏
		title();//打印页眉
		cout << "\t登录模式 : 教师" << endl;
		cout << "\t\t菜单" << endl;
		cout << "\t\t1 : 发布作业\n\t\t2 : 查看学生学习评估 \n\t\t3 : 查看学生作业成绩\n\t\t4 : 管理学生作业成绩 \n\t\t5 : 查看学生作业反馈 \n\t\t6 : 修改个人信息 \n\t\t7 : 退出" << endl;
		cout << "\t请输入你的选择  : ";
		int choice = 0;
		cin >> choice;
		switch (choice)
		{
		case 1:
			iter->add_w();
			goto sig1;  //转会本函数的开始，重新进行菜单选择
			break;
		case 2:
			iter->pinggu();
			goto sig1;
			break;
		case 3:
			iter->viewScores();
			goto sig1;
			break;
		case 4:
			iter->setScores();
			goto sig1;
			break;
		case 5:
			iter->view_fb();
			goto sig1;
			break;
		case 6:
			iter->setinfo();
			goto sig1;
			break;
		case 7:
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

//查找数据库中是否存在该教师
bool findID(vector<Teacher>  teachers, string NetID) {
	for (const auto& teacher : teachers) {
		if (teacher.NetID == NetID) {
			return true;
		}
	}
	return false;
}

//发布作业
int Teacher::add_w()
{
	system("cls");
	ofstream putf("notification.txt", ios::binary | ios::out | ios::app);
	if (!putf) {
		cerr << "\t无法打开公告文件notification!" << endl;   //这里使用了最后一次理论课学的cerr输出流
		return -1;
	}
	title();//打印页眉
	cout << "\t登录模式 : 教师\n" << endl;
	string date;
	cout << "\t请输入作业发布时间：";
	cin >> date;
	cout << "\n\t\t   发布日期 : " << date << endl;

	string note;
	cout << "\t请输入作业内容、要求、截至日期、提交方式等: \n\t\t";
	cin.ignore(); // 清除之前的输入缓冲
	getline(cin, note);
	putf << "_________________________________________________________________   " << date << "\n" << note << "\n\n\n";
	putf.close();//关闭文件
	cout << "\n\t作业公告发布完成..." << endl;
	cin.get();
	system("cls");
	return 0;
}

//查看学生反馈的需要讲的题：使用文件读取操作，允许学生写入、助教和老师读取
int Teacher::view_fb() {
	system("cls");
	//读取文件
	ifstream readfile("feedback.txt", std::ios::in | std::ios::binary);
	if (!readfile.is_open()) {
		cout << "\t没有学生反馈信息。  3秒后自动返回。";
		Sleep(2000);
		return -1;
	}

	title();//打印页眉
	cout << "\n\t学生作业反馈: \n\n";

	string line;//逐行读取
	while (getline(readfile, line)) {
		cout << "\t\t" << line << "\n ";
	}

	cout << "\n\t请老师根据学生反馈调整授课重点。   请按任意键退出。";
	_getch();
	readfile.close();  //关闭文件
	return 0;
}

//这个结构体用来存储作业科目、完成时间和暂停次数
struct HomeworkData {
	std::string subject;
	int completionTime; // 完成时间
	int pauseCount;     // 暂停次数
};
//这个结构体用来存储每科作业的难度等级，和该学生的专注度等级
struct Evaluation {
	std::string subject;
	std::string focusLevel;     // 专注度等级
	std::string difficultyLevel; // 难度等级
};

//教师可以查询学生完成作业的学习专注度评估
void Teacher::pinggu()
{
	//这里我自己定义了学习专注度评估算法
	// 主要依据学生的作业完成时长、中途暂停次数对学生写作业时的状态进行评估
	//作业完成时长、中途暂停次数的信息在学生学习端被记录，保存在data.txt文件中
	std::string fileName = "homework_data.txt";
	analyzeHomeworkData(fileName);
}

void analyzeHomeworkData(const std::string& fileName) {
	std::ifstream file(fileName);
	std::vector<HomeworkData> data;
	std::string line;

	system("cls");  //清屏
	title();//打印页眉

	if (!file.is_open()) {
		std::cerr << "\n\t无法打开文件 " << fileName << std::endl;
		return;
	}
	//逐行读取
	while (getline(file, line)) {
		std::istringstream iss(line);
		HomeworkData hw;  //每一行创建一个HomeworkData变量
		if (iss >> hw.subject >> hw.completionTime >> hw.pauseCount) {
			data.push_back(hw);
		}
	}

	file.close();
	std::cout << "\n\t\t\t      学生张瑞程的各科作业分析" << std::endl;
	cout << "\t-----------------------------------------------------------------------" << endl;
	if (data.empty()) {
		std::cerr << "\n\t数据为空！" << std::endl;
		return;
	}

	// 计算平均值,包括用时平均值和停止次数平均值
	double avgTime = accumulate(data.begin(), data.end(), 0.0,
		[](double sum, const HomeworkData& hw) { return sum + hw.completionTime; }) / data.size();
	double avgPauseCount = accumulate(data.begin(), data.end(), 0.0,
		[](double sum, const HomeworkData& hw) { return sum + hw.pauseCount; }) / data.size();

	// 分析并输出结果
	for (const auto& hw : data) {
		Evaluation eval;
		eval.subject = hw.subject;

		// 评判专注度，以暂停次数的0.75和1.25为阈值划分高、中、低
		eval.focusLevel = hw.pauseCount < avgPauseCount * 0.75 ? "高" :
			hw.pauseCount <= avgPauseCount * 1.25 ? "中" : "低";

		// 评判难度，以完成时间的0.75和1.25为阈值划分高、中、低
		eval.difficultyLevel = hw.completionTime > avgTime * 1.25 ? "高" :
			hw.completionTime >= avgTime * 0.75 ? "中" : "低";

		cout << setw(15) << "\n\t作业科目: " << setw(20) << eval.subject << setw(20)
			<< " 专注度等级: " << eval.focusLevel << setw(20)
			<< " 难度等级评估: " << eval.difficultyLevel << endl;
	}
	cout << "\n\n\t\t请按任意键退出。" << endl;
	_getch();//当用户按下一个键后程序继续进行
}

//查看个人信息
void Teacher::info() {
	system("cls");  //清屏
	title();//打印页眉
	cout << "\t登录模式 : 教师" << endl;
	cout << "\t\t个人信息" << endl;
	cout << "\t\t姓名:" << name;
	cout << "\n\t\t性别" << sex;
	cout << "\n\t\tNetID:" << NetID;
	cout << "\n\t\t教授课程:" << subject;
	cout << "\n\t\t学院:" << pm;
	cout << "\n\t\t职称:" << jobtitle;
	cout << "\n\t\t邮箱：" << email;
	cout << "\n\t\t电话：" << phone;
	cout << "\n\n\t\t请按任意键退出。" << endl;
	_getch();//当用户按下一个键后程序继续进行
	Sleep(1000);
}

//更改个人信息
void Teacher::setinfo() {
	system("cls");  //清屏
	title();//打印页眉
	cout << "  登录模式 : 教师" << endl;
	cout << "\t当前个人信息" << endl;
	cout << "\t\t姓名:\t\t" << name;
	cout << "\n\t\t性别\t\t" << sex;
	cout << "\n\t\tNetID:\t\t" << NetID;
	cout << "\n\t\t教授课程:\t\t" << subject;  //显示旧信息
	cout << "\n\t\t学院:\t\t" << pm;
	cout << "\n\t\t职称:\t\t" << jobtitle;
	cout << "\n\t\t邮箱:\t\t" << email;
	cout << "\n\t\t电话:\t\t" << phone;
	cout << "\n-----------------------------------------------------------------------------------------------------------------------\n" << endl;
	cout << "\t请输入新的个人信息：";
	cout << "\n\t\t姓名:\t\t";			cin >> name;
	cout << "\n\t\t性别:\t\t";			cin >> sex;
	cout << "\n\t\tNetID:\t\t" << NetID;
	cout << "\n\t\t教授课程:\t\t";   cin >> subject;     //更改数据
	cout << "\n\t\t学院:\t\t";			cin >> pm;
	cout << "\n\t\t职称:\t\t";			cin >> jobtitle;
	cout << "\n\t\t邮箱:\t\t";		    cin >> email;
	cout << "\n\t\t电话:\t\t";		    cin >> phone;
	cout << "\n\t\t修改完成！正在更新数据库···" << endl;
	//STL容器可以自动维护数据库中信息,
	Sleep(2000);//
}

//教师评定学生作业成绩：教师只能评定其所授科目的作业
void Teacher::setScores()
{
	system("cls");  //清屏
	title();//打印页眉
	cout << "  登录模式 : 教师" << endl;
	//显示当前成绩
	const int menuWidth = 60;
	string border(menuWidth, '-'); // 使用 '-' 创建边框
	// 打印顶部边框
	cout << border << endl;
	printT("学生当前成绩");
	// 打印底部边框
	cout << border << endl;
	cout << "\t姓名\t\t学号\t\t成绩" << endl;
	cout << "------------------------------------------------------------" << endl;
	for (auto stu = stu_v.begin(); stu != stu_v.end(); stu++) //使用STL迭代器输出所有学生该科成绩
	{
		cout << "\t" << stu->name << "\t      " << stu->NetID << "\t         ";
		if (subject == "C++程序设计")cout << stu->cpp[0] << endl;
		else if (subject == "数据结构")cout << stu->data_struct[0] << endl;
		else if (subject == "离散数学")cout << stu->dis_math[0] << endl;
		else if (subject == "机器学习")cout << stu->m_learning[0] << endl;
		else if (subject == "信号与系统")cout << stu->sig_sys[0] << endl;
		else cout << stu->python[0] << endl;
	}
	cout << "------------------------------------------------------------" << endl;
	cout << "\n\t请输入要评定的学生学号 : ";
	string id;   cin >> id;
	//找到数据库中的对应的学生
	auto iter = stu_v.begin();
	while (iter != stu_v.end())
	{
		if (iter->NetID == id)
		{
			break;
		}
		else
		{
			++iter;//迭代器递增
		}
	}
	if (iter == stu_v.end())
	{
		cout << "\n\t该学生不存在！" << endl;
	}
	else {
		cout << "\n\t请输入该学生本次作业成绩：";
		
		if (subject == "C++程序设计")		cin>> iter->cpp[0] ;
		else if (subject == "数据结构")		cin >>iter->data_struct[0] ;
		else if (subject == "离散数学")		cin >> iter->dis_math[0] ;
		else if (subject == "机器学习")		cin >> iter->m_learning[0] ;
		else if (subject == "信号与系统")		cin >> iter->sig_sys[0];
		else cin>>iter->python[0] ;
	}
	cout<<"\n\n\t作业成绩评定成功！"<<endl;
	Sleep(3000);//3s后跳转
}

//教师查看学生成绩：教师只允许查看所有学生其教授课程的成绩，查看全部成绩需要通过管理员
void Teacher::viewScores()
{
	system("cls");  //清屏
	title();//打印页眉
	cout << "  登录模式 : 教师" << endl;
	//成绩单将以表格的形式呈现
	string sub = subject;
	sub += "成绩统计表";//字符串拼接“XXX成绩统计表”
	const int menuWidth = 60;
	string border(menuWidth, '-'); // 使用 '-' 创建边框
	// 打印顶部边框
	cout << border << endl;
	printT(sub);
	// 打印底部边框
	cout << border << endl;

	cout << "\t姓名\t\t学号\t\t成绩" << endl;
	cout << "------------------------------------------------------------" << endl;
	for (auto stu = stu_v.begin(); stu != stu_v.end(); stu++) //使用STL迭代器输出所有学生该科成绩
	{
		cout << "\t" << stu->name << "\t      " << stu->NetID << "\t         " ;
		if (subject == "C++程序设计")cout << stu->cpp[0]<<endl;
		else if (subject == "数据结构")cout << stu->data_struct[0]<<endl;
		else if (subject == "离散数学")cout << stu->dis_math[0]<<endl;
		else if (subject == "机器学习")cout << stu->m_learning[0]<<endl;
		else cout << stu->python[0]<<endl;
	}
	cout << "------------------------------------------------------------" << endl;
	cout << "\n\t请按任意键退出。";
	_getch();
}