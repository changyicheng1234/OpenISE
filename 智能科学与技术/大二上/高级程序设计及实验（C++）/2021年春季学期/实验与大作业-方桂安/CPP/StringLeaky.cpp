#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>

class StringLeaky
{
private:
	char* str;	// pointer to string
	int len;	// length of string
	//static int num_strings;
	static int mem;
public:
	StringLeaky(const char* s)
	{
		len = std::strlen(s);
		str = new char[len + 1];
		std::strcpy(str, s);
		mem += len;
		//num_strings++;
		//std::cout << num_strings << ":\"" << str << "\" object created\n";
		std::cout << "total memory: " << mem << " bytes." << std::endl;
	}
	int GetLen() const { return len; }
	~StringLeaky() 
	{ 
		delete[]str;
		mem -= len;
	}
};

void EatMemory()
{
	StringLeaky str("Hello world! I'm from SYSU.");
	std::cout << "allocated memory " << str.GetLen() << " bytes, " << "without reclaim.\n";
}

//int StringLeaky::num_strings = 0;
int StringLeaky::mem = 0;

int main()
{
	for (;;)
	{
		EatMemory();
		Sleep(50);
	}
	return 0;
}