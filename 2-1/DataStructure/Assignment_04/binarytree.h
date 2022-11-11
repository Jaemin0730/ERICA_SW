#pragma once
#include <stdio.h>
#include <stdbool.h>

typedef struct BinaryTree
{
	int data;
	struct BinaryTree* lchild;
	struct BinaryTree* rchild;
}BinaryTree;

BinaryTree* bt_create(int data);

bool bt_is_empty(BinaryTree* bt);

BinaryTree* bt_make(BinaryTree* root, BinaryTree* bt1, BinaryTree* bt2);

void bt_print_preorder(BinaryTree* bt);
void bt_print_postorder(BinaryTree* bt);
void bt_print_inorder(BinaryTree* bt);