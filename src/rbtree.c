
#include "rbtree.h"
#include <stdlib.h>

/////////////////////////////////////////
void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right; // y 정의

  //////////////////////////////////////////////////////////
  ////// step1. 회전 시 갈곳을 잃은 y의 왼쪽 자식을 x에 연결하자 //////
  //////////////////////////////////////////////////////////

  x->right = y->left; // x의 오른쪽에 y의 왼쪽 자식 연결

  if (y->left != t->nil) // 만약, y의 왼쪽 자식이 nil 노드가 아니라면
  {
    y->left->parent = x; // y의 왼쪽 자식의 부모를 x로 변경
  }

  ///////////////////////////////////////////
  ////// step2. y를 서브 트리의 루트로 만들자 //////
  ///////////////////////////////////////////

  y->parent = x->parent; // y의 부모를 x의 부모로 변경

  if (x->parent == t->nil) // 만약, x가 트리의 루트였으면
  {
    t->root = y; // 트리의 루트를 y로 변경
  }
  else if (x == x->parent->left) // 만약, x가 부모의 왼쪽 자식이었다면
  {
    x->parent->left = y; // x의 부모의 왼쪽 자식을 y로 변경
  }
  else // 만약, x가 부모의 오른쪽 자식이었다면
  {
    x->parent->right = y; // x의 부모의 오른쪽 자식을 y로 변경
  }

  ///////////////////////////////////////
  ////// step3. x와 y의 관계 역전시키자 //////
  ///////////////////////////////////////

  y->left = x;   // y의 왼쪽 자식을 x로 변경
  x->parent = y; // x의 부모를 y로 변경
}

void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left; // y 정의

  //////////////////////////////////////////////////////////
  ////// step1. 회전 시 갈곳을 잃은 y의 오른쪽 자식을 x에 연결하자 //////
  //////////////////////////////////////////////////////////
  x->left = y->right;

  if (y->right != t->nil)
  {
    y->right->parent = x;
  }

  ///////////////////////////////////////////
  ////// step2. y를 서브 트리의 루트로 만들자 //////
  ///////////////////////////////////////////

  y->parent = x->parent;

  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }

  ///////////////////////////////////////
  ////// step3. x와 y의 관계 역전시키자 //////
  ///////////////////////////////////////
  y->right = x;
  x->parent = y;
}

