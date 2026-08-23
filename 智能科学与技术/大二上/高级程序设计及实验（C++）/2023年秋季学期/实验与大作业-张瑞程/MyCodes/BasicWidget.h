#pragma once
#include "heads.h"
// "BasicWidget.h"
using namespace std;
//空模块，虚基类
//该模块会用到继承、虚函数等功能
class BasicWidget
{
protected: //保护类，子类能用
	int x;
	int y;
	int w;
	int h;
public:
	BasicWidget(int x=0, int y=0, int w=40, int h=30);//坐标
	BasicWidget(const BasicWidget& m);
	virtual ~BasicWidget() {}
	virtual void show(int size_x, int size_y)=0;//纯虚函数
};

