#include "record.h"
#include "PushButton.h"
using namespace std;
Record::Record()
{
	record = new list<study>;
	num = 0;
	time_t t;tm tmp;
	time(&t);localtime_s(&tmp, &t);
	month = tmp.tm_mon+1;day = tmp.tm_mday;
}
Record::Record(const Record& r):num(r.num),month(r.month),day(r.day),path(r.path),cost_min(r.cost_min)
{
	record = new list<study>;
	//调用算法实现深复制record
	copy(r.record->begin(), r.record->end(), record->begin());
}
Record::~Record()
{
	delete record; //数据成员是指针，指向list,直接删除list就好
}
//读入文件
bool Record::read_file(const string &user)
{

	path = "./file/" + user + "/ Record_" + to_string(month) + "_" + to_string(day) + ".txt";   //目标文件路径
	/**
		搜寻文件库中是否存在当前日期的日志文件，如果有，则用它，无，则重新创建
		文件内容:第一行储存当日学习次数
		每1行依次存储study的内容
		string name;
		int  start_h;
		int start_m;
		int end_h;
		int end_m;
		int cost_h;
		int cost_m;
		string text
	**/
	ifstream ifs; //文件输入流对象
	ifs.open(path, ios::in);
	if (!ifs.is_open()) {
		cout << "用户资料打开失败" << endl;
	}
	else {
		cout << "用户资料文件打开成功!" << endl;
		//读取文件
		ifs >> num;
		for (int i = 0; i < num; i++)
		{
			string name;
			char text[50];
			int start_h, start_m, end_h, end_m, cost_h, cost_m;
			ifs >> name>>start_h >> start_m >> end_h >> end_m >> cost_h >> cost_m;
			ifs.getline(text,sizeof(text));//只读取第一行——剩下的为截至时间
			//去掉一个空格
			int x =0;
			int p = 0;
			while (text[x] != '\0')
			{
				if (text[x] != ' ' && text[x] != '\t')
					text[p++] = text[x];
				x++;
			}
			text[p] = '\0';
			//写入task中
			study new_study(name, start_h , start_m , end_h , end_m , cost_h , cost_m,string(text));
			record->push_back(new_study); //这里使用了深度复制！！！
			cost_min += (cost_h * 60 + cost_m);
		}
		return true;
	}
	return false;
}
//写出文件
bool Record::write_file()
{
	/**
		搜寻文件库中是否存在当前日期的日志文件，如果有，则用它，无，则重新创建
		文件内容:第一行储存当日学习次数
		每1行依次存储study的内容
		string name;
		int  start_h;
		int start_m;
		int end_h;
		int end_m;
		int cost_h;
		int cost_m;
	**/
	ofstream ofs;
	ofs.open(path, ios::trunc);//trunc先删除再创建文件。
	ofs << num << endl;
	for (list<study>::iterator it = record->begin(); it != record->end(); it++)
	{
		ofs << it->get_name() << " ";
		ofs << it->get_sh() << " ";
		ofs << it->get_sm() << " ";
		ofs << it->get_eh() << " ";
		ofs << it->get_em() << " ";
		ofs << it->get_ch() << " ";
		ofs << it->get_cm() << " ";
		ofs << it->get_text() << endl;
	}
	//关闭文件
	ofs.close();
	return true;
}
//向上计时
void Record::start_up_study()
{
	study *new_study = new study(up);
	num++;
	record->push_back(*new_study);
	cost_min += (new_study->get_ch()* 60 + new_study->get_cm());
	delete new_study;
}
//倒计时
void Record::start_down_study()
{
	study* new_study = new study(down);
	num++;
	record->push_back(*new_study);
	cost_min += (new_study->get_ch() * 60 + new_study->get_cm());
	delete new_study;
}
void Record::start_up_study(const string& name)
{
	study* new_study = new study(up,name);
	num++;
	record->push_back(*new_study);
	cost_min += (new_study->get_ch() * 60 + new_study->get_cm());
	delete new_study;
}
void Record::start_down_study(const string& name)
{
	study* new_study = new study(down,name);
	num++;
	record->push_back(*new_study);
	cost_min += (new_study->get_ch() * 60 + new_study->get_cm());
	delete new_study;
}
void Record::show_record()
{
	IMAGE background;
	::loadimage(&background, "./file/background.jpg", 1848 / 3 * 2, 862 / 3 * 2);//依照窗口规格加载图片
	PushButton  Esc("返回", 1848 / 3 * 2 - 350, 40);
	ExMessage msg;	//鼠标键盘输入信息
	int x = 50;
	int y = 50;
	while (1)
	{
		x = 70;
		y = 70;
		BeginBatchDraw();
		::putimage(0, 0, &background);//绘制背景板
		settextcolor(BLACK);//设置颜色
		settextstyle(0, 0, "宋体");//设置字体样式
		//输出学习记录
		outtextxy(x+10, y, (string("今天你一共学习:") + to_string(num) + "次").c_str());
		outtextxy(x+10+textwidth((string("今天你一共学习:") + to_string(num) + "次").c_str())+20, y, (string("共") + to_string(cost_min/60) + "小时"+to_string(cost_min%60)+"分钟").c_str());
		int i = 1;
		for (list<study>::iterator it = record->begin(); it != record->end(); it++)
		{
			y += 50;
			outtextxy(x, y, (to_string(i)+".").c_str());
			it->disp(x+15, y);
			i++;
		}
		if (num < 2)
			outtextxy(x+10, y+60, string("今天学习时长略低哦,继续加油！") .c_str());
		else 
			outtextxy(x+10, y + 60, string("今天状态不错哦，继续保持！").c_str());


		Esc.show();

		FlushBatchDraw();
		//等待退出
		peekmessage(&msg, EM_MOUSE);
		if (Esc.Click(msg))
			break;
	}

}

string Record::get_task_name(int i)
{

	list<study>::iterator it = record->begin();
	int cout = 1;
	for (cout = 1; cout < i; cout++)
		it++;
	return it->get_name();
}


void Record::draw_clock(int time_x, int time_y, int space)
{
	time_t t;
	tm tmp;
	time(&t);
	localtime_s(&tmp, &t);
	int h = tmp.tm_hour;
	int m = tmp.tm_min;
	int s = tmp.tm_sec;
	if (h < 10)
		outtextxy(time_x, time_y, ("0" + to_string(h) + ":").c_str());
	else
		outtextxy(time_x, time_y, (to_string(h) + ":").c_str());
	if (m < 10)
		outtextxy(time_x + space, time_y, ("0" + to_string(m)).c_str());
	else
		outtextxy(time_x + space, time_y, to_string(m).c_str());
}
