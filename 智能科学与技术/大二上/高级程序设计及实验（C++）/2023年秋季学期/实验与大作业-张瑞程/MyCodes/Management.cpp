#include "Management.h"
using namespace std;

//管理类初始化，在此进行各项资料读取
Management::Management()
{
	//读取用户列表
	width = 1848 / 3 * 2;
	height = 862 / 3 * 2;
	load_userList();
	Window GUI(width, height, EX_SHOWCONSOLE||EX_NOCLOSE);//绘制图窗
	//登录
	if (!load()) exit(0);//取消登录，退出程序
	//读取备忘录
	tasks_list = new Task_list;
	tasks_list->read_file(user);
	//读取学习记录
	record = new Record;
	record->read_file(user);
	//读取背景图
	width = 1850 / 3 * 2;
	height = 850 / 3 * 2;
	::loadimage(&background1, "./file/background.jpg", width, height);//依照窗口规格加载图片
	//主界面按钮初始化
	//右方菜单按钮
	menu_btns = new vector<PushButton>;
	menu_btns->push_back(PushButton("正计时", width - 550, 33));//位置的x,y坐标
	menu_btns->push_back(PushButton("倒计时", width - 450, 33));
	menu_btns->push_back(PushButton("今日记录", width - 350, 33));
	menu_btns->push_back(PushButton("退出", width - 250, 33));
	//初始化Task_list打钩窗口  Del_btns备忘录删除键
	Tasks_btns = new vector<click_btn>;
	Study_btns = new vector<PushButton>;
	Del_btns = new vector<Del_btn>;
	Modify_btns = new vector<PushButton>;
	int num = tasks_list->get_num();
	int c_x = 80;
	int y = 90;
	int d_x = 650;
	for (int i = 0; i < num; i++, y += 30)
	{
		Tasks_btns->push_back(click_btn("\0", c_x,y));
		Study_btns->push_back(PushButton("开始学习", d_x-80, y,70,20));
		Del_btns->push_back(Del_btn( d_x, y));
		Modify_btns->push_back(PushButton("学习记录", d_x + 32, y, 70, 20));
	}
	add_btns=new click_btn("+", c_x, y);//添加代办任务的按钮
}

//析构函数
Management::~Management()
{
	delete menu_btns;
	delete tasks_list;
	delete record;
	delete Tasks_btns;
	delete Del_btns;
	delete Study_btns;
	delete Modify_btns;
	delete add_btns;
}

//登录界面
bool Management::load()
{
	IMAGE png;
	::loadimage(&png, "./file/background1.jpg", width, height);
	PushButton P("*开启作业之旅*", width / 2 - 80, height - 350,150,50);

	PushButton esc("*一会再学*", width / 2 - 80, height-280,150,50);
	ExMessage msg;
	//等待点击
	while (1)
	{
		BeginBatchDraw();
		::putimage(0, 0, &png);//绘制背景板
		P.show();//显示按键
		
		esc.show();
		FlushBatchDraw();
		if (peekmessage(&msg, EM_MOUSE))
		{
			if (P.Click(msg))//登录
			{
				//循环 使可以多次尝试登录
				while (1)
				{
					//输入用户名
					char name[10];
					InputBox(name, 10, "请输入用户名：  ");
					//是否存在该用户？
					if (find_user(name))//有用户
					{
						user = name;
						return true;
					}
					else//无此用户
					{
						//提示窗，创建新用户
						int order;
						order = MessageBox(Window::m_handle, "查询无该用户，是->创建该用户 否->重新输入 取消->退出", "提示", MB_YESNOCANCEL);
						if (order == IDYES)
						{
							user = name;
							users_list.push_back(user);
							write_userList();
							//增加该用户，写用户清单
							return true;
						}
						else if (order == IDNO)//重来
						{
							continue;
						}
						else if (order == IDCANCEL)
						{
							break;
						}
					}
				}
			}
			
			else if (esc.Click(msg))
			{
				return false;
			}
		}
	}
}
//读取用户列表
void Management::load_userList()
{
	ifstream ifs; //文件输入流对象
	ifs.open("./file/users_list.txt", ios::in);
	if (!ifs.is_open()) cout << "用户清单打开失败" << endl;
	else {
		cout << "用户清单打开成功!" << endl;
		//读取文件
		while (1)
		{
			string name;
			if (!(ifs >> name))
				break;
			else
				users_list.push_back(name);
		}
	}
}

