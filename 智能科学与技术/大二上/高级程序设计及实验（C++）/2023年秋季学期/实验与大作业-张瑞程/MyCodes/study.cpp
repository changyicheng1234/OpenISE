#include "study.h"
#include "PushButton.h"
//以下为study类的多种构造函数，在不同的传入参数时，会调用不同的构造函数
study::study(string n, int sh, int sm, int eh, int em, int ch, int cm, string text)
{//记录学习时间信息的数据成员
	name = n;
	start_h = sh;
	end_h = eh;
	end_m = em;
	cost_h = ch;
	cost_m = cm;
	start_m = em;
	this->text = text;
}

study::study(int model)
{
	start_h = start_m  = end_h = end_m =  cost_h = cost_m = 0;
	switch (model)
	{
	case up:
		count_up();
		break;
	case down:
		count_down();
		break;
	}
}

study::study(int model, const string& name)
{
	start_h = start_m = end_h = end_m = cost_h = cost_m = 0;
	switch (model)
	{
	case up:
		count_up(name);
		break;
	case down:
		count_down(name);
		break;
	}
}

study::study(const study& s)
{
	name = s.name;
	start_h = s.start_h;
	start_m = s.start_m;
	end_h = s.end_h;
	end_m = s.end_m;
	cost_h = s.cost_h;
	cost_m = s.cost_m;
	text = s.text;
}

//正计时模式
void study::count_up()
{
	IMAGE background;
	::loadimage(&background, "./file/background.jpg", 1848 / 3 * 2, 862 / 3 * 2);//依照窗口规格加载图片
	//获取学习目标
	char txt[50];
	InputBox(txt, 50, "学习目标:");
	name = txt;
	MessageBox(Window::m_handle, string("开始计时！").c_str(), string("提示").c_str(), MB_OK);
	//获取开始时间
	time_t t;
	tm tmp;
	std::time(&t);
	localtime_s(&tmp, &t);
	start_h = tmp.tm_hour;//时
	start_m = tmp.tm_min;//分

	//中间时间
	time_t t2=t;
	int x = 80;
	int y = 100;
	int space = 30;
	int dist = 0;
	int start_x = x + 190;//输出时间的初始坐标
	ExMessage msg;	//鼠标键盘输入信息
	//按钮绘制
	PushButton end("结束", start_x + 150, y + 150);
	PushButton Pause("暂停", start_x - 150, y + 150);
	PushButton Continue("继续", start_x - 150, y + 150);//绘制继续按钮
	PushButton Esc("退出", start_x - 150, y + 150);//绘制继续按钮
	while (1)
	{
		//绘制计时窗口
		BeginBatchDraw();
		::putimage(0, 0, &background);//绘制背景板
		settextcolor(BLACK);//设置颜色
		settextstyle(0, 0, "宋体");//设置字体样式
		outtextxy(x, y - 50, (string("本次学习目标: ") + name).c_str());
		outtextxy(x, y, string("计时中，你已经学习: ").c_str());
		//求解已经学习的时长
		t = t2;//定义t为上一次循环的时间
		time(&t2);//获取当次循环时间
		if (t2 - t != 0) dist++; //定义两次时间不一致，则秒数相加（为了实现暂停时计数停止,采用累加形式）
		int h = dist / 3600;
		int m = (dist - 3600 * h) / 60;
		int s = dist % 60;
		if (h < 10)
			outtextxy(start_x, y + 50, ("0" + to_string(h) + ":").c_str());
		else
			outtextxy(start_x, y + 50, (to_string(h) + ":").c_str());
		if (m < 10)
			outtextxy(start_x + space, y + 50, ("0" + to_string(m) + ":").c_str());
		else
			outtextxy(start_x + space, y + 50, (to_string(m) + ":").c_str());
		if (s < 10)
			outtextxy(start_x + 2 * space, y + 50, ("0" + to_string(s)).c_str());
		else
			outtextxy(start_x + 2 * space, y + 50, to_string(s).c_str());
		//绘制暂暂停按钮与结束按钮
		Pause.show();
		end.show();
		//下方小时钟绘制
		settextcolor(WHITE);
		settextstyle(20, 20, "宋体");
		draw_clock(563, 480, 60);
		FlushBatchDraw();
		//检测是否结束或暂停
		//暂停时需要把时间计数也暂停
		if (peekmessage(&msg, EM_MOUSE))
		{
			if (Pause.Click(msg))//暂停键被按下
			{
				Continue.show();
				FlushBatchDraw();
				//消抖
				Sleep(5 * 200);//1s
				while (1)
				{
					peekmessage(&msg, EM_MOUSE);
					if (Continue.Click(msg))
						break;
					if (end.Click(msg))
						break;
				}
			}
			if (end.Click(msg))//结束键被按下
			{
				break;
			}
		}
	}
	//结束计时
	time(&t2);
	localtime_s(&tmp, &t);
	end_h = tmp.tm_hour;//时
	end_m = tmp.tm_min;//分
	//计算学习时间时间
	cost_h = dist / 3600;
	cost_m = (dist - 3600 * cost_h) / 60;

	//输出结束界面
	
	InputBox(txt, 50, "请填写本次学习感受");
	text = txt;
	
	//退出界面
	while (1)
	{
		BeginBatchDraw();
		settextcolor(BLACK);//设置颜色
		settextstyle(0, 0, "宋体");//设置字体样式
		::putimage(0, 0, &background);//绘制背景板
		outtextxy(x, y - 50, (string("本次学习内容:") + name).c_str());
		outtextxy(x, y, string("学习结束，本次学习时长:").c_str());
		outtextxy(start_x, y + 50, (to_string(cost_h) + "小时" + to_string(cost_m) + "分钟").c_str());
		//绘制时钟
		settextcolor(WHITE);
		settextstyle(20, 20, "宋体");
		draw_clock(563, 480, 60);
		Esc.show();
		settextcolor(BLACK);//设置颜色
		settextstyle(0, 0, "宋体");//设置字体样式
		outtextxy(x, y + 100, ("学习感受： " + text).c_str());
		FlushBatchDraw();
		peekmessage(&msg, EM_MOUSE);
		if (Esc.Click(msg))
			break;
	}
}

