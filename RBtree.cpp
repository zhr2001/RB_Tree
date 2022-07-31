#include <iostream>
#include <assert.h>
#include <queue>
using namespace std;

typedef enum node_type{
    red,
    black
} n_type;

typedef enum location_type{
    LL,
    LR,
    RL,
    RR
} l_type;

typedef struct RB_Node{
    int val;
    n_type type;

    RB_Node *p;
    RB_Node *l_ch;
    RB_Node *r_ch;
} RB_Node;

RB_Node *root = nullptr;

void left_rot(RB_Node *target){
    assert(target->p != NULL && target == target->p->r_ch);
    RB_Node *tmp = target->p;
    tmp->r_ch = target->l_ch;
    target->p = tmp->p;
    if (tmp->p != NULL)
        if (tmp == tmp->p->l_ch) tmp->p->l_ch = target;
        else tmp->p->r_ch = target;
    target->l_ch = tmp;
    tmp->p = target;
}

void right_rot(RB_Node *target){
    assert(target->p != NULL && target == target->p->l_ch);
    RB_Node *tmp = target->p;
    tmp->l_ch = target->r_ch;
    target->p = tmp->p;
    if (tmp->p != NULL)
        if (tmp == tmp->p->l_ch) tmp->p->l_ch = target;
        else tmp->p->r_ch = target;
    target->r_ch = tmp;
    tmp->p = target;
}

void LL_Handle(RB_Node *target){
    RB_Node *tmp = target->p;
    right_rot(tmp);
    tmp->type = black;
    tmp->r_ch->type = red;
}

void LR_Handle(RB_Node *target){
    left_rot(target);
    right_rot(target);
    target->type = black;
    target->r_ch->type = red;
}

void RL_Handle(RB_Node *target){
    right_rot(target);
    left_rot(target);
    target->type = black;
    target->l_ch->type = red;
}

void RR_Handle(RB_Node *target){
    RB_Node *tmp = target->p;
    left_rot(tmp);
    tmp->type = black;
    tmp->l_ch->type = red;
}

RB_Node *getUncle(RB_Node *node) {
    assert(node->p != NULL && node->p->p != NULL);
    RB_Node *pparent = node->p->p;
    RB_Node *parent = node->p;
    return parent == pparent->l_ch ? pparent->r_ch : pparent->l_ch;
}

bool isUncleBlack(RB_Node *node){
    assert(node->p != NULL && node->p->p != NULL);
    RB_Node *pparent = node->p->p;
    RB_Node *parent = node->p;
    if (parent == pparent->l_ch) {
        return pparent->r_ch != NULL ? pparent->r_ch->type == black : true;
    } else {
        return pparent->l_ch != NULL ? pparent->l_ch->type == black : true;
    }
}

l_type getLocationType(RB_Node *node) {
    assert(node->p != NULL && node->p->p != NULL);
    RB_Node *pparent = node->p->p;
    RB_Node *parent = node->p;
    if (node == parent->l_ch) {
        if (parent == pparent->l_ch) return LL;
        else return RL;
    } else {
        if (parent == pparent->l_ch) return LR;
        else return RR;
    }
}

void insertNode(RB_Node *root, RB_Node *node) {
    RB_Node *tmp = root;
    if (tmp == nullptr) {
        root = node;
        return ;
    }
    if (tmp->val == node->val) return ;
    else if (tmp->val < node->val) {
        if (tmp->r_ch == nullptr) {
            tmp->r_ch = node;
            node->p = tmp;
            return ;
        } else {
            insertNode(tmp->r_ch, node);
        }
    } else {
        if (tmp->l_ch == NULL) {
            tmp->l_ch = node;
            node->p = tmp;
            return ;
        } else {
            insertNode(tmp->l_ch, node);
        }
    }
}

void adjust(RB_Node *node) {
    if (node->p == NULL){ 
        root = node;
        node->type = black;
        return ;
    }
    if (node->p->type == black) return;

    if (isUncleBlack(node)){
        switch (getLocationType(node)) {
            case LL:
                if (node->p->p == root) root = node->p;
                LL_Handle(node);
                break;
            case LR:
                if (node->p->p == root) root = node;
                LR_Handle(node);
            case RL:
                if (node->p->p == root) root = node;
                RL_Handle(node);
                break;
            case RR:
                if (node->p->p == root) root = node->p;
                RR_Handle(node);
                break;
        }
    }
    else {
        getUncle(node)->type = black;
        node->p->type = black;
        node->p->p->type = red;
        adjust(node->p->p);
    }
}

void printTree() {
    queue<pair<RB_Node*, int>> q;
    q.push(make_pair(root, 0));
    int now = 0;
    while (q.size())
    {
        pair<RB_Node* ,int> tmp = q.front();
        q.pop();
        if (tmp.second != now) {
            cout << endl;
            now = tmp.second;
        } 
        cout << tmp.first->val << " "; 
        if (tmp.first->type == red) cout << "red" << " ";
        else cout << "black" << " ";
        if (tmp.first->l_ch) q.push(make_pair(tmp.first->l_ch, tmp.second+1));
        if (tmp.first->r_ch) q.push(make_pair(tmp.first->r_ch, tmp.second+1));       
    }
    cout << endl << "-----------------------------" << endl;
}

int main() {
    int val;
    while (1) {
        cin >> val;
        RB_Node *new_node = (RB_Node *)malloc(sizeof(RB_Node));
        new_node->val = val;
        new_node->type = red;
        new_node->l_ch = NULL;
        new_node->r_ch = NULL;
        new_node->p = NULL;
        insertNode(root, new_node);
        adjust(new_node);
        printTree();
    }
}