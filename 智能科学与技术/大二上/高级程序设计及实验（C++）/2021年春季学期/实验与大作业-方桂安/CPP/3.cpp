#include <iostream>
#include <string>
using namespace std;
void stringFilter(const string &str1, string &str2) {
	int pos;
	for (int i = 0; i < str1.length(); i++)
	{
		pos = str1.find(str1[i]);
		if (pos == i)
			str2.push_back(str1[i]);
	}
}
void stringZip(const string& str1, string& str2) {
	int n = 1;
	for (int i = 0; i < str1.length(); i++)
	{
		if(str1[i]==str1[i+1])
		{
			n++;
		}
		else
		{
			string temp = to_string(n);
			if (n != 1)
				str2 += temp;
			n = 1;
			str2 += str1[i];
		}
	}
}
int main() {
	string str;
	void stringFilter(const string &str1, string &str2);
	void stringZip(const string &str1, string &str2);
	stringFilter("deefd", str);
	cout << str << endl;;
	str.erase(0);
	stringFilter("afafafaf", str);
	cout << str << endl;
	str.erase(0);
	stringFilter("pppppppp", str);
	cout << str << endl;
	str.erase(0);
	stringZip("cccddecc", str);
	cout << str << endl;
	str.erase(0);
	stringZip("adef", str);
	cout << str << endl;
	str.erase(0);
	stringZip("pppppppppppp", str);
	cout << str;
}