//倒计时模式
void study::count_down()
{
	IMAGE background;
	::loadimage(&background, "./file/background.jpg", 1848 / 3 * 2, 862 / 3 * 2);//依照窗口规格加载图片
	//获取学习目标
	char txt[50];
	char get_time[5];
	InputBox(txt, 50, "学习目标:");
	InputBox(get_time, 5, "计划学习时长(分钟):");
	name = txt;
	int aim_sec = atoi(get_time)*60;//获取计划秒数
	MessageBox(Window::m_handle, string("开始计时！").c_str(), string("提示").c_str(), MB_OK);
	//获取开始时间
	time_t t;
	tm tmp;
	std::time(&t);
	localtime_s(&tmp, &t);
	start_h = tmp.tm_hour;//时
	start_m = tmp.tm_min;//分

	//中间时间
	time_t t2 = t;
	int x = 80;
	int y = 100;
	int space = 30;
	int dist = 0;
	int start_x = x + 190;//输出时间的初始坐标
	ExMessage msg;	//鼠标键盘输入信息
	//一些按钮绘制
	PushButton end("提前停止", start_x + 150, y + 150);
	PushButton Pause("暂停", start_x - 150, y + 150);
	PushButton Continue("继续", start_x - 150, y + 150);//绘制继续按钮
	PushButton Esc("退出", start_x - 150, y + 150);//绘制继续按钮
	while (dist<aim_sec)
	{
		//绘制计时窗口
		BeginBatchDraw();
		::putimage(0, 0, &background);//绘制背景板
		settextcolor(BLACK);//设置颜色
		settextstyle(0, 0, "宋体");//设置字体样式
		outtextxy(x, y - 50, (string("本次学习内容:") + name).c_str());
		outtextxy(x, y, string("计时中，距离目标，还有:").c_str());
		//求解剩余时长
		t = t2;//定义t为上一次循环的时间
		time(&t2);//获取当次循环时间
		if (t2 - t != 0) dist++; //定义两次时间不一致，则秒数++（为了实现暂停时计数停止,采用累加形式）
		int h = (aim_sec-dist) / 3600;
		int m = ((aim_sec-dist) - 3600 * h) / 60;
		int s = (aim_sec-dist) % 60;
		//输出时间
		if (h < 10)
			outtextxy(start_x, y + 50, ("0" + to_string(h) + ":").c_str());
		else
			outtextxy(start_x, y + 50, (to_string(h) + ":").c_str());
		if (m < 10)
			outtextxy(start_x + space, y + 50, ("0" + to_string(m) + ":").c_str());
		else
			outtextxy(start_x + space, y + 50, (to_string(m) + ":").c_str());
		if (s < 10)
			outtextxy(start_x + 2 * space, y + 50, ("0" + to_string(s)).c_str());
		else
			outtextxy(start_x + 2 * space, y + 50, to_string(s).c_str());
		//绘制暂暂停按钮与结束按钮
		Pause.show();
		end.show();
		//下方小时钟绘制
		settextcolor(WHITE);
		settextstyle(20, 20, "宋体");
		draw_clock(563, 480, 60);
		FlushBatchDraw();
		//检测是否结束或暂停
		//暂停时需要把时间计数也暂停
		if (peekmessage(&msg, EM_MOUSE))
		{
			if (Pause.Click(msg))//暂停键被按下
			{
				Continue.show();
				FlushBatchDraw();
				//消抖
				Sleep(5 * 200);//1s
				while (1)
				{
					peekmessage(&msg, EM_MOUSE);
					if (Continue.Click(msg))
						break;
					if (end.Click(msg))
						break;
				}
			}
			if (end.Click(msg))//结束键被按下
			{
				break;
			}
		}
	}
	//结束计时
	time(&t2);
	localtime_s(&tmp, &t);
	end_h = tmp.tm_hour;//时
	end_m = tmp.tm_min;//分
	//计算学习时间时间
	cost_h = dist / 3600;
	cost_m = (dist - 3600 * cost_h) / 60;
	InputBox(txt, 50, "请填写本次学习感受");
	text = txt;
	//等待退出
	while (1)
	{
		BeginBatchDraw();
		settextcolor(BLACK);//设置颜色
		settextstyle(0, 0, "宋体");//设置字体样式
		::putimage(0, 0, &background);//绘制背景板
		outtextxy(x, y - 50, (string("本次学习目标: ") + name).c_str());
		outtextxy(x, y, string("学习结束，本次学习时长: ").c_str());
		outtextxy(start_x, y + 50, (to_string(cost_h) + "小时" + to_string(cost_m) + "分钟").c_str());
		//绘制时钟
		settextcolor(WHITE);
		settextstyle(20, 20, "宋体");
		draw_clock(563, 480, 60);
		outtextxy(x, y + 100, ("学习感受： " + text).c_str());
		Esc.show();
		FlushBatchDraw();
		peekmessage(&msg, EM_MOUSE);
		if (Esc.Click(msg))
			break;
	}
}

