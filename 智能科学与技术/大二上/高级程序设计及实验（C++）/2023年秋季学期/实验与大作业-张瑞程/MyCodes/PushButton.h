#pragma once
#include "BasicWidget.h"
using namespace std;
//按键类，继承自基类
class PushButton :
    public BasicWidget
{
private:
    string text; //按键内容
public:
    PushButton(const string &text, int x=0, int y=0, int w = 85, int h = 40);//默认大小
    PushButton(const PushButton& btn);//复制构造函数
    ~PushButton(){}
    void show(int size_x=0, int size_y=0) override;//覆盖虚函数
    bool Click(const ExMessage& msg);//按下操作
};

