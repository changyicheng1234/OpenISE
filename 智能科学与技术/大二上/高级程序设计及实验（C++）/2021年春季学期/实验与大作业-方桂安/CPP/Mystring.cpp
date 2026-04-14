#include<iostream>
#include<string.h>
using namespace std;
class Mystring
{
public:
	Mystring() { this->cont = '\0'; this->len = 0; };
	Mystring(string n) { this->cont = n; this->len = n.length(); }
	Mystring(const Mystring& s) { 
		this->cont = s.cont;
		this->len = s.len;
	}
	~Mystring(){}
	Mystring operator=(const Mystring& s) {
		this->cont = s.cont;
		this->len = s.len;
		return *this;
	}
	friend ostream& operator<<(ostream& os, const Mystring&s){
		os << "The content:"<<s.cont <<" The length:"<<s.len;
		return os;
	}
	friend istream& operator>>(istream& is,  Mystring& s) {
		string temp;
		is >> temp;
		Mystring t(temp);
		s = t;
		return is;
	}
	bool operator!() {
		if (this->len != 0)
			return false;
		else return true;
	}
	bool operator>(const Mystring& s) {
		return s.cont>(this->cont);
	}
	bool operator<(const Mystring& s) {
		return s.cont < (this->cont);
	}
	bool operator==(const Mystring& s) {
		return this->cont == (s.cont);
	}
	bool operator!=(const Mystring& s) {
		return this->cont != (s.cont);
	}
private:
	string cont;
	int len;
};

int main() {
	Mystring s1("Enderfga");
	cout << s1 <<endl;
	Mystring s2;
	if (!s2)
		cout << "Empty!" << endl;
	cin >> s2;
	cout << s2 << endl;
	if (s1 < s2)
		cout << "y";
	else cout << "n";
	if (s1 > s2)
		cout << "y";
	else cout << "n";
	if (s1 == s2)
		cout << "y";
	else cout << "n";
	if (s1 != s2)
		cout << "y";
	else cout << "n";
	return 0;
}