void study::count_up(const string& name)
{
	IMAGE background;
	::loadimage(&background, "./file/background.jpg", 1848 / 3 * 2, 862 / 3 * 2);//依照窗口规格加载图片
	//获取学习目标
	this->name = name;
	MessageBox(Window::m_handle, string("开始计时！").c_str(), string("提示").c_str(), MB_OK);
	//获取开始时间
	time_t t;
	tm tmp;
	std::time(&t);
	localtime_s(&tmp, &t);
	start_h = tmp.tm_hour;//时
	start_m = tmp.tm_min;//分

	//中间时间
	time_t t2 = t;
	int x = 80;
	int y = 200;
	int space = 30;
	int dist = 0;
	int start_x = x + 190;//输出时间的初始坐标
	ExMessage msg;	//鼠标键盘输入信息
	//一些按钮绘制
	PushButton end("结束", start_x + 150, y + 150);
	PushButton Pause("暂停", start_x - 150, y + 150);
	PushButton Continue("继续", start_x - 150, y + 150);//绘制继续按钮
	PushButton Esc("退出", start_x - 150, y + 150);//绘制继续按钮
	while (1)
	{
		//绘制计时窗口
		BeginBatchDraw();
		::putimage(0, 0, &background);//绘制背景板
		settextcolor(BLACK);//设置颜色
		settextstyle(0, 0, "宋体");//设置字体样式
		outtextxy(x, y - 50, (string("本次学习目标: ") + name).c_str());
		outtextxy(x, y, string("计时中，你已经学习: ").c_str());
		//求解已经学习的时长
		t = t2;//定义t为上一次循环的时间
		time(&t2);//获取当次循环时间
		if (t2 - t != 0) dist++; //定义两次时间不一致，则秒数++（为了实现暂停时计数停止,采用累加形式）
		int h = dist / 3600;
		int m = (dist - 3600 * h) / 60;
		int s = dist % 60;
		if (h < 10)
			outtextxy(start_x, y + 50, ("0" + to_string(h) + ":").c_str());
		else
			outtextxy(start_x, y + 50, (to_string(h) + ":").c_str());
		if (m < 10)
			outtextxy(start_x + space, y + 50, ("0" + to_string(m) + ":").c_str());
		else
			outtextxy(start_x + space, y + 50, (to_string(m) + ":").c_str());
		if (s < 10)
			outtextxy(start_x + 2 * space, y + 50, ("0" + to_string(s)).c_str());
		else
			outtextxy(start_x + 2 * space, y + 50, to_string(s).c_str());
		//绘制暂暂停按钮与结束按钮
		Pause.show();
		end.show();
		//下方小时钟绘制
		settextcolor(WHITE);
		settextstyle(20, 20, "宋体");
		draw_clock(563, 480, 60);
		FlushBatchDraw();
		//检测是否结束或暂停
		//暂停时需要把时间计数也暂停
		if (peekmessage(&msg, EM_MOUSE))
		{
			if (Pause.Click(msg))//暂停键被按下
			{
				Continue.show();
				FlushBatchDraw();
				//消抖
				Sleep(5 * 200);//1s
				while (1)
				{
					peekmessage(&msg, EM_MOUSE);
					if (Continue.Click(msg))
						break;
					if (end.Click(msg))
						break;
				}
			}
			if (end.Click(msg))//结束键被按下
			{
				break;
			}
		}
	}
	//结束计时
	time(&t2);
	localtime_s(&tmp, &t);
	end_h = tmp.tm_hour;//时
	end_m = tmp.tm_min;//分
	//计算学习时间时间
	cost_h = dist / 3600;
	cost_m = (dist - 3600 * cost_h) / 60;

	char txt[50];
	InputBox(txt, 50, "请填写本次学习感受");
	text = txt;
	//退出界面
	while (1)
	{
		BeginBatchDraw();
		settextcolor(BLACK);//设置颜色
		settextstyle(0, 0, "宋体");//设置字体样式
		::putimage(0, 0, &background);//绘制背景板
		outtextxy(x, y - 50, (string("本次学习内容:") + name).c_str());
		outtextxy(x, y, string("学习结束，本次学习时长:").c_str());
		outtextxy(start_x, y + 50, (to_string(cost_h) + "小时" + to_string(cost_m) + "分钟").c_str());
		//绘制时钟
		settextcolor(WHITE);
		settextstyle(20, 20, "宋体");
		draw_clock(563, 480, 60);
		Esc.show();
		outtextxy(x, y + 100, ("学习感受： " + text).c_str());
		FlushBatchDraw();
		peekmessage(&msg, EM_MOUSE);
		if (Esc.Click(msg))
			break;
	}
}