void Management::write_userList()
{
	ofstream ofs;
	ofs.open("./file/users_list.txt", ios::trunc);//trunc先删除再创建文件。
	for (vector<string>::iterator it = users_list.begin(); it != users_list.end(); it++)
		ofs << (*it) << endl;
	//关闭文件
	ofs.close();
}

bool Management::find_user(const string& user)
{
	if (find(users_list.begin(), users_list.end(), user) != users_list.end())
	{
		return true;
	}
	else return false;
}

void Management::study(int model)
{
	//交互界面：输入学习内容，学习模式，学习时间
	switch(model)
	{
	case 1://正向计时模式
		record->start_up_study();
		break;
	case 2://倒计时模式
		record->start_down_study();
		break; 
	}
}

//启动，进行功能清单选择
bool Management::run()
{
	ExMessage msg;	//鼠标键盘输入信息
	peekmessage(&msg, EM_MOUSE);
	int click = 0;
	while (true)
	{
		//页面一，主界面绘制
		BeginBatchDraw();
		draw_page1();
		//消抖，当上一条鼠标信息是点击时，在这delay一会
		if (msg.message == WM_LBUTTONDOWN)
			Sleep(5 * 50);//0.25s
		peekmessage(&msg, EM_MOUSE);
		//1:正计时 2：倒计时 3:今日状态 
		click = menu_click(msg);
		if (click)
		{
			switch (click)
			{
			case 1://学习计时操作 
				study(click);
				break;
			case 2:
				study(click);
				break;
			case 3:
				record->show_record();
				break;
			case 4:
				cout << "退出" << endl;
				return false;
			}
		}
		//待办操作： 打钩打叉
		click = tasks_click(msg);
		if (click)
		{
			tasks_list->set_status(click);
			continue; //一次只能有一个按键，故continue 提高效率
		}
		click = del_click(msg);
		if (click)
		{
			del_task(click);
			continue;
		}
		click = modify_click(msg);
		if (click)
		{
			tasks_list->modify(click);
		}
		//待办学习键被按下
		click = study_click(msg);
		if (click)
		{
			int order = MessageBox(Window::m_handle, "选择模式：正计时->是  or  倒计时->否", "OK",MB_YESNOCANCEL );
			switch (order)
			{
			case IDCANCEL:
				break;
			case IDYES:
			{
				record->start_up_study(tasks_list->get_task_name(click));
				break;
			}
			case IDNO:
				record->start_down_study(tasks_list->get_task_name(click));
				break;
			}
		}
		FlushBatchDraw();
		if (add_btns->Click(msg))
		{
			tasks_list->add();
			//增加按键
			Tasks_btns->clear();
			Del_btns->clear();
			Study_btns->clear();
			Modify_btns->clear();
			int num = tasks_list->get_num();
			int c_x = 80;
			int y = 90;
			int d_x = 650;
			for (int i = 0; i < num; i++, y += 30)
			{

				Tasks_btns->push_back(click_btn("\0", c_x, y));
				Study_btns->push_back(PushButton("开始学习", d_x - 80, y, 70, 20));
				Del_btns->push_back(Del_btn(d_x, y));
				Modify_btns->push_back(PushButton("学习记录", d_x + 32, y, 70, 20));

			}
			add_btns = new click_btn("+", c_x, y);
		}
	}
	
}

//文件关闭，在此进行新旧文件储存
bool Management::close()
{
	if(!tasks_list->write_file())  
		return false;
	if (!record->write_file())
		return false;

	record->read_file(user);
	tasks_list->read_file(user);
	return true;
}

