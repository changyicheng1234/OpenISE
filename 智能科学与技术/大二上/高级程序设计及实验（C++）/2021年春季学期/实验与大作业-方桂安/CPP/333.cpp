#include<iostream>
using namespace std;
const int SIZE = 100;
template<class T>
void swap(T* a, T* b) {
 T* t;
 t = a; a = b; b = t;
}
template<class Type>
class Array {
private:
 Type a[SIZE];
 int length;
public:
 Array(Type* b, int n);
 void print() {
  for (int i = 0; i < length; i++) {
   cout << a[i]<<" ";
  }
  cout << endl;
 }
 void sort();
 void reverse();
 int find(Type t);
 Type &sum();
};
template<class Type>
Array<Type>::Array(Type* b, int n) {
 if (n > SIZE)
 {
  cout << "数组太大！" << endl;
  exit(1);
 }
 length = n;
 for (int i = 0; i < n; i++)
  a[i] = b[i];
}
template<class Type>
void Array<Type>::sort() {
 for (int i = 0; i < length - 1; i++)
  for (int j = i; j < length; j++)
   if (a[j] < a[i]) swap(a[i], a[j]);
}
template<class Type>
void Array<Type>::reverse() {
 for (int i = 0; i < length / 2; i++) {
  swap(a[i], a[length - 1 - i]);
 }
}
template<class Type>
int Array<Type>::find(Type x) {
 for (int i = 0; i < length; i++)
  if (x == a[i])
   return i + 1;
}
template<class Type>
Type &Array<Type>::sum() {
 Type s=a[0];
 for (int i = 1; i < length; i++)
  s = s + a[i];
 return s;
}
int main() {
 int a[] = {1,2,5,4,3};
 double b[] = {10.0,8.0,7.0,9.0,6.0};
 Array<int> A(a, 5);
 Array<double> B(b, 5);
 A.sort(); B.sort();
 A.print(); B.print();
 A.reverse(); B.reverse();
 A.print(); B.print();
 cout<<A.find(3)<<" "<<B.find(9)<<endl;
 cout<<A.sum()<<" "<< B.sum();
 return 0;
}