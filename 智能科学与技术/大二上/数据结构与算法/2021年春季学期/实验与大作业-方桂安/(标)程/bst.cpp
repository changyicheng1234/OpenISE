#include<stdio.h>
#include<stdlib.h>

typedef struct BiTNode{
    int data;
    struct BiTNode *lchild;
    struct BiTNode *rchild;
} *BSTNode;

BSTNode root = NULL;

BSTNode search_BST(BSTNode pTree,int key,BSTNode parent){
    if(!pTree) return parent;
    else if(key < pTree->data)
        return search_BST(pTree->lchild,key,pTree);
    else
        return search_BST(pTree->rchild,key,pTree);
}

void insert(int key){
    BSTNode pNew = (BSTNode)malloc(sizeof(BiTNode));
    pNew->data = key; pNew->lchild = pNew->rchild = NULL;

	BSTNode p = search_BST(root,key,NULL);
    if(!p) root = pNew;
    else if (key < p->data) p->lchild = pNew;
    else p->rchild = pNew;
}

void in_traverse(BSTNode p){
    if(p->lchild) in_traverse(p->lchild);
    printf("%d ",p->data);
    if(p->rchild) in_traverse(p->rchild);
}

void print_depth(BSTNode p, int depth){
    if (p->lchild) print_depth(p->lchild, depth+1);
    printf("%d ", depth);
    if (p->rchild) print_depth(p->rchild, depth+1);
}

void print_degree(BSTNode p){
    if (p->lchild) print_degree(p->lchild);
    printf("%d ", (p->lchild!=NULL) + (p->rchild!=NULL));
    if (p->rchild) print_degree(p->rchild);
}
    
int main(){
	int n, key;
    scanf("%d",&n);
    for(int i=0; i < n; i++){
    	scanf("%d",&key); insert(key);
	}
	in_traverse(root); printf("\n");
    print_depth(root, 1); printf("\n");
    print_degree(root); return 0;
}
