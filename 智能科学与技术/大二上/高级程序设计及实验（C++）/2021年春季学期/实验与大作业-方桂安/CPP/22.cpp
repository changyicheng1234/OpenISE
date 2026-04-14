#include<iostream>
#include<math.h>
constexpr double pi = 3.1415;
using namespace std;
class Shape
{
public:
	Shape(){}
	~Shape(){}
	virtual double GetArea() = 0;
	virtual int GetLength() = 0;
	virtual void Display() = 0;
protected:

};
class Triangle:public Shape
{
public:
	Triangle(int a, int b, int c) { this->a = a; this->b = b; this->c = c; }
	~Triangle(){}
	double GetArea() {
		double s, area;
		s = (a + b + c) / 2;
		area = sqrt(s * (s - a) * (s - b) * (s - c));
		return area;
	}
	int GetLength() {
		return a + b + c;
	}
	void Display() {
		cout << "Triangle:a=" << a << " b=" << b << " c=" << c << endl;
		cout << "Area:" << GetArea() << " Length:" << GetLength() << endl;
	}
protected:
	int a;
	int b;
	int c;
};

class Circle:public Shape
{
public:
	Circle(int r) { this->r = r; }
	~Circle(){}
	double GetArea() {
		return pow(r, 2) * pi;
	}
	int GetLength() {
		return int(2 * pi * r);
	}
	void Display(){
		cout << "Circle:r=" << r << endl;
		cout << "Area:" << GetArea() << " Length:" << GetLength() << endl;
	}
protected:
	int r;
};
void print(Shape*ptr) {
	ptr->Display();
}
int main() {
	Triangle s1(6, 6, 6);
	Circle s2(2);
	Shape* shapes[2] = { &s1,&s2 };
	for (int i = 0; i < 2; i++) {
		print(shapes[i]);
	}
	return 0;
}