void study::count_down(const string& name)
{
	IMAGE background;
	::loadimage(&background, "./file/background.jpg", 1848 / 3 * 2, 862 / 3 * 2);//依照窗口规格加载图片
	//获取学习目标
	char get_time[5];
	InputBox(get_time, 5, "计划学习时长(分钟):");
	this->name = name;
	int aim_sec = atoi(get_time) * 60;//获取计划秒数
	MessageBox(Window::m_handle, string("开始计时！").c_str(), string("提示").c_str(), MB_OK);
	//获取开始时间
	time_t t;
	tm tmp;
	std::time(&t);
	localtime_s(&tmp, &t);
	start_h = tmp.tm_hour;//时
	start_m = tmp.tm_min;//分

	//中间时间
	time_t t2 = t;
	int x = 80;
	int y = 100;
	int space = 30;
	int dist = 0;
	int start_x = x + 190;//输出时间的初始坐标
	ExMessage msg;	//鼠标键盘输入信息
	//一些按钮绘制
	PushButton end("提前停止", start_x + 150, y + 150);
	PushButton Pause("暂停", start_x - 150, y + 150);
	PushButton Continue("继续", start_x - 150, y + 150);//绘制继续按钮
	PushButton Esc("退出", start_x - 150, y + 150);//绘制继续按钮
	while (dist < aim_sec)
	{
		//绘制计时窗口
		BeginBatchDraw();
		::putimage(0, 0, &background);//绘制背景板
		settextcolor(BLACK);//设置颜色
		settextstyle(0, 0, "宋体");//设置字体样式
		outtextxy(x, y - 50, (string("本次学习目标: ") + name).c_str());
		outtextxy(x, y, string("计时中，距离目标，还有: ").c_str());
		//求解剩余时长
		t = t2;//定义t为上一次循环的时间
		time(&t2);//获取当次循环时间
		if (t2 - t != 0) dist++; //定义两次时间不一致，则秒数++（为了实现暂停时计数停止,采用累加形式）
		int h = (aim_sec - dist) / 3600;
		int m = ((aim_sec - dist) - 3600 * h) / 60;
		int s = (aim_sec - dist) % 60;
		//输出时间
		if (h < 10)
			outtextxy(start_x, y + 50, ("0" + to_string(h) + ":").c_str());
		else
			outtextxy(start_x, y + 50, (to_string(h) + ":").c_str());
		if (m < 10)
			outtextxy(start_x + space, y + 50, ("0" + to_string(m) + ":").c_str());
		else
			outtextxy(start_x + space, y + 50, (to_string(m) + ":").c_str());
		if (s < 10)
			outtextxy(start_x + 2 * space, y + 50, ("0" + to_string(s)).c_str());
		else
			outtextxy(start_x + 2 * space, y + 50, to_string(s).c_str());
		//绘制暂暂停按钮与结束按钮
		Pause.show();
		end.show();
		//下方小时钟绘制
		settextcolor(WHITE);
		settextstyle(20, 20, "宋体");
		draw_clock(563, 480, 60);
		FlushBatchDraw();
		//检测是否结束或暂停
		//暂停时需要把时间计数也暂停
		if (peekmessage(&msg, EM_MOUSE))
		{
			if (Pause.Click(msg))//暂停键被按下
			{
				Continue.show();
				FlushBatchDraw();
				//消抖
				Sleep(5 * 200);//1s
				while (1)
				{
					peekmessage(&msg, EM_MOUSE);
					if (Continue.Click(msg))
						break;
					if (end.Click(msg))
						break;
				}
			}
			if (end.Click(msg))//结束键被按下
			{
				break;
			}
		}
	}
	//结束计时
	time(&t2);
	localtime_s(&tmp, &t);
	end_h = tmp.tm_hour;//时
	end_m = tmp.tm_min;//分
	//计算学习时间时间
	cost_h = dist / 3600;
	cost_m = (dist - 3600 * cost_h) / 60;
	char txt[50];
	InputBox(txt, 50, "请填写本次学习感受");
	text = txt;
	//等待退出
	while (1)
	{
		BeginBatchDraw();
		settextcolor(BLACK);//设置颜色
		settextstyle(0, 0, "宋体");//设置字体样式
		::putimage(0, 0, &background);//绘制背景板
		outtextxy(x, y - 50, (string("本次学习目标: ") + name).c_str());
		outtextxy(x, y, string("学习结束，本次学习时长: ").c_str());
		outtextxy(start_x, y + 50, (to_string(cost_h) + "小时" + to_string(cost_m) + "分钟").c_str());
		//绘制时钟
		settextcolor(WHITE);
		settextstyle(20, 20, "宋体");
		draw_clock(563, 480, 60);
		outtextxy(x, y + 100, ("学习感受： " + text).c_str());
		Esc.show();
		FlushBatchDraw();
		peekmessage(&msg, EM_MOUSE);
		if (Esc.Click(msg))
			break;
	}
}

