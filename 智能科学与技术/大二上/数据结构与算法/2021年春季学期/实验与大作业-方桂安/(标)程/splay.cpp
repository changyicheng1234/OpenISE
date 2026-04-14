#include <iostream>
using namespace std;

typedef struct SplayNode* Tree;
typedef int ElementType;
struct SplayNode
{
    Tree parent;     //�ý��ĸ��ڵ㣬�������
    ElementType val; //���ֵ
    Tree lchild;
    Tree rchild;
    SplayNode(int val = 0) //Ĭ�Ϲ��캯��
    {
        parent = NULL;
        lchild = rchild = NULL;
        this->val = val;
    }
};
bool search(Tree& root, ElementType val);
Tree* search_val(Tree& root, ElementType val, Tree& parent);
bool insert(Tree& root, ElementType val, int& n);

Tree left_single_rotate(Tree& root, Tree node);
Tree right_single_rotate(Tree& root, Tree node);
void LR_rotate(Tree& root, Tree node);
void RL_rotate(Tree& root, Tree node);
void right_double_rotate(Tree& root, Tree node);
void left_double_rotate(Tree& root, Tree node);

int SplayTree(Tree& root, Tree node);
int up(Tree& root, Tree node);
Tree* Find_Min(Tree& root);
void Delete(Tree& root, ElementType val, int& n);
int num(Tree& root);
int Rank(Tree& root, int x, int& n);
Tree gettarget(Tree& root, int& k);
int get(Tree& root, int k, int& n);
void InOrder(Tree root);

//���Һ���������������
bool search(Tree& root, ElementType val)//����Ϊ����㣬��Ҫ���ҵ�val
{

    Tree parent = NULL;
    Tree* temp = NULL;
    temp = search_val(root, val, parent);
    if (*temp && *temp != root)
    {
        SplayTree(root, *temp);
        return true;
    }
    return false;
}

//���Һ�����ʵ��
Tree* search_val(Tree& root, ElementType val, Tree& parent)
{
    if (root == NULL)//������ҳɹ�:����������
        return &root;
    if (root->val > val)
        return search_val(root->lchild, val, parent = root);
    else if (root->val < val)
        return search_val(root->rchild, val, parent = root);
    return &root; //�������ʧ�ܣ����ظ��ڵ�,�������Ĳ������
}

//���뺯����ʵ��
bool insert(Tree& root, ElementType val, int& n)
{
    Tree* temp = NULL;
    Tree parent = NULL;
    //�Ȳ��ң�����ɹ���������룬���򷵻ظý������á�
    temp = search_val(root, val, parent);

    if (*temp == NULL) //��Ҫ��������
    {
        Tree node = new SplayNode(val);
        *temp = node;          //��Ϊ�������ͣ���������ֱ�Ӹ�ֵ�����˺ܶ��ˡ�
        node->parent = parent; //���ø��ڵ㡣
        n = SplayTree(root, node);
        return true;
    }
    return false;
}


//����������
Tree left_single_rotate(Tree& root, Tree node)//����:������ת���(��ת����)
{
    if (node == NULL)
        return NULL;
    Tree parent = node->parent;        //�丸���
    Tree grandparent = parent->parent; //���游���
    parent->rchild = node->lchild;     //�����丸�ڵ���Һ���
    if (node->lchild)                  //��������������node������ӵĸ��ڵ���Ϣ
        node->lchild->parent = parent;
    node->lchild = parent; //����node����������Ϣ
    parent->parent = node; //����ԭ���ڵ����Ϣ
    node->parent = grandparent;

    if (grandparent) //�����游���ӽ�����Ϣ
    {

        if (grandparent->lchild == parent)
            grandparent->lchild = node;
        else
            grandparent->rchild = node;
    }
    else   //�������游�ڵ㣬��ԭ���ڵ�Ϊ������ô��ת��nodeΪ��
        root = node;
    return node; //���ص�ǰ�����е��¸�
}
//����������
Tree right_single_rotate(Tree& root, Tree node)//����:������ת���(��ת����)
{
    if (node == NULL)
        return NULL;
    Tree parent, grandparent;   //�����������Գ�
    parent = node->parent;
    grandparent = parent->parent;
    parent->lchild = node->rchild;
    if (node->rchild)
        node->rchild->parent = parent;
    node->rchild = parent;
    parent->parent = node;
    node->parent = grandparent;
    if (grandparent)
    {
        if (grandparent->lchild == parent)
            grandparent->lchild = node;
        else
            grandparent->rchild = node;
    }
    else
        root = node;
    return node;//����:��ǰ�����е��¸�
}
//˫��������LR�ͣ�����AVL������
void LR_rotate(Tree& root, Tree node)//����Ϊ������󽫱�����������Ľ��
{
    left_single_rotate(root, node);  //�������
    right_single_rotate(root, node); 
}

