#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define maxn 100005
namespace IO {
    template <typename T>inline T read()
    {
        char c = getchar();
        T ans = 0;
        bool f = false;
        while (c < '0' || c>'9')
        {
            if (c == '-')f = true;
            c = getchar();
        }
        while ('0' <= c && c <= '9')
        {
            ans = ans * 10 + c - '0';
            c = getchar();
        }
        return f ? -ans : ans;
    }
}
/*end input&output accelerate*/

/*begin AVL Tree*/
namespace AVL {
    /*begin basic functions*/
    template <typename T>inline T abs(T x)
    {
        if (x >= 0)return x;
        return -x;
    }
    template <typename T>inline T max(T a, T b)
    {
        return a > b ? a : b;
    }
    template <typename T>inline T min(T a, T b)
    {
        return a < b ? a : b;
    }
    /*end basic functions*/

    /*begin node*/
    template <typename T>struct Node
    {
        Node<T>* lef, * rig;
        int dep, num, sum;
        T key;
        Node() :lef(NULL), rig(NULL), dep(0), sum(0), key(0), num(0) {}
        Node(T _key, Node<T>* _lef = NULL, Node<T>* _rig = NULL) :lef(_lef), rig(_rig), key(_key), dep(1), num(1), sum(1) {}
        int fac()
        {
            int ldep = 0, lsum = 0;
            int rdep = 0, rsum = 0;
            if (lef != NULL)ldep = lef->dep, lsum = lef->sum;
            if (rig != NULL)rdep = rig->dep, rsum = rig->sum;
            dep = max(ldep, rdep) + 1;
            sum = lsum + rsum + num;
            return rdep - ldep;
        }
    };
    /*end node*/

    template <typename T>int H(Node<T>* c)
    {
        if (c == NULL)return 0;
        c->fac();
        return c->dep;
    }

