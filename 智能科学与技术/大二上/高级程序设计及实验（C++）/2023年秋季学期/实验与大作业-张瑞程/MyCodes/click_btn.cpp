#include "click_btn.h"
using namespace std;
click_btn::click_btn(const string& text, int x, int y, int w, int h) :BasicWidget(x, y, w, h), text(text)
{
	return;
}
click_btn::click_btn(const click_btn& btn):BasicWidget(btn.x,btn.y,btn.w,btn.h)
{
	text = btn.text;
}
//绘制按钮
void click_btn::show(int size_x, int size_y)
{
	settextcolor(BLACK);//设置颜色
	setfillcolor(RGB(78, 121, 111));//在图里截的
	::fillroundrect(x, y, x + w, y + h, 0, 0);//后两个参数为弧度
	//文字居中显示在按钮中间
	int text_x = x + (w - textwidth(text.c_str())) / 2;
	int text_y = y + (h - textheight(text.c_str())) / 2;
	outtextxy(text_x, text_y, text.c_str());
}

bool click_btn::Click(const ExMessage& msg)
{
	if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) //鼠标在当前按键上 
	{
		if (msg.message == WM_LBUTTONDOWN)//左键点击
			return true;
	}
	return false;
}

void click_btn::move_y(int y)
{
	this->y = y; 
}

int click_btn::get_y() const
{
	return y; 
}
