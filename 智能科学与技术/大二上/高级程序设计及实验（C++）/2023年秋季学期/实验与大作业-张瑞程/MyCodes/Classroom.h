#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Classroom
{
public:
	string address;//课室位置
	int type;//课室种类：小型教室1，中型教室2，大型教室3，会议室4，琴房5，体育馆6
	int size;//教室容量
	int state;//教室状态：0空闲，1占用
};

