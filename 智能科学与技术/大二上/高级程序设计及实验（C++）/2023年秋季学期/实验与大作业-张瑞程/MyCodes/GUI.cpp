#include "GUI.h"

//创建窗口
ExMessage Window::m_msg; //定义静态成员
int Window::width;
int Window::height;
HWND Window::m_handle;
//窗体初始化
Window::Window(int w, int h, int f)
{
	//width = 1848;
	//height = 862;
	width = w;
	height = h;
	m_handle = ::initgraph(width, height, f);
	title = "中山大学作业系统";
	SetWindowText(m_handle, title.c_str());//写标题
	::setbkmode(TRANSPARENT);//设置该图窗字体背景为透明颜色
}

void Window::beginDraw()
{
	::BeginBatchDraw();
}

void Window::flushDraw()
{
	FlushBatchDraw();
}

inline bool Window::hasMsg()
{
	 return ::peekmessage(&m_msg, EM_MOUSE); 
}

inline const ExMessage& Window::getMsg()
{
	return m_msg; 
}
