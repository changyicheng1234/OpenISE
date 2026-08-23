#pragma once
#include "GUI.h"  //这里一旦引用“management”就会崩
using namespace std;
#define up 1
#define down 2
//单次学习记录类
class study
{
private:
	string name;
	 int start_h;
	int start_m;
	 int end_h;
	int end_m;
	int cost_h;
	int cost_m;
	string text; //学习记录
public:
	study(int model);
	study(int model,const string & name);
	study(const study& s);//复制构造函数
	study(string n, int start_h, int start_m, int end_h, int end_m, int cost_h, int cost_m,string text);
	string get_name() { return name; }
	int get_sh() { return start_h; }
	int get_sm() { return start_m; }
	int get_eh() { return end_h; }
	int get_em() { return end_m; }
	int get_ch() { return cost_h; }
	int get_cm() { return cost_m; }
	string get_text() { return text; }
	void count_up();//正计时
	void count_down();//倒计时
	void count_up(const string &name);//正计时
	void count_down(const string &name);//倒计时
	void disp(int x,int y);//展示今日学习内容
	void draw_clock(int time_x, int time_y, int space);
};

