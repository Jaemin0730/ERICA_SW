#include "binarytree.h"
#include <stdlib.h>

BinaryTree* bt_create(int data)
{
    BinaryTree *new = (BinaryTree *)malloc(sizeof(BinaryTree));
    new->data = data;
    new->lchild = NULL;
    new->rchild = NULL;
    return new;
}

bool bt_is_empty(BinaryTree* bt)
{
    if (bt == NULL) {
        return true;
    }
    else {
        return false;
    }
}

BinaryTree* bt_make(BinaryTree* root, BinaryTree* bt1, BinaryTree* bt2)
{
    root->lchild = bt1;
    root->rchild = bt2;
    return root;
}

void bt_print_preorder(BinaryTree* bt)
{
    if (bt != NULL) {
        printf("-> %d ", bt->data);
        bt_print_preorder(bt->lchild);
        bt_print_preorder(bt->rchild);
    }
}

void bt_print_postorder(BinaryTree* bt)
{
    if (bt != NULL) {
        bt_print_postorder(bt->lchild);
        bt_print_postorder(bt->rchild);
        printf("-> %d ", bt->data);
    }
}

void bt_print_inorder(BinaryTree* bt)
{
    if (bt != NULL) {
        bt_print_inorder(bt->lchild);
        printf("-> %d ", bt->data);
        bt_print_inorder(bt->rchild);
    }
}