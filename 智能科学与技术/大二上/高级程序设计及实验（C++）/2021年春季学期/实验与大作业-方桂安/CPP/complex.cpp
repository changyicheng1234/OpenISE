#include <iostream>
#include <cmath>
using namespace std;

class Complex
{
public:
	Complex();
	Complex(double r);
	Complex(double r, double i);
	Complex(const Complex& c);
	~Complex();

	void add(Complex& c)
	{
		real += c.real;
		imaginary += c.imaginary;
		magnitude = sqrt(real * real + imaginary * imaginary);
	}

	void show()
	{
		/*
		if (imaginary == 0)
		{
			cout << real << endl;
		}
		else
		{
			cout << real << "+" << imaginary << "i" << endl;
		}
		*/
		cout << real << "+" << imaginary << "i" << endl;
		cout << "magnitude = " << magnitude << endl;
	}

	static void showCount()
	{
		cout << "Complex count = " << count << endl;
	}

	friend Complex mul(Complex& c1, Complex& c2);

private:
	double real;
	double imaginary;
	double magnitude;
	static int count;
};

Complex::Complex()
{
	real = 1.0;
	imaginary = 1.0;
	magnitude = sqrt(real * real + imaginary * imaginary);
	count++;
}

Complex::Complex(double r)
{
	real = r;
	imaginary = 0;
	magnitude = sqrt(real * real + imaginary * imaginary);
	count++;
}

Complex::Complex(double r, double i)
{
	real = r;
	imaginary = i;
	magnitude = sqrt(real * real + imaginary * imaginary);
	count++;
}

Complex::Complex(const Complex& c)
{
	real = c.real;
	imaginary = c.imaginary;
	magnitude = sqrt(real * real + imaginary * imaginary);
	count++;
}

Complex::~Complex(void)
{
	count--;
}

Complex mul(Complex& c1, Complex& c2)
{
	Complex c;
	c.real = c1.real * c2.real - c1.imaginary * c2.imaginary;
	c.imaginary = c1.imaginary * c2.real + c1.real * c2.imaginary;
	return c;
}

int Complex::count = 0;

int main()
{
	Complex C1_1;
	cout << "C1_1 = ";
	C1_1.show();
	//C1_1.showCount();
	Complex::showCount();

	Complex C1_2(1, 1);
	cout << "C1_2 = ";
	C1_2.show();

	Complex C1_3(1);
	cout << "C1_3 = ";
	C1_3.show();

	Complex C2 = 1;
	cout << "C2 = ";
	C2.show();

	Complex C1_4 = C2;
	cout << "C1_4 = ";
	C1_4.show();

	C1_4.add(C2);
	cout << "C1_4 + C2 = ";
	C1_4.show();
	C1_4.showCount();

	//Complex C;
	Complex C = mul(C1_1, C1_4);
	cout << "C = ";
	C.show();
	C.showCount();

	return 0;
}