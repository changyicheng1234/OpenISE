#include<iostream>
#include<string>
using namespace std;

class Student
{
public:
	Student() { this->id = 0; this->name = '\0'; }
	virtual ~Student() {}
	string GetName() {
		return this->name;
	}
	int GetId() {
		return this->id;
	}
	virtual void Display() = 0;
	virtual void printMore(Student*) const {};
protected:
	string name;
	int id;
};
class ScienceStudent:public Student
{
public:
	ScienceStudent(string n, int id) { this->name = n; this->id = id; }
	~ScienceStudent(){}
	int GetYear() {
		return this->year;
	}
	void Display() override{
		cout << "ScienceStudent£º" << endl;
		cout << this->GetName() << ":" << this->GetId() << " in " << this->GetYear() << endl;
	}
protected:
	int year=2020;
};
class EngineeringStudent :public Student
{
public:
	EngineeringStudent(string n, int id) { this->name = n; this->id = id; }
	~EngineeringStudent(){}
	int GetYear() {
		return this->year;
	}
	void Display() override {
		cout << "EngineeringStudent£º" << endl;
		cout << this->GetName() << ":" << this->GetId() << " in " << this->GetYear() << endl;
	}
protected:
	int year=2020;
};
void printMore(Student* s)  {
	s->Display();
}
int main() {
	ScienceStudent s1("Alice",1);
	ScienceStudent s2("Bob",2);
	EngineeringStudent s3("Carrol",3);
	EngineeringStudent s4("David",4);
	Student* students[4] = {&s1,&s2,&s3,&s4};
	for (int i = 0; i < 4; i++){
		printMore(students[i]);
	}
	return 0;
}