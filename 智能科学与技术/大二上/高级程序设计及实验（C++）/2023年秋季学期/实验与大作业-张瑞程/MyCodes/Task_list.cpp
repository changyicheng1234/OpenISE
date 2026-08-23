#include "Task_list.h"
#include "GUI.h"
using namespace std;

#define undo 0
#define done 1
Task_list::Task_list()
{
	tasks_num = finished_num = unfinished_num = 0;
	tasks = new vector<task>; 
}
Task_list::Task_list(const Task_list& t)
{
	tasks_num = t.tasks_num;
	finished_num = t.finished_num;
	unfinished_num = t.unfinished_num;
	path = t.path;
	//深复制vector
	tasks = new vector<task>;
	for (vector<task>::iterator it = t.tasks->begin(); it != t.tasks->end(); it++)
	{
		tasks->push_back(task(*it));
	}
}
//读取列表
bool Task_list::read_file(const string &user)
{
	//读文件
	/*规定文件格式:
	file_name: user/Task_list.txt
	第一行:tasks_num
	后每一行代表一个数据
	string task_name; //
	int status;  //状态
	string tips;
	*/
	path = "./file/" + user + "/Task_list.txt";
	ifstream ifs; //文件输入流对象
	ifs.open(path, ios::in);
	if (!ifs.is_open()) {
		cout << "用户待办打开失败" << endl;
		string dir = "./file/" + user;
		if (CreateDirectory(dir.c_str(), NULL)) 
			cout << "用户资料文件夹创建成功！" << endl;
		else 
			cout << "用户资料文件夹创建失败" << endl;
	}
	else {
		cout << "用户待办打开成功!" << endl;
		//读取文件
		ifs >> tasks_num;
		for (int i = 0; i < tasks_num; i++)
		{	
			string name;
			int status;
			char tips[50];
			ifs >> name;
			ifs >> status;
			ifs.getline(tips, sizeof(tips));
			//写入task中
			task new_task(name, status,tips);
			tasks->push_back(new_task);
			//记录数目
			if (status == 0) unfinished_num++;
			else if (status == 1) finished_num++;
		}
		
		return true;
	}
	return false;
}

//将任务清单写入文件
bool Task_list::write_file()
{
	/*规定文件格式:
	file_name: Task_list.txt
	第一行:tasks_num
	后每一行代表一个数据
	string task_name; //
	int status;  //状态
	*/
	ofstream ofs;
	ofs.open(path, ios::trunc);//先删除再创建文件。
	ofs << tasks_num << endl;
	for (vector<task>::iterator it = tasks->begin(); it != tasks->end(); it++)
	{
		ofs << it->get_name() << "\t";
		ofs << it->get_status() << "\t";
		ofs	<< it->get_tips() <<endl;
	}
	//关闭文件
	ofs.close();
	return true;
}

string Task_list::get_status(int index)
{
	if ((*tasks)[index].get_status() == 0)
	{
		return "\0";
	}
	else
		return "√";
}

string Task_list::get_task_name(int index)
{
	vector<task>::iterator it = tasks->begin();
	int cout = 1;
	for (cout = 1; cout < index; cout++)
		it++;
	return it->get_name();
}

//添加任务
void Task_list::add()
{
	if (tasks_num > 8)
	{
		MessageBox(Window::m_handle, "待办作业太多了！先完成几个吧！", "太卷了，别累着了", MB_OK);
		return;
	}
	char name[30];
	InputBox(name, 30, "待办名称:");
	char tips[50];
	InputBox(tips, 50, "截至日期:");
	task new_T(name,"截止日期:"+string(tips));
	tasks->push_back(new_T);
	tasks_num++;
	unfinished_num++;
}


//转置某项待办的状态
void Task_list::set_status(int index)
{
	vector<task>::iterator it = tasks->begin();
	for (int i = 1; i < index; i++) it++;
	if (it->set_status() == done)
	{
		finished_num++;
		unfinished_num--;
	}
	else {
		finished_num--;
		unfinished_num++;
	}
}

//删除任务
void Task_list::del(int index)
{
	//删除按键
	vector<task>::iterator it1; //迭代器，用于查找task
	int num = 1;
	for (it1 = tasks->begin(); num < index; it1++)
		num++;
	if (it1->get_status() == done) finished_num--;
	else unfinished_num--;
	//删除
	tasks->erase(it1);
	tasks_num--;
}

//修改任务
void Task_list::modify(int index)
{
	vector<task>::iterator it = tasks->begin(); //迭代器，用于查找
	for (int i = 1; i < index; i++, it++);
	char new_tips[30];
	InputBox(new_tips, 30, "tips:");
	it->change_tips("备注:"+string(new_tips));
	MessageBox(Window::m_handle, "修改成功！", "恭喜你！", MB_OK);
}

//任务清单展示
/*
	outtextxy(text_x, text_y, text.c_str());
	settextcolor(BLACK);//设置颜色
	settextstyle(0,0, "隶书");//设置字体样式
*/
void Task_list::disp()
{
	int text_x = 80;
	int text_y = 60;
	vector<task>::iterator it = tasks->begin();
	string s;
	if (tasks->begin() == tasks->end()) 
	{ 
		s = "当前任务栏为空，快来添加任务吧！";
		outtextxy(text_x, text_y, s.c_str());
		return; 
	}
	else 
	{
		settextcolor(BLACK);//设置颜色
		settextstyle(0, 0, "宋体");//设置字体样式
		int space = 80;
		s = "总任务数:";	
		outtextxy(text_x, text_y,s.c_str());		
		outtextxy(text_x+space+20, text_y, to_string(tasks_num).c_str());
		s = "已完成:";     
		outtextxy(text_x+200, text_y, s.c_str());	
		outtextxy(text_x+200+space, text_y, to_string(finished_num).c_str());
		s = "未完成:";	
		outtextxy(text_x+400, text_y, s.c_str());
		outtextxy(text_x+400+space, text_y, to_string(unfinished_num).c_str()); 
		for (it; it != tasks->end(); it++)
		{
			text_y += 30;
			it->disp(text_x,text_y);
		}
	}
}

