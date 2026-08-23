#pragma once
#include "study.h"
#include "heads.h"
using namespace std;
//学习记录类
// 考虑只需要记录每日学习日程，不需要删除查找
//故使用链表储存

class Record
{
private:
	list<study>* record;  
	int num;
	//date 日期
	int month;
	int day;
	string path;//存储到文件路径
	int cost_min;
public:
	Record();//构造函数
	Record(const Record& r);//复制构造函数
	~Record();
	bool read_file(const string &user);
	bool write_file();
	void start_up_study();//开启向上计时
	void start_down_study();//开启向下计时
	//面向待办（省去输入框的快速学习）
	void start_up_study(const string& name);//开启向上计时
	void start_down_study(const string&name);//开启向下计时
	void show_record();
	string get_task_name(int i);
	void draw_clock(int time_x, int time_y, int space);
};