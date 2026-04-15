
#include<fstream>
#include<iostream>
#include<string>
#include<cstdlib>
#include<cstdio>
using namespace std;
ifstream fin,fout,fstd;
ofstream fscore,freport;
int score;
const int maxn  = 10001;
void judge()
{
	int n;
    int a[maxn+1];
    int b[maxn+1];
    int L[maxn+1];
    int flag_[maxn+1]={0};
    int h[maxn+1];
	int label_used[maxn+1]={0};
	fin>>n;
	for(int i=1;i<=n-1;i++) fin>>a[i]>>b[i];
	for(int i=1;i<=n;i++)  fout>>L[i];
	
	for(int i=1;i<=n;i++)
	{
		label_used[L[i]]=1;    //判断顶点 
	} 
	int sum1=0;
	for(int i=1;i<=n;i++)
		sum1=sum1+label_used[i]; // 判断顶点 
		
	for(int i=1;i<=n-1;i++)
	{
		h[i]=abs(L[a[i]]-L[b[i]]);  //判断边 
		flag_[h[i]]=1;
	}
	int sum2=0;
	for(int i=1;i<=n-1;i++)
 	    sum2=sum2+flag_[i];  //判断边 
 	
	if((sum1==n) &&(sum2==n-1))
	{
	     fscore<<score<<endl;
	     freport<<"Success"<<endl;
	}
	else
	{
	
	    freport<<"Invalid result!";
	    fscore<<0<<endl;
	}
 	  
}

 int main(int argc,char*argv[])
 {
		
		//put something to fstreams...
	/*
	argv[1]:输入文件
	argv[2]:选手输出文件 
	argv[3]:标准输出文件 
	argv[4]:单个测试点分值 
	argv[5]:输出最终得分的文件 
	argv[6]:输出错误报告的文件 
	*/
	fin.open(argv[1]);
	fout.open(argv[2]);
	fstd.open(argv[3]);
	fscore.open(argv[5]);
	freport.open(argv[6]);
	
	score=atoi(argv[4]);
	
	judge();
    
	fin.close();
    fout.close();
    fstd.close();
    fscore.close();
    freport.close();
 	return 0;
 }
