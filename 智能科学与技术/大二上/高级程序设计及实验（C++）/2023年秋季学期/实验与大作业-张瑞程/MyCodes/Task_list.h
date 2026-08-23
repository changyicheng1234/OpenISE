#pragma once
#include "Task.h"
using namespace std;
//任务清单类，允许操作有：增加任务，查找任务，删除任务，修改任务。搞个文件操作，读入文件，输出文件
class Task_list
{
private:
	vector<task> *tasks;  //任务清单，使用vector储存,在文件中  
	int tasks_num;
	int finished_num; //完成任务数
	int unfinished_num; //未完成任务数
	string path;
public:
	Task_list();
	Task_list(const Task_list& t);
	~Task_list() { delete tasks; }
	bool read_file(const string& user);
	void add();
	void del(int index);
	void modify(int index); // 修改待办
	void disp();//打印所有待办
	void set_status(int index);//转置待办的状态
	bool write_file(); //写出文件
	int get_num() { return tasks_num; }//获取任务数
	string get_status(int index); //获取第index件任务状态
	string get_task_name(int index);
};



