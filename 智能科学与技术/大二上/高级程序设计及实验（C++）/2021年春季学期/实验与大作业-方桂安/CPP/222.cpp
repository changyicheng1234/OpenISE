#include<iostream>
using namespace std;
template<class T>
void my_swap(T& a, T& b) {
	T temp;
	temp = a;
	a = b;
	b = temp;
}
int main() {
	int a = 5;
	int b = 43;
	cout << a << ' ' << b << endl;
	my_swap(a, b);
	cout << a << ' ' << b << endl;
	string c = "Enderfga";
	string d = "Pann";
	cout << c << ' ' << d << endl;
	my_swap(c, d);
	cout << c << ' ' << d << endl;
}