    /*begin avl tree body*/
    template <typename T>class Tree
    {
    public:
        Tree() :root(NULL) {}
        Node<T>* search(T key)
        {
            Node<T>* c;
            c = root;
            while (c != NULL)
            {
                if (c->key == key)return c;
                if (c->key > key)c = c->lef;
                if (c->key < key)c = c->rig;
            }
            return NULL;
        }
        Node<T>* insert(T key)
        {
            return insert(root, key);
        }
        void remove(T key)
        {
            remove(root, key, 1);
        }
        int rank(T key)
        {
            return rank(root, key);
        }
        T kth(int k)
        {
            return kth(root, k);
        }
        T prev(T key)
        {
            prekey = -1;
            prev(root, key);
            return prekey;
        }
        T next(T key)
        {
            nexkey = -1;
            next(root, key);
            return nexkey;
        }
        int depth(T)
        {
            return depth(root);
        }
        int depth2(T)
        {
            return depth2(root);
        }
    private:
        /*begin find max*/
        Node<T>* maxone(Node<T>* c)
        {
            if (c != NULL)
            {
                while (c->rig != NULL)
                    c = c->rig;
                return c;
            }
            return NULL;
        }
        /*end find max*/

        /*begin find min*/
        Node<T>* minone(Node<T>* c)
        {
            if (c != NULL)
            {
                while (c->lef != NULL)
                    c = c->lef;
                return c;
            }
            return NULL;
        }
        /*end find min*/

        /*begin rotate*/
        Node<T>* clockwise_rotate(Node<T>*& c)
        {
            Node<T>* r = c->lef;
            c->lef = r->rig;
            r->rig = c;
            c->fac();
            r->fac();
            return r;
        }
        Node<T>* anti_clockwise_rotate(Node<T>*& c)
        {
            Node<T>* r = c->rig;
            c->rig = r->lef;
            r->lef = c;
            c->fac();
            r->fac();
            return r;
        }
        /*end rotate*/

        /*begin maintain*/
        void maintain(Node<T>*& c)
        {
            if (c == NULL)
                return;
            int fac = c->fac();
            if (fac < -1)
            {
                //left-left child;
                if (H(c->lef->lef) > H(c->lef->rig))
                    c = clockwise_rotate(c);
                //left-right child;
                else
                {
                    c->lef = anti_clockwise_rotate(c->lef);
                    c = clockwise_rotate(c);
                }
            }
            if (fac > 1)
            {
                //right-right child;
                if (H(c->rig->rig) > H(c->rig->lef))
                    c = anti_clockwise_rotate(c);
                //right-left child;
                else
                {
                    c->rig = clockwise_rotate(c->rig);
                    c = anti_clockwise_rotate(c);
                }
            }
            c->fac();
        }
        /*end maintain*/

        /*begin insert*/
        Node<T>* insert(Node<T>*& c, T key)
        {
            if (c == NULL)
                c = new Node<T>(key);
            else
            {
                if (key == c->key)c->num++;
                //key goes into the left child;
                if (key < c->key)
                    c->lef = insert(c->lef, key);
                //key goes into the right child;
                if (key > c->key)
                    c->rig = insert(c->rig, key);
                maintain(c);
            }
            return c;
        }
        /*end insert*/

        /*begin remove*/
        void remove(Node<T>*& c, T key, int s)
        {
            if (c == NULL)return;
            if (c->key > key)
            {
                remove(c->lef, key, s);
                maintain(c);
                return;
            }
            if (c->key < key)
            {
                remove(c->rig, key, s);
                maintain(c);
                return;
            }
            c->num -= s;
            if (c->num == 0)
            {
                if (c->lef != NULL && c->rig != NULL)
                {
                    if (H(c->lef) > H(c->rig))
                    {
                        Node<T>* temp = maxone(c->lef);
                        c->key = temp->key;
                        c->num = temp->num;
                        remove(c->lef, temp->key, temp->num);
                    }
                    else
                    {
                        Node<T>* temp = minone(c->rig);
                        c->key = temp->key;
                        c->num = temp->num;
                        remove(c->rig, temp->key, temp->num);
                    }
                }
                else
                {
                    Node<T>* temp = c;
                    if (c->lef != NULL)
                        c = c->lef;
                    else
                        c = c->rig;
                    delete temp;
                }
            }
            maintain(c);
        }
        /*end remove*/

        /*begin rank*/
        int rank(Node<T>* c, T key)
        {
            if (c == NULL)
                return 0;
            if (c->key == key)
                return (c->lef != NULL) ? (c->lef->sum) + 1 : 1;
            if (c->key > key)
                return rank(c->lef, key);
            return (c->lef != NULL) ? (c->lef->sum + c->num + rank(c->rig, key)) : (c->num + rank(c->rig, key));
        }
        /*end rank*/

        /*begin kth*/
        T kth(Node<T>* c, int k)
        {
            if (c->lef != NULL)
            {
                if (c->lef->sum >= k)
                    return kth(c->lef, k);
                else
                    k -= c->lef->sum;
            }
            if (k <= c->num)return c->key;
            return kth(c->rig, k - c->num);
        }
        /*end kth*/

        /*begin find previous*/
        void prev(Node<T>* c, T key)
        {
            if (c == NULL)return;
            if (c->key < key)
            {
                prekey = c->key;
                prev(c->rig, key);
            }
            else
                prev(c->lef, key);
        }
        /*end find previous*/

        /*begin find next*/
        void next(Node<T>* c, T key)
        {
            if (c == NULL)return;
            if (c->key > key)
            {
                nexkey = c->key;
                next(c->lef, key);
            }
            else
                next(c->rig, key);
        }
        /*end find next*/
        int depth(Node<T>* c) {
            if (c == NULL)return 0;
            int a = depth(c->lef);
            int b = depth(c->rig);
            if (a > b)
                return a + 1;
            else return b + 1;
        }
        int depth2(Node<T>* c) {
            Node<T> *STACK1[maxn], *p = c;
		    int STACK2[maxn];
		    int cur , max = 0, top = -1;
		    if(c != NULL){
		        cur = 1;
		        while(top != -1 || p != NULL){
		            while(p != NULL){
		                STACK1[++top] = p;
		                STACK2[top] = cur++;
		                p = p->lef;
		            }
		            p = STACK1[top];
		            cur = STACK2[top--];
		            if(p->lef==NULL && p->rig==NULL)
		                if(cur > max)
		                    max = cur;
		            p = p->rig;
		            cur++;
		        }
    }
    return max;
        }
    private:
        Node<T>* root;
        T prekey, nexkey;
    };
    /*end avl tree body*/
}
/*end AVL Tree*/

int ans1[maxn], ans2[maxn];
int main() {
    using namespace IO;
    using namespace AVL;
    int (*R)() = read<int>;
    Tree<int> Tr;
    int n = R();
    int count1 = 0, count2 = 0;
    while (n--)
    {
        int f = R();
        int x = R();
        if (f == 1)
            ans2[count2++] =(Tr.insert(x))->dep;
        else if (f == 3)
            {
			ans1[count1++] = Tr.rank(x);
			ans2[count2++] =ans2[count2-2];}
        else if (f == 4)
            {
			ans1[count1++] = Tr.kth(x);
			ans2[count2++] =ans2[count2-2];}
    }
    for (int i = 0; i < count1; i++)
        std::cout << ans1[i] << " ";
    std::cout << std::endl;
    for (int i = 0; i < count2; i++)
        std::cout << ans2[i] << " ";
    return 0;
}