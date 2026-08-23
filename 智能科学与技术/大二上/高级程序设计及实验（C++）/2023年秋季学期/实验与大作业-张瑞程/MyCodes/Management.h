#pragma once
#include "Task_list.h"
#include "record.h"
#include "PushButton.h"
#include "click_btn.h"
#include"Del_btuns.h"
#include "GUI.h"
/*管理类, 用于管理所有功能
* 该类一次运行只会被调用一次，所以不必复制构造
*/
class Management
{
private:
	Task_list *tasks_list;        //任务清单
	string user;                  //用户名---用于登录
	Record* record;               //用于记录数据于文件中
	vector<string> users_list;    //用户列表，查找用户是否存在
	vector<PushButton>* menu_btns;//菜单界面的按钮参数
	vector<click_btn>* Tasks_btns;//待办的按钮，点击可以对待办打钩
	vector<Del_btn>* Del_btns;    //快捷删除待办内容
	vector<PushButton>* Study_btns;//快捷删除待办内容
	vector<PushButton>* Modify_btns;//修改待办的按钮
	click_btn *add_btns;
	//图窗的尺寸
	int width;
	int height;
public:
	Management();
	~Management(); 
	bool run();
	bool load();//登录
	void load_userList();
	void write_userList();//写用户清单文件
	bool find_user(const string& user);
	void draw_page1();//主界面绘制
	void study(int model);//学习计时界面绘制:1为正计时，2为倒计时
	int menu_click(const ExMessage &msg); //获取菜单按钮点击情况
	int tasks_click(const ExMessage& msg); //获取备忘录点击情况
	int del_click(const ExMessage& msg);//获取删除键点击情况
	int study_click(const ExMessage& msg);
	int modify_click(const ExMessage& msg);
	void del_task(int index);//删除待办项的键
	void draw_time(int time_x, int time_y, int space);
	bool close(); //关闭并写入文件
	IMAGE background1;  //背景图
	template <typename it>
	void disp(it in, it out);
};

//模板函数，用于绘制各种按钮
template<typename it>
inline void Management::disp(it in, it out)
{
		while (in != out)
		{
			in->show();
			in++;
		}
}