//在GUI窗口上绘制时间
void Management::draw_time(int time_x, int time_y, int space)  
{
	time_t t; 
	tm tmp; 
	time(&t); 
	localtime_s(&tmp, &t);
	int h = tmp.tm_hour; //时
	int m = tmp.tm_min; //分
	int s = tmp.tm_sec;
	outtextxy(time_x-111, time_y+30, ("完成作业，分秒必争！"));
	if (h < 10) 
		outtextxy(time_x, time_y, ("0" + to_string(h) + "：").c_str());
	else 
		outtextxy(time_x, time_y, (to_string(h) + "：").c_str());
	if (m < 10) 
		outtextxy(time_x + space, time_y, ("0" + to_string(m)).c_str());
	else
		outtextxy(time_x + space, time_y, to_string(m).c_str());
}


//主界面：
void Management::draw_page1()
{
	::putimage(0, 0, &background1);//绘制背景板
	//绘制按键
	disp<vector<PushButton>::iterator>(menu_btns->begin(), menu_btns->end());
	disp<vector<click_btn>::iterator>(Tasks_btns->begin(), Tasks_btns->end());
	disp<vector<PushButton>::iterator>(Study_btns->begin(), Study_btns->end());
	disp<vector<Del_btn>::iterator>(Del_btns->begin(), Del_btns->end());
	disp<vector<PushButton>::iterator>(Modify_btns->begin(), Modify_btns->end());
	//绘制添加键
	add_btns->show();
	//绘制备忘录
	tasks_list->disp();
	//绘制时钟
	settextcolor(WHITE);
	settextstyle(20, 20, "宋体");
	draw_time(563, 480, 60);
}

//主界面的click获取
int Management::menu_click(const ExMessage& msg)
{
	int index = 1; //表示按钮顺序
	for (vector<PushButton>::iterator it = menu_btns->begin(); it != menu_btns->end(); it++, index++)
		if (it->Click(msg)) 
			return index;
	return 0;
}

//任务清单的click获取
int Management::tasks_click(const ExMessage& msg)
{
	int index = 1; //表示按钮顺序
	for (vector<click_btn>::iterator it = Tasks_btns->begin(); it != Tasks_btns->end(); it++, index++)
		if (it->Click(msg)) 
			return index;
	return 0;
}

int Management::study_click(const ExMessage& msg)
{
	int index = 1; //表示按钮顺序
	for (vector<PushButton>::iterator it = Study_btns->begin(); it != Study_btns->end(); it++, index++)
		if (it->Click(msg))
			return index;
	return 0;
}

int Management::del_click(const ExMessage& msg)
{
	int index = 1; //表示按钮顺序
	for (vector<Del_btn>::iterator it = Del_btns->begin(); it != Del_btns->end(); it++, index++)
		if (it->Click(msg))
			return index;
	return 0;
}
int Management::modify_click(const ExMessage& msg)
{
	int index = 1; //表示按钮顺序
	for (vector<PushButton>::iterator it = Modify_btns->begin(); it != Modify_btns->end(); it++, index++)
		if (it->Click(msg))
			return index;
	return 0;
}
//删除待办
void Management::del_task(int index)
{
	//确认提示窗
	if (MessageBox(Window::m_handle, string("确定要删除该待办吗？").c_str(), string("请检查作业是否完成。").c_str(), MB_OKCANCEL) == IDCANCEL) 
		return;
	//重构按键
	Del_btns->pop_back();
	Study_btns->pop_back();
	Tasks_btns->pop_back();
	Modify_btns->pop_back();
	//删除待办里的
	tasks_list->del(index);
	add_btns->move_y(add_btns->get_y() - 30);
	//提示窗
	MessageBox(Window::m_handle, string("你已成功完成该作业").c_str(), string("继续加油！").c_str(), MB_OK);
}

