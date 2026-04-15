#include <stdio.h>   
#include <stdlib.h>     
#include <string.h>
#include <pthread.h> 
#include<iostream>
#include<unistd.h>
using namespace std;
//因为pthread 库非Linux 默认库，
// 编译时使用 g++ *.c -l pthread 
void * print1(void* args)
{
	// fill
	cout << "子线程 1 运行。。。" << endl;
	sleep(5);
	
}
void* print2(void * args)
{
	cout << "子线程 2 运行" << endl;
	

}
int main()
{
	pthread_t thread1, thread2;
	//创建线程
	pthread_create(&thread1, NULL, print1, NULL);
	pthread_create(&thread2, NULL, print2, NULL);
	//等待线程执行结束
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("The End...\n");
}
