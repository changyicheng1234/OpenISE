#include "Assit.h"
#include "../../../360MoveData/Users/DELL/Desktop/1/Students.h"

//导入外部文件函数
extern void title();
extern void menu();
extern void printT(const std::string& m);

//导入外部文件变量
extern vector<Assit> assit_v;
extern vector<Student> stu_v;

//以下静态变量记录了各校区各种类型的课室的初始剩余个数
//为防止此变量被其他文件修改，故设置位静态变量，每天开始更新一次
static int sroom[5] = { 30,20,40,60,50 };
static int mroom[5] = { 30,20,40,60,50 };
static int lroom[5] = { 10,8,15,25,20 };
static int gym[5] = { 60,80,100,100,100 };
static int proom[5] = { 15,10,15,25,25 };
static int meetingroom[5] = { 33,25,35,45,40 };


//查找数据库中是否存在该助教
bool findID(vector<Assit>  students, string NetID) {
	for (const auto& student : students) {
		if (student.NetID == NetID) {
			return true;
		}
	}
	return false;
}

//登陆函数
int aslogin()
{
	static int err;
	system("cls");  //清屏
	title();//打印页眉
	string NetID;
	string pword;
	//输入NetID
	cout << "\t登录模式 : 助教" << endl;
	cout << endl << "\t请输入你的NetID :  ";
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

	//初始密码为NetID,检查是否相等
	if (NetID != pword)
	{
		err++;
		if (3 - err > 0) {//设定只有三次输入机会，否则要退出重新登录
			cout << endl << "\n" << "\tNetID或密码无效！\n" << "\t您还有" << 3 - err << "次输入机会" << "请在5秒后重新输入。" << endl;
			Sleep(3000);//5秒后重新输入
			aslogin();
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
	if (!findID(assit_v, NetID))
	{
		cout << "\n\n\t该NetID不存在！请重新输入！3秒后自动跳转。";
		Sleep(2000);//3秒后重新输入
		aslogin();
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
		auto iter = assit_v.begin();
		while (iter != assit_v.end())
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
		cout << "\t登录模式 : 助教" << endl;
		cout << "\t\t菜单" << endl;
		cout << "\t\t1 : 录入作业成绩\n\t\t2 : 预约习题课课室\n\t\t3 : 发布习题课公告\n\t\t4 : 查看学生作业反馈\n\t\t5 : 修改个人信息 \n\t\t6 : 退出" << endl;
		cout << "\t请输入你的选择  : ";
		int choice = 0;
		cin >> choice;
		switch (choice)
		{
		case 1:
			//iter->info();
			iter->setScores();
			goto sig1;  //转回本函数的开始，重新进行菜单选择
			break;
		case 2:
			iter->bookingroom();
			goto sig1;
			break;
		case 3:
			iter->xitike();
			goto sig1;
			break;
		case 4:
			iter->view_fb();
			goto sig1;
			break;
		case 5:
			iter->setinfo();
			goto sig1;
			break;
		case 6:
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


//助教评定学生作业成绩：助教只能评定其科目的作业
void Assit::setScores()
{
	system("cls");  //清屏
	title();//打印页眉
	cout << "  登录模式 : 助教" << endl;
	//显示当前成绩
	const int menuWidth = 60;
	string border(menuWidth, '-'); // 使用 '-' 创建边框
	// 打印顶部边框
	cout << border << endl;
	printT("学生当前作业成绩");
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
		cout << "\n\t请输入该学生作业成绩：";

		if (subject == "C++程序设计")		cin >> iter->cpp[0];
		else if (subject == "数据结构")		cin >> iter->data_struct[0];
		else if (subject == "离散数学")		cin >> iter->dis_math[0];
		else if (subject == "机器学习")		cin >> iter->m_learning[0];
		else if (subject == "信号与系统")		cin >> iter->sig_sys[0];
		else cin >> iter->python[0];
	}
	cout << "\n\t作业成绩评定成功！" << endl;
	Sleep(3000);
}

//查看学生反馈的需要讲的题：使用文件读取操作，允许学生写入、助教和老师读取
int Assit::view_fb() {
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

	cout << "\n\t请助教根据学生反馈安排习题讲解。   请按任意键退出。";
	_getch();
	readfile.close();  //关闭文件
	return 0;
}

//助教更改个人信息
void Assit::setinfo() {
	system("cls");  //清屏
	title();//打印页眉
	cout << "\t登录模式 : 助教" << endl;
	cout << "\t\t当前个人信息" << endl;
	cout << "\t\t姓名: " << name;
	cout << "\n\t\t性别: " << sex;
	cout << "\n\t\t科目:  " << subject;
	cout << "\n\t\tNetID: " << NetID;         //显示旧数据
	cout << "\n\t\t年级: " << grade;
	cout << "\n\t\t学院: " << pm;
	cout << "\n\t\t邮箱: " << email;
	cout << "\n\t\t电话: " << phone;
	cout << "\n-----------------------------------------------------------------------------------------------------------------------\n" << endl;
	cout << "请输入新的个人信息: ";
	cout << "\n\t\t姓名: ";  cin >> name;
	cout << "\n\t\t性别: ";  cin >> sex;
	cout << "\n\t\t科目: ";  cin >> subject;
	cout << "\n\t\tNetID: ";  cin >> NetID;
	cout << "\n\t\t年级: ";  cin >> grade;     //更改数据
	cout << "\n\t\t学院: ";  cin >> pm;
	cout << "\n\t\t邮箱: ";  cin >> email;
	cout << "\n\t\t电话: ";  cin >> phone;
	cout << "\n\t\t修改完成！正在更新数据库···" << endl;
	//STL容器可以自动维护数据库中信息,
	Sleep(3000);//
}

//发布习题课时间、地址、内容，提醒学生按时参加
int Assit::xitike()
{
	system("cls");
	ofstream putf("notification.txt", ios::binary | ios::out | ios::app);
	if (!putf) {
		cerr << "\t无法打开公告文件notification!" << endl;   //这里使用了最后一次理论课学的cerr输出流
		return -1;
	}
	title();//打印页眉
	cout << "\t登录模式 : 助教\n" << endl;
	string date;
	cout << "\t请输入公告发布时间：";
	cin >> date;
	cout << "\n\t\t   发布日期 : " << date << "\n"<<endl;

	string note;
	cout << "\t请输入习题课内容、时间、地点等: \n\t\t";
	cin.ignore(); // 清除之前的输入缓冲
	getline(cin, note);
	putf << "_________________________________________________________________   " << date << "\n" << note << "\n\n\n";
	putf.close();//关闭文件
	cout << "\n\t习题课公告发布完成..." << endl;
	cin.get();
	system("cls");
	return 0;
}

//助教预约习题课教室
void Assit::bookingroom()
{
	system("cls");  //清屏
	title();//打印页眉
	cout << "   登录模式 : 助教" << endl;
	cout << "——————————————————————————————————————————————————";
	cout << "\n\t请选择你所在的校区：" << endl;
	cout << "\t\t【1】广州南校区" << endl;
	cout << "\t\t【2】广州北校区" << endl;
	cout << "\t\t【3】广州东校区" << endl;
	cout << "\t\t【4】深圳校区" << endl;
	cout << "\t\t【5】珠海校区" << endl;
	cout << "\t\t【6】退出" << endl;
	cout << "\n\t请输入你的选择：";
	int choice;
	cin >> choice;
	// 根据用户输入进入不同的预约界面
	switch (choice) {
	case 1:
		gzn();//广州南校区场地预约系统
		break;
	case 2:
		gzb();//广州北校区场地预约系统
		break;
	case 3:
		gzd();//广州东校区场地预约系统
		break;
	case 4:
		shenzhen();//深圳校区场地预约系统
		break;
	case 5:
		zhuhai();//珠海校区场地预约系统
		break;
	case 6:
		cout << "\t退出系统中······" << endl;
		Sleep(2000);//让子弹飞一会~
		break;
	default:
		cout << "\t无效选项!" << endl;
		Sleep(2000);
		break;
	}
}

// 格式化打印函数
void printAS(const std::string& menuItem) {
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

//深圳校区课室预约系统
void Assit::shenzhen()
{
	system("cls");  //清屏
	title();//打印页眉
	cout << "   登录模式 : 助教" << endl;

	const int menuWidth = 60;
	string border(menuWidth, '-'); // 使用 '-' 创建边框
	// 打印菜单的顶部边框
	cout << endl << border << endl;
	// 打印菜单项
	printAS("深圳校区课室预约系统");
	// 打印菜单的底部边框
	cout << border << endl;

	cout << "\t欢迎来到深圳校区课室预约系统！" << endl;
	cout << "\t\t【1】小型课室    剩余：" << sroom[3] << "间。" << endl;
	cout << "\t\t【2】中型课室    剩余：" << mroom[3] << "间。" << endl;
	cout << "\t\t【3】大型课室    剩余：" << sroom[3] << "间。" << endl;
	cout << "\t\t【4】会议室      剩余：" << meetingroom[3] << "间。" << endl;
	cout << "\t\t【5】音乐教室      剩余：" << proom[3] << "个。" << endl;
	cout << "\t\t【6】体育课室      剩余：" << gym[3] << "间。" << endl;
	cout << "\t请选择你要预约的课室类型：";
	int choice1;
	cin >> choice1;//用户输入预约课室种类
	cout << "\t预约时间：" << endl;
	cout << "\t\t【1】1~2节    【2】3~4节" << endl;
	cout << "\t\t【3】5~6节    【4】7~8节" << endl;
	cout << "\t\t【5】9~10节   【6】11~12节" << endl;
	cout << "\n\t请选择你要预约的时间：";
	int choice2;
	cin >> choice2;//用户提供预约时间

	int flag = 0;//flag用于标志预约是否成功

	string t;//课室种类
	string time;//预定时间
	switch (choice1) {
	case 1:
		if (sroom[3] > 0)//判断课室是否有剩余
		{
			cout << "\t小型课室一间预约成功！" << endl;
			sroom[3]--;  flag = 1;
			t = "小型课室";
		}
		break;
	case 2:
		if (mroom[3] > 0)//判断课室是否有剩余
		{
			cout << "\t中型课室一间预约成功！" << endl;
			mroom[3]--;   flag = 1;  //该类型剩余课室数减1
			t = "中型课室";
		}
		break;
	case 3:
		if (lroom[3] > 0)//判断课室是否有剩余
		{
			cout << "\t大型课室一间预约成功！" << endl;
			lroom[3]--;   flag = 1;
			t = "大型课室";
		}
		break;
	case 4:
		if (meetingroom[3] > 0)//判断课室是否有剩余
		{
			cout << "\t会议室一间预约成功！" << endl;
			meetingroom[3]--;   flag = 1;
			t = "会议室";
		}
		break;
	case 5:
		if (proom[3] > 0)//判断课室是否有剩余
		{
			cout << "\t音乐教室一间预约成功！" << endl;
			proom[3]--;   flag = 1;
			t = "音乐教室";
		}
		break;
	case 6:
		if (gym[3] > 0)//判断课室是否有剩余
		{
			cout << "\t体育课室一间预约成功！" << endl;
			gym[3]--;   flag = 1;
			t = "体育课室";
		}
		break;
	}
	if (flag == 0)
	{
		cout << "\n\t预约失败！程序退出中···" << endl;//flag用于标志预约是否成功
		Sleep(2000);
	}
	//确定time
	switch (choice2) {
	case 1:
		time = "第1~2节";
		break;
	case 2:
		time = "第3~4节";
		break;
	case 3:
		time = "第5~6节";
		break;
	case 4:
		time = "第7~8节";
		break;
	case 5:
		time = "第9~10节";
		break;
	case 6:
		time = "第11~12节";
		break;
	}
	if (flag == 0)
	{
		cout << "\n\t预约失败！程序退出中···" << endl;//flag用于标志预约是否成功
		Sleep(2000);
	}

	//课室预约需要对预约人的信息进行记录，方便管理员查看和管理
	//这里使用文件操作来完成此功能
	ofstream putf("bookingroom1.dat", ios::binary | ios::out | ios::app);
	if (!putf) {
		cerr << "\n\t预约失败！程序退出中···" << endl;   //这里使用了最后一次理论课学的cerr输出流
		Sleep(2000);
		return;
	}
	putf << "______________________________________________________________  " << "\n深圳校区  " << name << "  " << t << "  " << time << "\n\n";
	putf.close();//关闭文件

	cout << "\n\t请按任意键退出···" << endl;
	_getch();
}


//珠海校区课室预约系统
void Assit::zhuhai()
{
	system("cls");  //清屏
	title();//打印页眉
	cout << "   登录模式 : 助教" << endl;

	const int menuWidth = 60;
	string border(menuWidth, '-'); // 使用 '-' 创建边框
	// 打印菜单的顶部边框
	cout << endl << border << endl;
	// 打印菜单项
	printAS("珠海校区课室预约系统");
	// 打印菜单的底部边框
	cout << border << endl;

	cout << "\t欢迎来到珠海校区课室预约系统！" << endl;
	cout << "\t\t【1】小型课室    剩余：" << sroom[4] << "间。" << endl;
	cout << "\t\t【2】中型课室    剩余：" << mroom[4] << "间。" << endl;
	cout << "\t\t【3】大型课室    剩余：" << sroom[4] << "间。" << endl;
	cout << "\t\t【4】会议室      剩余：" << meetingroom[4] << "间。" << endl;
	cout << "\t\t【5】音乐教室      剩余：" << proom[4] << "间。" << endl;
	cout << "\t\t【6】体育课室      剩余：" << gym[4] << "间。" << endl;
	cout << "\t请选择你要预约的课室类型：";
	int choice1;
	cin >> choice1;//用户输入预约课室种类
	cout << "\t预约时间：" << endl;
	cout << "\t\t【1】1~2节    【2】3~4节" << endl;
	cout << "\t\t【3】5~6节    【4】7~8节" << endl;
	cout << "\t\t【5】9~10节   【6】11~12节" << endl;
	cout << "\n\t请选择你要预约的时间：";
	int choice2;
	cin >> choice2;//用户提供预约时间

	int flag = 0;//flag用于标志预约是否成功

	string t;//课室种类
	string time;//预定时间
	switch (choice1) {
	case 1:
		if (sroom[4] > 0)//判断课室是否有剩余
		{
			cout << "\t小型课室一间预约成功！" << endl;
			sroom[4]--;  flag = 1;
			t = "小型课室";
		}
		break;
	case 2:
		if (mroom[4] > 0)//判断课室是否有剩余
		{
			cout << "\t中型课室一间预约成功！" << endl;
			mroom[4]--;   flag = 1;  //该类型剩余课室数减1
			t = "中型课室";
		}
		break;
	case 3:
		if (lroom[4] > 0)//判断课室是否有剩余
		{
			cout << "\t大型课室一间预约成功！" << endl;
			lroom[4]--;   flag = 1;
			t = "大型课室";
		}
		break;
	case 4:
		if (meetingroom[4] > 0)//判断课室是否有剩余
		{
			cout << "\t会议室一间预约成功！" << endl;
			meetingroom[4]--;   flag = 1;
			t = "会议室";
		}
		break;
	case 5:
		if (proom[4] > 0)//判断课室是否有剩余
		{
			cout << "\t音乐课室一间预约成功！" << endl;
			proom[4]--;   flag = 1;
			t = "音乐课室";
		}
		break;
	case 6:
		if (gym[4] > 0)//判断课室是否有剩余
		{
			cout << "\t体育课室一位预约成功！" << endl;
			gym[4]--;   flag = 1;
			t = "体育课室";
		}
		break;
	}
	if (flag == 0)
	{
		cout << "\n\t预约失败！程序退出中···" << endl;//flag用于标志预约是否成功
		Sleep(2000);
	}
	//确定time
	switch (choice2) {
	case 1:
		time = "第1~2节";
		break;
	case 2:
		time = "第3~4节";
		break;
	case 3:
		time = "第5~6节";
		break;
	case 4:
		time = "第7~8节";
		break;
	case 5:
		time = "第9~10节";
		break;
	case 6:
		time = "第11~12节";
		break;
	}
	if (flag == 0)
	{
		cout << "\n\t预约失败！程序退出中···" << endl;//flag用于标志预约是否成功
		Sleep(2000);
	}

	//课室预约需要对预约人的信息进行记录，方便管理员查看和管理
	//这里使用文件操作来完成此功能
	ofstream putf("bookingroom1.dat", ios::binary | ios::out | ios::app);
	if (!putf) {
		cerr << "\n\t预约失败！程序退出中···" << endl;   //这里使用了最后一次理论课学的cerr输出流
		Sleep(2000);
		return;
	}
	putf << "______________________________________________________________  " << "\n珠海校区  " << name << "  " << t << "  " << time << "\n\n";
	putf.close();//关闭文件

	cout << "\n\t请按任意键退出···" << endl;
	_getch();
}

//广州南校区课室预约系统
void Assit::gzn()
{
	system("cls");  //清屏
	title();//打印页眉
	cout << "   登录模式 : 助教" << endl;

	const int menuWidth = 60;
	string border(menuWidth, '-'); // 使用 '-' 创建边框
	// 打印菜单的顶部边框
	cout << endl << border << endl;
	// 打印菜单项
	printAS("广州南校区课室预约系统");
	// 打印菜单的底部边框
	cout << border << endl;

	cout << "\t欢迎来到广州南校区课室预约系统！" << endl;
	cout << "\t\t【1】小型课室    剩余：" << sroom[0] << "间。" << endl;
	cout << "\t\t【2】中型课室    剩余：" << mroom[0] << "间。" << endl;
	cout << "\t\t【3】大型课室    剩余：" << sroom[0] << "间。" << endl;
	cout << "\t\t【4】会议室      剩余：" << meetingroom[0] << "间。" << endl;
	cout << "\t\t【5】音乐课室      剩余：" << proom[0] << "间。" << endl;
	cout << "\t\t【6】体育教室      剩余：" << gym[0] << "间。" << endl;
	cout << "\t请选择你要预约的课室类型：";
	int choice1;
	cin >> choice1;//用户输入预约课室种类
	cout << "\t预约时间：" << endl;
	cout << "\t\t【1】1~2节    【2】3~4节" << endl;
	cout << "\t\t【3】5~6节    【4】7~8节" << endl;
	cout << "\t\t【5】9~10节   【6】11~12节" << endl;
	cout << "\n\t请选择你要预约的时间：";
	int choice2;
	cin >> choice2;//用户提供预约时间

	int flag = 0;//flag用于标志预约是否成功

	string t;//课室种类
	string time;//预定时间
	switch (choice1) {
	case 1:
		if (sroom[0] > 0)//判断课室是否有剩余
		{
			cout << "\t小型课室一间预约成功！" << endl;
			sroom[0]--;  flag = 1;
			t = "小型课室";
		}
		break;
	case 2:
		if (mroom[0] > 0)//判断课室是否有剩余
		{
			cout << "\t中型课室一间预约成功！" << endl;
			mroom[0]--;   flag = 1;  //该类型剩余课室数减1
			t = "中型课室";
		}
		break;
	case 3:
		if (lroom[0] > 0)//判断课室是否有剩余
		{
			cout << "\t大型课室一间预约成功！" << endl;
			lroom[0]--;   flag = 1;
			t = "大型课室";
		}
		break;
	case 4:
		if (meetingroom[0] > 0)//判断课室是否有剩余
		{
			cout << "\t会议室一间预约成功！" << endl;
			meetingroom[0]--;   flag = 1;
			t = "会议室";
		}
		break;
	case 5:
		if (proom[0] > 0)//判断课室是否有剩余
		{
			cout << "\t音乐课室一间预约成功！" << endl;
			proom[0]--;   flag = 1;
			t = "音乐课室";
		}
		break;
	case 6:
		if (gym[0] > 0)//判断课室是否有剩余
		{
			cout << "\t体育教室一位预约成功！" << endl;
			gym[0]--;   flag = 1;
			t = "体育教室";
		}
		break;
	}
	if (flag == 0)
	{
		cout << "\n\t预约失败！程序退出中···" << endl;//flag用于标志预约是否成功
		Sleep(2000);
	}
	//确定time
	switch (choice2) {
	case 1:
		time = "第1~2节";
		break;
	case 2:
		time = "第3~4节";
		break;
	case 3:
		time = "第5~6节";
		break;
	case 4:
		time = "第7~8节";
		break;
	case 5:
		time = "第9~10节";
		break;
	case 6:
		time = "第11~12节";
		break;
	}
	if (flag == 0)
	{
		cout << "\n\t预约失败！程序退出中···" << endl;//flag用于标志预约是否成功
		Sleep(2000);
	}

	//课室预约需要对预约人的信息进行记录，方便管理员查看和管理
	//这里使用文件操作来完成此功能
	ofstream putf("bookingroom.dat", ios::binary | ios::out | ios::app);
	if (!putf) {
		cerr << "\n\t预约失败！程序退出中···" << endl;   //这里使用了最后一次理论课学的cerr输出流
		Sleep(2000);
		return;
	}
	putf << "______________________________________________________________  " << "\n南校区  " << name << "  " << t << "  " << time << "\n\n";
	putf.close();//关闭文件

	cout << "\n\t请按任意键退出···" << endl;
	_getch();
}


//广州东校区课室预约系统
void Assit::gzd()
{
	system("cls");  //清屏
	title();//打印页眉
	cout << "   登录模式 : 助教" << endl;

	const int menuWidth = 60;
	string border(menuWidth, '-'); // 使用 '-' 创建边框
	// 打印菜单的顶部边框
	cout << endl << border << endl;
	// 打印菜单项
	printAS("广州东校区课室预约系统");
	// 打印菜单的底部边框
	cout << border << endl;

	cout << "\t欢迎来到广州东校区课室预约系统！" << endl;
	cout << "\t\t【1】小型课室    剩余：" << sroom[2] << "间。" << endl;
	cout << "\t\t【2】中型课室    剩余：" << mroom[2] << "间。" << endl;
	cout << "\t\t【3】大型课室    剩余：" << sroom[2] << "间。" << endl;
	cout << "\t\t【4】会议室      剩余：" << meetingroom[2] << "间。" << endl;
	cout << "\t\t【5】音乐教室      剩余：" << proom[2] << "间。" << endl;
	cout << "\t\t【6】体育课室      剩余：" << gym[2] << "间。" << endl;
	cout << "\t请选择你要预约的课室类型：";
	int choice1;
	cin >> choice1;//用户输入预约课室种类
	cout << "\t预约时间：" << endl;
	cout << "\t\t【1】1~2节    【2】3~4节" << endl;
	cout << "\t\t【3】5~6节    【4】7~8节" << endl;
	cout << "\t\t【5】9~10节   【6】11~12节" << endl;
	cout << "\n\t请选择你要预约的时间：";
	int choice2;
	cin >> choice2;//用户提供预约时间

	int flag = 0;//flag用于标志预约是否成功

	string t;//课室种类
	string time;//预定时间
	switch (choice1) {
	case 1:
		if (sroom[2] > 0)//判断课室是否有剩余
		{
			cout << "\t小型课室一间预约成功！" << endl;
			sroom[2]--;  flag = 1;
			t = "小型课室";
		}
		break;
	case 2:
		if (mroom[2] > 0)//判断课室是否有剩余
		{
			cout << "\t中型课室一间预约成功！" << endl;
			mroom[2]--;   flag = 1;  //该类型剩余课室数减1
			t = "中型课室";
		}
		break;
	case 3:
		if (lroom[2] > 0)//判断课室是否有剩余
		{
			cout << "\t大型课室一间预约成功！" << endl;
			lroom[2]--;   flag = 1;
			t = "大型课室";
		}
		break;
	case 4:
		if (meetingroom[2] > 0)//判断课室是否有剩余
		{
			cout << "\t会议室一间预约成功！" << endl;
			meetingroom[2]--;   flag = 1;
			t = "会议室";
		}
		break;
	case 5:
		if (proom[2] > 0)//判断课室是否有剩余
		{
			cout << "\t音乐教室一间预约成功！" << endl;
			proom[2]--;   flag = 1;
			t = "音乐教室";
		}
		break;
	case 6:
		if (gym[2] > 0)//判断课室是否有剩余
		{
			cout << "\t体育课室一间预约成功！" << endl;
			gym[2]--;   flag = 1;
			t = "体育课室";
		}
		break;
	}
	if (flag == 0)
	{
		cout << "\n\t预约失败！程序退出中···" << endl;//flag用于标志预约是否成功
		Sleep(2000);
	}
	//确定time
	switch (choice2) {
	case 1:
		time = "第1~2节";
		break;
	case 2:
		time = "第3~4节";
		break;
	case 3:
		time = "第5~6节";
		break;
	case 4:
		time = "第7~8节";
		break;
	case 5:
		time = "第9~10节";
		break;
	case 6:
		time = "第11~12节";
		break;
	}
	if (flag == 0)
	{
		cout << "\n\t预约失败！程序退出中···" << endl;//flag用于标志预约是否成功
		Sleep(2000);
	}

	//课室预约需要对预约人的信息进行记录，方便管理员查看和管理
	//这里使用文件操作来完成此功能
	ofstream putf("bookingroom1.dat", ios::binary | ios::out | ios::app);
	if (!putf) {
		cerr << "\n\t预约失败！程序退出中···" << endl;   //这里使用了最后一次理论课学的cerr输出流
		Sleep(2000);
		return;
	}
	putf << "______________________________________________________________  " << "\n东校区  " << name << "  " << t << "  " << time << "\n\n";
	putf.close();//关闭文件

	cout << "\n\t请按任意键退出···" << endl;
	_getch();
}


//广州北校区课室预约系统
void Assit::gzb()
{
	system("cls");  //清屏
	title();//打印页眉
	cout << "   登录模式 : 助教" << endl;

	const int menuWidth = 60;
	string border(menuWidth, '-'); // 使用 '-' 创建边框
	// 打印菜单的顶部边框
	cout << endl << border << endl;
	// 打印菜单项
	printAS("广州北校区课室预约系统");
	// 打印菜单的底部边框
	cout << border << endl;

	cout << "\t欢迎来到广州北校区课室预约系统！" << endl;
	cout << "\t\t【1】小型课室    剩余：" << sroom[1] << "间。" << endl;
	cout << "\t\t【2】中型课室    剩余：" << mroom[1] << "间。" << endl;
	cout << "\t\t【3】大型课室    剩余：" << sroom[1] << "间。" << endl;
	cout << "\t\t【4】会议室      剩余：" << meetingroom[1] << "间。" << endl;
	cout << "\t\t【5】音乐教室      剩余：" << proom[1] << "间。" << endl;
	cout << "\t\t【6】体育课室      剩余：" << gym[1] << "间。" << endl;
	cout << "\t请选择你要预约的课室类型：";
	int choice1;
	cin >> choice1;//用户输入预约课室种类
	cout << "\t预约时间：" << endl;
	cout << "\t\t【1】1~2节    【2】3~4节" << endl;
	cout << "\t\t【3】5~6节    【4】7~8节" << endl;
	cout << "\t\t【5】9~10节   【6】11~12节" << endl;
	cout << "\n\t请选择你要预约的时间：";
	int choice2;
	cin >> choice2;//用户提供预约时间

	int flag = 0;//flag用于标志预约是否成功

	string t;//课室种类
	string time;//预定时间
	switch (choice1) {
	case 1:
		if (sroom[1] > 0)//判断课室是否有剩余
		{
			cout << "\t小型课室一间预约成功！" << endl;
			sroom[1]--;  flag = 1;
			t = "小型课室";
		}
		break;
	case 2:
		if (mroom[1] > 0)//判断课室是否有剩余
		{
			cout << "\t中型课室一间预约成功！" << endl;
			mroom[1]--;   flag = 1;  //该类型剩余课室数减1
			t = "中型课室";
		}
		break;
	case 3:
		if (lroom[1] > 0)//判断课室是否有剩余
		{
			cout << "\t大型课室一间预约成功！" << endl;
			lroom[1]--;   flag = 1;
			t = "大型课室";
		}
		break;
	case 4:
		if (meetingroom[1] > 0)//判断课室是否有剩余
		{
			cout << "\t会议室一间预约成功！" << endl;
			meetingroom[1]--;   flag = 1;
			t = "会议室";
		}
		break;
	case 5:
		if (proom[1] > 0)//判断课室是否有剩余
		{
			cout << "\t音乐教室一间预约成功！" << endl;
			proom[1]--;   flag = 1;
			t = "音乐教室";
		}
		break;
	case 6:
		if (gym[1] > 0)//判断课室是否有剩余
		{
			cout << "\t体育课室一间预约成功！" << endl;
			gym[1]--;   flag = 1;
			t = "体育课室";
		}
		break;
	}
	if (flag == 0)
	{
		cout << "\n\t预约失败！程序退出中···" << endl;//flag用于标志预约是否成功
		Sleep(2000);
	}
	//确定time
	switch (choice2) {
	case 1:
		time = "第1~2节";
		break;
	case 2:
		time = "第3~4节";
		break;
	case 3:
		time = "第5~6节";
		break;
	case 4:
		time = "第7~8节";
		break;
	case 5:
		time = "第9~10节";
		break;
	case 6:
		time = "第11~12节";
		break;
	}
	if (flag == 0)
	{
		cout << "\n\t预约失败！程序退出中···" << endl;//flag用于标志预约是否成功
		Sleep(2000);
	}

	//课室预约需要对预约人的信息进行记录，方便管理员查看和管理
	//这里使用文件操作来完成此功能
	ofstream putf("bookingroom1.dat", ios::binary | ios::out | ios::app);
	if (!putf) {
		cerr << "\n\t预约失败！程序退出中···" << endl;   //这里使用了最后一次理论课学的cerr输出流
		Sleep(2000);
		return;
	}
	putf << "______________________________________________________________  " << "\n北校区  " << name << "  " << t << "  " << time << "\n\n";
	putf.close();//关闭文件

	cout << "\n\t请按任意键退出···" << endl;
	_getch();
}

