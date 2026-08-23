#pragma once
using namespace std;
#include "heads.h"
//GUI界面
class Window
{
	friend class study;
	friend class Task_list;
	friend class Management;
public:
	Window(int w, int h, int flag);
	static void beginDraw(); //全称 beginbatch_draw 批量绘图
	static void flushDraw();
	//设置按键和鼠标操作识别函数  设置为内联函数减小开销
	//判断有没有按键和操作
	inline static bool hasMsg();//返回值！=0时有消息
	inline  static const ExMessage& getMsg();//get message(); 
private:
	static int width;
	static int height;
	static HWND m_handle; //窗口的句柄,HWND是其类
	static ExMessage m_msg; //获取消息 ，要求为静态成员,需要定义
	string title;//图窗标题
};


