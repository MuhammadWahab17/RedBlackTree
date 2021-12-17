//
//  main.cpp
//  RBT
//
//  Created bnodeB Ch Muhammad Wahab on 12/12/2021.
//  CopnodeBright © 2021 Ch Muhammad Wahab. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <stack>
#include <queue>
using namespace std;
#define red 'R';
#define black 'B';

//class for tree node
template<class t>
class TNode{
    //data members
    t data;
    char color;
    TNode*left;
    TNode*right;
    TNode*parent;
public:
    //member functions (interface)

    //constructors
    TNode():left(NULL),right(NULL),parent(NULL){};
    TNode(t val,TNode*l=NULL,TNode*r=NULL,TNode*p=NULL,
          char colour='R'):data(val),left(l),right(r),parent(p),color(colour){};

    //mutator (setter)
    void setLeftChild(TNode*node){
        left = node;
    }
    void setRightChild(TNode*node){
        right = node;
    }
    void setParent(TNode*node){
        parent = node;
    }
    void setColor(char colour){
        color = colour;
    }
    void setData(t val){
        data = val;
    }

    //accessors (getters)
    t Data(){
        return data;
    }
    char Color(){
        return color;
    }
    TNode* leftChild(){
        return left;
    }
    TNode* rightChild(){
        return right;
    }
    TNode* Parent(){
        return parent;
    }
    TNode*GrandParent(){
        if(parent == NULL)
            return NULL;
        return parent->parent;
    }
    TNode*Uncle(){
        TNode*GP = GrandParent();
        if(GP == NULL)
            return NULL;
        else if (GP->leftChild() == parent)
            return GP->rightChild();
        else
            return GP->leftChild();
    }
};

template <class t>
class RedBlackTree{
    TNode<t> *root;
    //erase function is of binary tree, implemented here just for destructor to work
    //otherwise it is not part of interface and assignment
    void erase(const t& val){
        TNode<t>*to_del=root;
        TNode<t>*prev=root;
        while(to_del!=NULL){
            //when value matches to a node
            if(val==to_del->Data()){
                //case 1 leaf node
                if(to_del->leftChild()==NULL && to_del->rightChild()==NULL){
                    if(prev->leftChild()!=NULL && prev->leftChild()->Data()==val)
                        prev->setLeftChild(NULL);
                    else if(prev->rightChild()!=NULL && prev->rightChild()->Data()==val)
                        prev->setRightChild(NULL);
                    //this is done if we have only root node exist then preva nd to_del are pointing to same address
                    //then prev right and left will be null so i freed up that memory and set root to null
                    else
                        root=NULL;
                    delete to_del;
                }
                //case 2 left child only
                else if(to_del->leftChild()==NULL){
                    if(prev->leftChild()!=NULL && prev->leftChild()->Data()==val)
                        prev->setLeftChild(to_del->rightChild());
                    else if(prev->rightChild()!=NULL && prev->rightChild()->Data()==val)
                        prev->setRightChild(to_del->rightChild());
                    //if we have root node with just one child and root node address is to be
                    //deleted then prev and to_del would be pointing to same address (ptr->data==to_del->data)
                    //so prev right and left part will be pointing to some other node
                    //so in this case i moved root according to left or right as if we reach here then it has
                    //only child either left or right (in else part root=root->right if root->left is null)
                    else{
                        if(root->leftChild()!=NULL)
                            root=root->leftChild();
                        else
                            root=root->rightChild();
                    }
                    delete to_del;
                }
                //case 3 right child only
                else if(to_del->rightChild()==NULL){
                    if(prev->leftChild()!=NULL && prev->leftChild()->Data()==val)
                        prev->setLeftChild(to_del->leftChild());
                    else if(prev->rightChild()!=NULL && prev->rightChild()->Data()==val)
                        prev->setRightChild(to_del->rightChild());
                    else{
                        if(root->leftChild()!=NULL)
                            root=root->leftChild();
                        else
                            root=root->rightChild();
                    }
                    delete to_del;
                }
                //case 3
                else{
                    TNode<t>*successor=to_del->rightChild();
                    while (successor->leftChild()!=NULL) {
                        successor=successor->leftChild();
                    }
                    t val=successor->Data();
                    erase(val);
                    to_del->setData(val);
                }
                return;
            }
            else if(val<to_del->Data()){
                prev=to_del;
                to_del=to_del->leftChild();
            }
            else{
                prev=to_del;
                to_del=to_del->rightChild();
            }
        }
        cout<<"Value doesn't exists\n";
    }
    
    //helper function to find number of nodes
    int countHelper(TNode<t>*node){
        if(node==NULL)
            return 0;
        else
            return 1+countHelper(node->leftChild())+countHelper(node->rightChild());
    }
    
    //ReColor function
    void MakeRBT(TNode<t>*node){
        if(node == NULL)
            return;
        TNode<t>*P = node->Parent();
        TNode<t>*G = node->GrandParent();
        TNode<t>*U = node->Uncle();
        if(P==NULL)
            node->setColor('B');
        else if(P->Color() == 'R'){
            if(U!=NULL && U->Color() == 'R'){
                ColorFixing(G,P,U);
                MakeRBT(G);
            }
            else
                ReStructure(node);
        }
    }
    
    //ColorFinodeAingMethod
    void ColorFixing(TNode<t>*G,TNode<t>*P,TNode<t>*U){
        P->setColor('B');
        U->setColor('B');
        if(G!=NULL)
            G->setColor('R');
    }
    
