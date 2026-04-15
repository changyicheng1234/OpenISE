#include<stdio.h>
#include <stdlib.h>

int m,n;

typedef struct Node{
	int data;
	struct Node*next;
}person;

person *newnode(int data)   //建立循环链表 
{
	person *temp=(person*)malloc(sizeof(person));
	temp->next=temp;
	temp->data=data;
}

void getposition(int n,int m)  // 找到剩下的那一个人 
{
	person *head=newnode(1);
	person *prev=head;
	for(int i=2;i<=n;i++)
	{
		prev->next=newnode(i);
		prev=prev->next;
	}
	prev->next=head;  //建立循环链表 
	
	person *p1=head,*p2=prev;  
	
	while(p1->next != p1){
	   int count=1;
	   while(count !=m){
		   p2=p1;
		   p1=p1->next;
		   count++;
        }
	   p2->next=p1->next;
	   free(p1);
	   p1=p2->next;
    } 
    printf("%d",p1->data);
}

int main(){
	scanf("%d%d",&n,&m);
	getposition(n,m);
	return 0; 
}