//˫��������RL�ͣ�����AVL������
void RL_rotate(Tree& root, Tree node)//����Ϊ������󽫱�����������Ľ��
{
    right_single_rotate(root, node); //���Һ���
    left_single_rotate(root, node);
}

//���ε���������
void right_double_rotate(Tree& root, Tree node)//����Ϊ������󽫱�����������Ľ��
{
    right_single_rotate(root, node->parent); //�������丸�ڵ�
    right_single_rotate(root, node);         //��������Լ�
}

//���ε���������
void left_double_rotate(Tree& root, Tree node)//����Ϊ������󽫱�����������Ľ��
{
    left_single_rotate(root, node->parent);
    left_single_rotate(root, node);
}

//Splay��������
int SplayTree(Tree& root, Tree node)
{
    int n = 0;
    while (root->lchild != node && root->rchild != node && root != node) //��ǰ��㲻�Ǹ������߲�����������Һ��ӣ���������������ת����
    {
        n += up(root, node);
    }
    if (root->lchild == node) //��ǰ���Ϊ�������ӣ�ֻ�����һ�ε�����
    {
        root = right_single_rotate(root, node);
        n++;
    }
    else if (root->rchild == node) //��ǰ���Ϊ�����Һ��ӣ�ֻ�����һ�ε�����
    {
        root = left_single_rotate(root, node);
        n++;
    }
    return n;
}

//���������ѡ��ͬ����ת��ʽ
int up(Tree& root, Tree node)
{
    Tree parent, grandparent;
    int i, j, n = 0;
    parent = node->parent;
    grandparent = parent->parent;
    i = grandparent->lchild == parent ? -1 : 1;
    j = parent->lchild == node ? -1 : 1;
    if (i == -1 && j == -1) //AVL���е�LL��
    {
        right_double_rotate(root, node);
        n++;
    }
    else if (i == -1 && j == 1) //AVL���е�LR��
    {
        LR_rotate(root, node);
        n++;
    }
    else if (i == 1 && j == -1) //AVL���е�RL��
    {
        RL_rotate(root, node);
        n++;
    }
    else //AVL���е�RR��
    {
        left_double_rotate(root, node);
        n++;
    }
    return n;
}

//������ǰ��������С���
Tree* Find_Max(Tree& root)
{
    if (root->rchild)
        return Find_Max(root->rchild);
    return &root;   //��������С��������
}

