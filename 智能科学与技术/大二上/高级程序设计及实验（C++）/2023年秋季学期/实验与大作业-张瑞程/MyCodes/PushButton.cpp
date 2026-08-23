#include "PushButton.h"
using namespace std;
PushButton::PushButton(const string& text, int x, int y, int w, int h) :BasicWidget(x, y, w, h),text(text)
{
	return;
}
PushButton::PushButton(const PushButton& btn) : BasicWidget(btn.x, btn.y, btn.w, btn.h)
{
	text = btn.text;
}
//绘制按钮
void PushButton::show(int size_x,int size_y)
{
	settextcolor(BLACK);//设置颜色
	settextstyle(size_x, size_y, "宋体");//设置字体样式
	setfillcolor(RGB(150, 95, 50));//在图里截的
	::fillroundrect(x-6,y,x+w+6,y+h,10,10);//前四个参数：左、上、右、下的边框位置，后两个参数为弧度
	//文字居中显示在按钮中间
	int text_x =x+ (w - textwidth(text.c_str()))/2;
	int text_y = y + (h - textheight(text.c_str())) / 2;
	outtextxy(text_x, text_y, text.c_str());
}
bool PushButton::Click(const ExMessage &msg)
{
	if (msg.x > x && msg.x<x + w && msg.y>y && msg.y < y + h) //鼠标在当前按键上 
	{
		if (msg.message == WM_LBUTTONDOWN)//左键点击
		return true;
	}
	return false;
}
