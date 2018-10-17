//Skiplist的实现。

#include<iostream>
#include<random>
using namespace std;

//定义节点
struct sList
{
    int level;
    int key;
    sList **next;//用于储存不同ｌｅｖｅｌ的链表，如next[２]指的是level 2的下一个元素

};

class SkipList
{
public:
    void InsertList(int x);
    void DeleteList(int x);
    int SearchList(int x);

    void DisplayList();
    SkipList(int max_level);
    ~SkipList();

private:
    int RandomLevel();
    int MAX_LEVEL;
    sList *head;
};

//释放内存，需要释放每个节点里面的next元素
SkipList::~SkipList()
{
    sList *curr=head;
    while(curr->next[0]!=nullptr)
    {
        sList *temp=curr;
        curr=curr->next[0];
        delete temp->next;
        delete temp;
    }
}
//初始化，获得max_level,并将head->next赋值为ｎｕｌｌｐｔｒ//
SkipList::SkipList(int max_level) {
    head=new sList;
    head->next=new sList*[max_level];
    for(int i=0;i<max_level;i++)
        head->next[i]=nullptr;
    MAX_LEVEL=max_level-1;
}

void SkipList::InsertList(int x) {
    //产生随机的ｌｅｖｅｌ//

    int level=RandomLevel();

    //寻找ｌｅｖｅｌ之前的ｐｒｅｖ//
    sList **prev;
    prev=new sList*[level+1];
    for(int i=0;i<=level;i++)
    {
        sList *curr=head;
        if (curr->next[i] == nullptr || curr->next[i]->key > x)
            prev[i] = curr;
            {
            while (curr->next[i] != nullptr && curr->next[i]->key < x)
                curr = curr->next[i];
            prev[i]=curr;
        }
    }
    //构建新节点//
    sList *temp;
    temp=new sList;
    temp->key=x;
    temp->level=level;
    temp->next=new sList*[MAX_LEVEL+1];
   //插入，各个ｌｅｖｅｌ独立，在但每个节点可以连接比它小的ｌｅｖｅｌ
    for(int i=level;i>=0;i--)
    {
        temp->next[i]=prev[i]->next[i];
        prev[i]->next[i]=temp;
    }

}

void SkipList::DeleteList(int x) {
    sList **prev;
    int level=-1;
    prev=new sList*[MAX_LEVEL+1];
    sList *curr=head;
    for(int i=MAX_LEVEL;i>=0;i--)
    {
        if(curr->next[i]== nullptr||curr->next[i]->key>x)//寻找所删除key所在的最大的ｌｅｖｅｌ，通过ｔａｉｌ和比较大小确认是否在这个ｌｅｖｅｌ，
            continue;
        else
        {
            while(curr->next[i]!=nullptr&&curr->next[i]->key<x)//在此ｌｅｖｅｌ逼近所找元素，直到next[i[]->key>＝ｘ或者遇到ｔａｉｌ//
                curr=curr->next[i];
            if(curr->next[i]!=nullptr&&curr->next[i]->key==x)
            {
                if(level==-1)//设置ｌｅｖｅｌ
                    level=i;
                prev[i]=curr;
            }
        }
    }
    if(level==-1)
        return;
    sList *temp;
    for(int i=level;i>=0;i--)
    {
        temp=prev[i]->next[i];
        prev[i]->next[i]=temp->next[i];
        delete temp->next;
        delete temp;
    }

}
int SkipList::SearchList(int x) {
    sList *curr=head;
    for(int i=MAX_LEVEL;i>=0;i--)
    {
        if(curr->next[i]== nullptr||curr->next[i]->key>x)
            continue;
        else
        {
           while(curr->next[i]!=nullptr&&curr->next[i]->key<x)
               curr=curr->next[i];
           if(curr->next[i]!=nullptr&&curr->next[i]->key==x)
               return x;
        }
    }
    return -1;
}

void SkipList::DisplayList() {
    sList *curr=head;
    while(curr->next[0]!= nullptr)
    {
        cout<<curr->next[0]->key<<" ";
        curr=curr->next[0];
    }
}
int SkipList::RandomLevel()
{
    int level=0;
    while(rand()%2&&level<MAX_LEVEL)//假设ｍａｘｌｅｖｅｌ为３，０层８，１层４，２层２，３层１//　
        ++level;
    return level;
}


int main()
{
    SkipList demo(10);
    for(int i=0;i<1000;i++)
        demo.InsertList(rand()%1000);
    demo.DeleteList(8);
    demo.DeleteList(5);
    demo.DeleteList(5);
    demo.DisplayList();
}
