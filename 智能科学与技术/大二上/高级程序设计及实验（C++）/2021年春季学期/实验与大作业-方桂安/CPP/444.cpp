#include<iostream>
using namespace std;
template<class T>
class myIncrement {
private:
 T value;
public:
 myIncrement(T arg) { value = arg; }
 T toIncrement() { return ++value; }
};
int main() {
 myIncrement<int>myint(7);
 myIncrement<double>mydouble(11.0);
 cout << "Incremented int value:" << myint.toIncrement() << endl;
cout<<"Incremented double value: "<<mydouble.toIncrement()<< endl;
  return 0;
}
