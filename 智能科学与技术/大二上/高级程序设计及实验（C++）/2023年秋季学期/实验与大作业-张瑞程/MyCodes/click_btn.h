#pragma once
#include "BasicWidget.h"
using namespace std;
class click_btn :
    public BasicWidget
{
private:
    string text; //按键内容
public:
    // 构造函数，初始化按钮的位置、大小和显示的文本，默认大小为 20x20
    click_btn(const string& text="\0", int x = 0, int y = 0, int w = 20, int h = 20);
    click_btn(const click_btn& btn);
    ~click_btn(){}
    void show(int size_x=0, int size_y=0) override;  // 覆盖虚函数，用于显示按钮
    bool Click(const ExMessage& msg);// 检测鼠标点击事件，判断鼠标是否点击了按钮
    void move_y(int y);// 移动按钮的纵坐标
    int get_y()const;// 获取按钮的纵坐标
};