void insert_fixup(rbtree *t, node_t *target)
{
  while (target->parent->color == RBTREE_RED)
  {
    node_t *grand_parent = target->parent->parent;
    if (target->parent == grand_parent->left) // 부모가 할아버지의 왼쪽 자식일 때
    {
      node_t *uncle = grand_parent->right;
      if (uncle->color == RBTREE_RED) // 삼촌이 RED일 때 (Case 1)
      {
        target->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        target = grand_parent;
      }
      else
      {
        if (target == target->parent->right) // target이 부모의 오른쪽 자식일 때 (Case 2)
        {
          target = target->parent;
          left_rotate(t, target);
        }
        // target이 부모의 왼쪽 자식일 때 (Case 3)
        target->parent->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        right_rotate(t, grand_parent);
      }
    }
    else // target의 부모가 할아버지의 오른쪽 자식일 때
    {
      node_t *uncle = grand_parent->left;
      if (uncle->color == RBTREE_RED) // 삼촌이 RED (Case 1)
      {
        grand_parent->color = RBTREE_RED;
        target->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        target = grand_parent;
      }
      else
      {
        if (target == target->parent->left) // target이 부모의 왼쪽 자식일 때 (Case 2)
        {
          target = target->parent;
          right_rotate(t, target);
        }

        // target이 부모의 오른쪽 자식일 때 (Case 3)
        target->parent->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        left_rotate(t, grand_parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

void erase_fixup(rbtree *t, node_t *target)
{
  while (1)
  {
    // 재조정 대상이 root
    if (target == t->root)
      return;

    if (target->color == RBTREE_RED) // red_and_black 생성
    {
      target->color = RBTREE_BLACK;
      return;
    }

    // doubly black 생성
    node_t *bro;
    if (target == target->parent->left) // doubly black이 왼쪽 자식
    {
      bro = target->parent->right;

      if (bro->color == RBTREE_RED) // 형제가 Red (Case 1)
      {
        // 부모와 형제의 color swap
        target->parent->color = RBTREE_RED;
        bro->color = RBTREE_BLACK;
        // 부모를 기준으로 회전
        left_rotate(t, target->parent);
      }
      else if (bro->right->color == RBTREE_RED) // 형제는 Black, 형제의 오른쪽 자식은 Red (Case 4)
      {
        // 형제 -> 부모.color / 부모 & 형제.right -> black
        bro->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        bro->right->color = RBTREE_BLACK;
        // 부모를 기준으로 회전
        left_rotate(t, target->parent);
        return;
      }
      else if (bro->left->color == RBTREE_RED) // 형제는 Black, 형제의 왼쪽 자식은 Red & 오른쪽 자식은 Black (Case 3)
      {
        // 형제와 형제.left의 color swap 후 형제를 기준으로 회전
        // case 4로 이동
        bro->color = RBTREE_RED;
        bro->left->color = RBTREE_BLACK;
        right_rotate(t, bro);
      }
      else // 형제는 Black, 형제의 자식들 모두 Black (Case 2)
      {
        // black을 부모로 위임하고 부모에서부터 다시 시작
        bro->color = RBTREE_RED;
        target = target->parent;
      }
    }
    else // 위 분기에서 left <-> right
    {
      bro = target->parent->left;

      if (bro->color == RBTREE_RED)
      {
        bro->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        right_rotate(t, target->parent);
      }
      else if (bro->left->color == RBTREE_RED)
      {
        bro->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        bro->left->color = RBTREE_BLACK;
        right_rotate(t, target->parent);
        return;
      }
      else if (bro->right->color == RBTREE_RED)
      {
        bro->color = RBTREE_RED;
        bro->right->color = RBTREE_BLACK;
        left_rotate(t, bro);
      }
      else
      {
        bro->color = RBTREE_RED;
        target = target->parent;
      }
    }
  }
}

void transplant(rbtree *t, node_t *x, node_t *y)
{
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->parent = x->parent;
}

void free_rbtree(rbtree *t)
{
  if (t->root == t->nil)
  {
    return;
  }

  rbtree *subtree = (rbtree *)calloc(1, sizeof(rbtree));
  subtree->nil = t->nil;

  // left
  if (t->root->left != t->nil)
  {
    subtree->root = t->root->left;
    free_rbtree(subtree);
  }

  // right
  if (t->root->right != t->nil)
  {
    subtree->root = t->root->right;
    free_rbtree(subtree);
  }

  // 루트 및 서브트리 free
  free(t->root);
  free(subtree);
}

/////////////////////////////////////////

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
  NIL->color = RBTREE_BLACK;
  NIL->key = -1;
  NIL->left = NULL;
  NIL->parent = NULL;
  NIL->right = NULL;

  p->nil = NIL;
  p->root = NIL;

  return p;
}

void delete_rbtree(rbtree *t)
{
  // 트리 순회하며 node들 free 해주기
  free_rbtree(t);

  // 트리의 nil 노드 free
  free(t->nil);

  // 트리 자체의 구조체 free
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  node_t *parent = t->nil;
  node_t *cur = t->root;
  new_node->key = key;

  while (cur != t->nil)
  {
    parent = cur;
    if (cur->key >= key)
    {
      cur = cur->left;
    }
    else
    {
      cur = cur->right;
    }
  }

  new_node->parent = parent;

  if (parent == t->nil)
  {
    t->root = new_node;
  }
  else if (parent->key >= key)
  {
    parent->left = new_node;
  }
  else
  {
    parent->right = new_node;
  }

  new_node->color = RBTREE_RED;
  new_node->left = t->nil;
  new_node->right = t->nil;

  insert_fixup(t, new_node);
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t *current = t->root;

  while (current != t->nil)
  {
    if (current->key == key)
    {
      return current;
    }
    else if (current->key > key)
    {
      current = current->left;
    }
    else
    {
      current = current->right;
    }
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t)
{
  node_t *current = t->root;

  while (current->left != t->nil)
  {
    current = current->left;
  }

  return current;
}

node_t *rbtree_max(const rbtree *t)
{
  node_t *current = t->root;

  while (current->right != t->nil)
  {
    current = current->right;
  }

  return current;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  node_t *temp = p;
  color_t del_color = temp->color;
  node_t *replaced;

  if (temp->left == t->nil)
  {
    replaced = temp->right;
    transplant(t, temp, temp->right);
  }
  else if (temp->right == t->nil)
  {
    replaced = temp->left;
    transplant(t, temp, temp->left);
  }
  else
  {
    // 후임자 찾기
    rbtree *subtree = (rbtree *)calloc(1, sizeof(rbtree));
    subtree->root = p->right;
    subtree->nil = t->nil;
    temp = rbtree_min(subtree);
    free(subtree);

    del_color = temp->color;
    replaced = temp->right;
    // replaced->parent = temp->parent;

    p->key = temp->key;
    transplant(t, temp, temp->right);
  }

  if (del_color == RBTREE_BLACK)
  {
    erase_fixup(t, replaced);
  }

  free(temp);
  return 0;
}

void inorder(const rbtree *t, key_t *arr, int *index, const size_t n)
{
  if (t->root == t->nil)
    return;

  if (*index == n)
    return;

  rbtree *subtree = (rbtree *)calloc(1, sizeof(rbtree));

  subtree->nil = t->nil;
  subtree->root = t->root->left;
  inorder(subtree, arr, index, n);

  if (*index == n)
    return;
  arr[*index] = t->root->key;
  *index += 1;

  subtree->nil = t->nil;
  subtree->root = t->root->right;
  inorder(subtree, arr, index, n);

  free(subtree);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  int *index = (int *)calloc(1, sizeof(int));
  *index = 0;
  // 중위 순회
  inorder(t, arr, index, n);
  free(index);
  return 0;
}
