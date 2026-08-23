#include "Task.h"

//构造函数1
task::task(const string &name, int status, const string &tips)
{
	task_name = name;
	m_status = status;
	this->tips = tips;
}

//深度复制构造
task::task(const task& t)
{
	task_name = t.task_name;
	m_status = t.m_status;
	tips = t.tips;
}

//构造函数2
task::task(const string& name,const string&tips)
{
	task_name = name;
	m_status = undo;
	this->tips = tips;
}

void task::disp(int x,int y)const
{
	settextcolor(BLACK);//设置颜色
	settextstyle(0, 0, "隶书");//设置字体样式
	string text = "√";
	if(m_status==done)
		outtextxy(x, y, text.c_str());
	x += 30;
	outtextxy(x, y, task_name.c_str()); //输出作业名称
	x += textwidth(task_name.c_str())+30;
	outtextxy(x, y, tips.c_str());		//输出截至时间
}

int task::set_status()
{
	if (m_status == done)
	{
		m_status = undo;
		return undo;
	}
	else
	{
		m_status = done;
		return done;
	}
}