    //ReStructure Function
    void ReStructure(TNode<t>*node){
        TNode<t>*P = node->Parent();
        TNode<t>*G = node->GrandParent();
        //rightChild() rotation
        if(P == G->leftChild() && node == P->leftChild()){
            RotateRight(G);
            char tempColor = P->Color();
            P->setColor(G->Color());
            G->setColor(tempColor);
        }
        //leftChild() rotation
        else if(P == G->rightChild() && node == P->rightChild()){
            RotateLeft(G);
            char tempColor = P->Color();
            P->setColor(G->Color());
            G->setColor(tempColor);
        }
        //rightChild() leftChild() rotation
        else if(P == G->leftChild() && node == P->rightChild()){
            RotateLeft(P);
            RotateRight(G);
            char tempColor = node->Color();
            node->setColor(G->Color());
            G->setColor(tempColor);
        }
        //leftChild() rightChild() rotation
        else if(P == G->rightChild() && node == P->leftChild()){
            RotateRight(P);
            RotateLeft(G);
            char tempColor = node->Color();
            node->setColor(G->Color());
            G->setColor(tempColor);
        }
    }
    
    void  RotateRight(TNode<t>* node) {
        if (node->leftChild() == NULL)
            return;
        TNode<t>* tempNode = node->leftChild();
        node->setLeftChild(tempNode->rightChild());
        if (node->rightChild() != NULL)
            tempNode->rightChild()->setParent(node);
        tempNode->setParent(node->Parent());
        if (node == this->root)
            this->root = tempNode;
        else if (node == node->Parent()->rightChild())
            node->Parent()->setRightChild(tempNode);
        else
            node->Parent()->setLeftChild(tempNode);
        tempNode->setRightChild(node);
        node->setParent(tempNode);
    }
    void  RotateLeft(TNode<t>* node) {
        if (node->rightChild() == NULL)
            return;
        TNode<t>* tempNode = node->rightChild();
        node->setRightChild(tempNode->leftChild());
        if (node->leftChild() != NULL)
            tempNode->leftChild()->setParent(node);
        tempNode->setParent(node->Parent());
        if (node == this->root)
            this->root = tempNode;
        else if (node == node->Parent()->leftChild())
            node->Parent()->setLeftChild(tempNode);
        else
            node->Parent()->setRightChild(tempNode);
        tempNode->setLeftChild(node);
        node->setParent(tempNode);
    }

public:
    //to get number of nodes in tree
    int count(){
        return countHelper(root);
    }
    
    //constructor
    RedBlackTree():root(NULL){
        root=NULL;
    }
    
//    destructor
    ~RedBlackTree(){
//        stack will be pushed until the leaf node is entered then that is removed
        stack<TNode<t>*> s;
        if(root!=NULL)
            s.push(root);
        while(!s.empty()){
            TNode<t>*temp=s.top();
            if(temp->leftChild()!=NULL)
                s.push(temp->leftChild());
            if(temp->rightChild()!=NULL)
                s.push(temp->rightChild());
            if(temp->rightChild()==NULL&&temp->leftChild()==NULL){
                erase(temp->Data());
                s.pop();
            }
        }
    }
    
    //insert iterativelnodeB
    void insert(t val){
        TNode<t>*input = NULL;
        if(root==NULL){
            input = new TNode<t>(val,NULL,NULL,NULL,'B');
            root = input;
        }else{
            TNode<t>*temp=root;
            while(1){
                if(val<temp->Data()){
                    //as value is less then temp node's data if leftChild() part is null then we can insert value here
                    if(temp->leftChild()==NULL){
                        input =new TNode<t>(val,NULL,NULL,temp,'R');
                        temp->setLeftChild(input);
                        break;
                    }
                    //otherwise move on to leftChild() part
                    temp=temp->leftChild();
                }
                else if(val>temp->Data()){
                    //val > temp->data so if rightChild() part is null then we can insert value here
                    if(temp->rightChild()==NULL){
                        input =new TNode<t>(val,NULL,NULL,temp,'R');
                        temp->setRightChild(input);
                        break;
                    }
                    //otherwise move on to rightChild() part
                    temp=temp->rightChild();
                }
                else
                    throw "Value already exists, Duplicates are not allowed";
            }
        }
        MakeRBT(input);
    }
    //finder_iterative
    bool find(const t& val){
        TNode<t>*finder=root;
        while(finder!=NULL){
            if(val==finder->Data())
                return true;
            else if(val<finder->Data())
                finder=finder->leftChild();
            else
                finder=finder->rightChild();
        }
        return false;
    }
    
    //level-order traversal
    void LevelOrder(){
        queue<TNode<t>*> q;
        if(root!=NULL)
            q.push(root);
        while(!q.empty()){
            TNode<t>*temp=q.front();
            cout<<temp->Data()<<" ";
            q.pop();
            if(temp->leftChild()!=NULL)
                q.push(temp->leftChild());
            if(temp->rightChild()!=NULL)
                q.push(temp->rightChild());
        }
        cout<<endl;
    }
};


int testCase(){
    RedBlackTree<int> RBT;
    int value[100];

    for(int i=0;i<100;i++){
        value[i] = rand()%10000;
    }

    int node = 0;
    try{
        for (int i=0; i<100; i++){
            RBT.insert(value[i]);
            node++;
        }
    }catch(const char* except){
        cout<<except<<endl;
    }

    cout<<"Nodes inserted :"<<node<<endl;
    cout<<"Nodes in tree :"<<RBT.count()<<endl;

    if(node==RBT.count()){
        cout<<"RB-Tree formation is okay\n";
        cout<<"\nLevel-Order traversal of tree \n";
        RBT.LevelOrder();
        return 0;
    }
    cout<<"There is something wrong with tree\n";
    return 139;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    exit(testCase());
}