//ɾ������
void Delete(Tree& root, ElementType val, int& n)
{
    Tree parent = NULL;
    Tree* temp;
    Tree* replace;
    n = 0;
    temp = search_val(root, val, parent); //�Ƚ��в��Ҳ���
    if (*temp)                            //������ҵ���
    {

        if ((*temp)->rchild && (*temp)->lchild)
        {

            replace = Find_Max((*temp)->lchild); //�ҵ��滻Ԫ��
            Tree d = *replace;
            (*temp)->val = (*replace)->val; //�滻
            Tree pa = (*replace)->parent;
            if ((*replace)->lchild)
            {
                Tree lc = (*replace)->lchild;
                lc->parent = pa;
                if (*replace == pa->lchild)
                {
                    pa->lchild = lc;
                }
                else if (*replace == pa->rchild)
                {
                    pa->rchild = lc;
                }
            }
            else
            {
                if (*replace == pa->lchild)
                {
                    pa->lchild = NULL;
                }
                else if (*replace == pa->rchild)
                {
                    pa->rchild = NULL;
                }
            }

            if (d != root && d->parent != root)
            {
                n = SplayTree(root, d->parent);
            }
            delete d;
        }
        else if ((*temp)->rchild)
        {
            Tree d = *temp;
            (*temp)->rchild->parent = (*temp)->parent;
            if ((*temp) != root)
            {
                if ((*temp) == (*temp)->parent->lchild)
                {
                    (*temp)->parent->lchild = (*temp)->rchild;
                }
                else
                {
                    (*temp)->parent->rchild = (*temp)->rchild;
                }
                Tree t = (*temp)->parent;
                n = SplayTree(root, t);
            }
            else
            {
                root = (*temp)->rchild;
                root->parent = NULL;
            }

            delete d;
        }
        else if ((*temp)->lchild)
        {
            Tree d = *temp;
            (*temp)->lchild->parent = (*temp)->parent;
            if ((*temp) != root)
            {
                if ((*temp) == (*temp)->parent->lchild)
                {
                    (*temp)->parent->lchild = (*temp)->lchild;
                }
                else
                {
                    (*temp)->parent->rchild = (*temp)->lchild;
                }
                Tree t = (*temp)->parent;
                n = SplayTree(root, t);
            }
            else
            {
                root = (*temp)->lchild;
                root->parent = NULL;
            }
            delete d;
        }
        else
        {
            Tree d = *temp;
            if ((*temp) != root)
            {
                Tree t = (*temp)->parent;
                if ((*temp) == (*temp)->parent->lchild)
                {
                    (*temp)->parent->lchild = NULL;
                }
                else if ((*temp) == (*temp)->parent->rchild)
                {
                    (*temp)->parent->rchild = NULL;
                }
                n = SplayTree(root, t);
            }
            else
            {
                root = NULL;
            }
            delete d;
        }
    }
}

int num(Tree& root)//����������ܽڵ���
{ 
    if (root != NULL)
    {
        int n = 0;
        n++;
        n += num(root->lchild);
        n += num(root->rchild);
        return n;
    }
    else
    {
        return 0;
    }
}

int Rank(Tree& root, int x, int& n)
{

    Tree pa = NULL;
    Tree* target = search_val(root, x, pa);
    Tree p = *target;
    int r = 1 + num(p->lchild);

    while (p->parent != NULL)
    {
        if (p->parent->rchild == p)
        {
            r = r + 1 + num(p->parent->lchild);
        }

        p = p->parent;
    }
    n = SplayTree(root, *target);
    return r;
}

Tree gettarget(Tree& root, int& k)
{
    Tree t1, t2;
    if (root == NULL)
    {
        return NULL;
    }

    t1 = gettarget(root->lchild, k);
    k--;
    if (k == 0)
    {
        return root;
    }
    t2 = gettarget(root->rchild, k);
    if (t1 != NULL)
    {
        return t1;
    }
    else
    {
        return t2;
    }
}

int get(Tree& root, int k, int& n)
{
    Tree target = gettarget(root, k);
    n = SplayTree(root, target);
    return target->val;
}
void InOrder(Tree root)
{
    if (root == NULL)
        return;
    InOrder(root->lchild);
    printf("%d ", root->val);
    InOrder(root->rchild);
}
#define MAXM 100001
int ans1[MAXM];
int ans2[MAXM];
int main()
{

    Tree root = NULL;
    
    int k = 0;
    int m, a, x, n;
    scanf("%d", &m);
    for (int i = 0; i < m; i++)
    {
        scanf("%d %d", &a, &x);
        if (a == 1)
        {
            insert(root, x, n);
        }
        else if (a == 2)
        {
            Delete(root, x, n);
        }
        else if (a == 3)
        {
            ans1[k++] = Rank(root, x, n);
        }
        else if (a == 4)
        {
            ans1[k++] = get(root, x, n);
        }
        ans2[i] = n;

    }
    for (int i = 0; i < k; i++)
    {
        printf("%d ", ans1[i]);
    }
    printf("\n");
    for (int i = 0; i < m; i++)
    {
        printf("%d ", ans2[i]);
    }
    return 0;
}