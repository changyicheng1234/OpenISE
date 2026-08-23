#pragma once
#include "BasicWidget.h"
class Del_btn :
    public BasicWidget
{
private:
    string text; //按键内容
public:
    Del_btn(int x=0, int y=0, int w = 20, int h = 20);//构造函数
    Del_btn(const Del_btn& btn);//复制构造函数
    ~Del_btn(){}
    void show(int size_x=0, int size_y=0) override;//覆盖虚函数
    bool Click(const ExMessage& msg);//按下操作
};