void study::disp(int x,int y)
{
	x = x + 10;
	outtextxy(x, y+30, ("记录："+text).c_str());
	settextcolor(BLACK);//设置颜色
	settextstyle(0, 0, "宋体");//设置字体样式
	outtextxy(x, y, (name+":").c_str());
	x = x + textwidth((name + ":").c_str()) + 5;
	if (start_h < 10) 
		outtextxy(x , y, ("0" + to_string(start_h)+":").c_str());
	else 
		outtextxy(x, y, (to_string(start_h)+":").c_str());
	x = x + textwidth(("0" + to_string(start_h) + ":").c_str()) + 2;
	if (start_m < 10) 
		outtextxy(x , y, ("0" + to_string(start_m) + "-").c_str());
	else
		outtextxy(x, y, (to_string(start_m) + "-").c_str());

	x = x + textwidth(("0" + (to_string(start_m) + "-")).c_str()) + 2;
	if (end_h < 10) 
		outtextxy(x, y, ("0" + to_string(end_h) + ":").c_str());
	else 
		outtextxy(x, y, (to_string(end_h) + ":").c_str());
	x = x + textwidth(("0" + to_string(end_h) + ":").c_str()) + 2;
	if (end_m < 10)
		outtextxy(x, y, ("0" + to_string(end_m)).c_str());
	else 
		outtextxy(x, y, to_string(end_m).c_str());
}

//时钟绘制函数
void study::draw_clock(int time_x, int time_y, int space)
{
	time_t t;
	tm tmp;
	time(&t);
	localtime_s(&tmp, &t);
	int h = tmp.tm_hour;
	int m = tmp.tm_min;
	int s = tmp.tm_sec;
	outtextxy(time_x - 111, time_y + 30, ("完成作业，分秒必争！"));
	if (h < 10)
		outtextxy(time_x, time_y, ("0" + to_string(h) + ":").c_str());
	else
		outtextxy(time_x, time_y, (to_string(h) + ":").c_str());
	if (m < 10)
		outtextxy(time_x + space, time_y, ("0" + to_string(m)).c_str());
	else
		outtextxy(time_x + space, time_y, to_string(m).c_str());
}
