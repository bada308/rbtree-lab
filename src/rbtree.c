
#include "rbtree.h"
#include <stdlib.h>

/////////////////////////////////////////

/**
 * @brief 주어진 노드 x를 기준으로 트리를 왼쪽 회전시키는 함수
 *
 * @param t 대상이 되는 레드블랙 트리
 * @param x 회전 기준 노드
 */
void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right; // y 정의

  // step1. 회전 시 갈곳을 잃은 y의 왼쪽 자식을 x에 연결하자
  x->right = y->left; // x의 오른쪽에 y의 왼쪽 자식 연결

  if (y->left != t->nil) // 만약, y의 왼쪽 자식이 nil 노드가 아니라면
  {
    y->left->parent = x; // y의 왼쪽 자식의 부모를 x로 변경
  }

  // step2. y를 서브 트리의 루트로 만들자
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

  // step3. x와 y의 관계 역전시키자
  y->left = x;   // y의 왼쪽 자식을 x로 변경
  x->parent = y; // x의 부모를 y로 변경
}

/**
 * @brief 주어진 노드 x를 기준으로 트리를 오른쪽 회전시키는 함수
 *
 * @param t 대상이 되는 레드블랙 트리
 * @param x 회전 기준 노드
 */
void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left; // y 정의

  // step1. 회전 시 갈곳을 잃은 y의 오른쪽 자식을 x에 연결하자
  x->left = y->right;

  if (y->right != t->nil)
  {
    y->right->parent = x;
  }

  // step2. y를 서브 트리의 루트로 만들자
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

  // step3. x와 y의 관계 역전시키자
  y->right = x;
  x->parent = y;
}

/**
 * @brief 레드블랙 트리에 노드 삽입 후 재조정하는 함수
 *
 * @param t 대상이 되는 레드블랙 트리
 * @param target 재조정이 필요한 노드
 */
void insert_fixup(rbtree *t, node_t *target)
{
  // target의 부모가 Red이면 재조정 (#4 위반)
  while (target->parent->color == RBTREE_RED)
  {
    node_t *grand_parent = target->parent->parent;

    if (target->parent == grand_parent->left) // 부모가 할아버지의 왼쪽 자식일 때
    {
      node_t *uncle = grand_parent->right;

      // 삼촌이 RED (Case 1)
      if (uncle->color == RBTREE_RED)
      {
        target->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        target = grand_parent;
      }
      else
      {
        // target이 부모의 오른쪽 자식 (Case 2)
        if (target == target->parent->right)
        {
          target = target->parent;
          left_rotate(t, target);
        }

        // target이 부모의 왼쪽 자식 (Case 3)
        target->parent->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        right_rotate(t, grand_parent);
      }
    }
    else // target의 부모가 할아버지의 오른쪽 자식일 때
    {
      node_t *uncle = grand_parent->left;

      // 삼촌이 RED (Case 1)
      if (uncle->color == RBTREE_RED)
      {
        grand_parent->color = RBTREE_RED;
        target->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        target = grand_parent;
      }
      else
      {
        // target이 부모의 왼쪽 자식 (Case 2)
        if (target == target->parent->left)
        {
          target = target->parent;
          right_rotate(t, target);
        }

        // target이 부모의 오른쪽 자식 (Case 3)
        target->parent->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        left_rotate(t, grand_parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

/**
 * @brief 레드블랙 트리에 노드 삭제 후 재조정하는 함수
 *
 * @param t 대상이 되는 레드블랙 트리
 * @param target 재조정이 필요한 노드
 */
void erase_fixup(rbtree *t, node_t *target)
{
  while (1)
  {
    // target이 root면 종료
    if (target == t->root)
      return;

    // target이 red_and_black인 경우
    if (target->color == RBTREE_RED)
    {
      target->color = RBTREE_BLACK;
      return;
    }

    // target이 doubly black인 경우
    node_t *bro;
    if (target == target->parent->left) // doubly black이 왼쪽 자식
    {
      bro = target->parent->right;

      if (bro->color == RBTREE_RED) // [Case 1] 형제가 Red
      {
        // 부모와 형제의 color swap
        target->parent->color = RBTREE_RED;
        bro->color = RBTREE_BLACK;
        // 부모를 기준으로 회전
        left_rotate(t, target->parent);
      }
      else if (bro->right->color == RBTREE_RED) // [Case 4] 형제는 Black, 형제의 오른쪽 자식은 Red
      {
        // 형제 -> 부모.color / 부모 & 형제.right -> black
        bro->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        bro->right->color = RBTREE_BLACK;
        // 부모를 기준으로 회전
        left_rotate(t, target->parent);
        return;
      }
      else if (bro->left->color == RBTREE_RED) // [Case 3] 형제는 Black, 형제의 왼쪽 자식은 Red & 오른쪽 자식은 Black
      {
        // 형제와 형제.left의 color swap 후 형제를 기준으로 회전
        // case 4로 이동
        bro->color = RBTREE_RED;
        bro->left->color = RBTREE_BLACK;
        right_rotate(t, bro);
      }
      else // [Case 2] 형제는 Black, 형제의 자식들 모두 Black
      {
        // black을 부모로 위임하고 부모에서부터 다시 시작
        bro->color = RBTREE_RED;
        target = target->parent;
      }
    }
    else // 위 분기에서 left <-> right
    {
      bro = target->parent->left;

      if (bro->color == RBTREE_RED) // Case 1
      {
        bro->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        right_rotate(t, target->parent);
      }
      else if (bro->left->color == RBTREE_RED) // Case 4
      {
        bro->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        bro->left->color = RBTREE_BLACK;
        right_rotate(t, target->parent);
        return;
      }
      else if (bro->right->color == RBTREE_RED) // Case 3
      {
        bro->color = RBTREE_RED;
        bro->right->color = RBTREE_BLACK;
        left_rotate(t, bro);
      }
      else // Case 2
      {
        bro->color = RBTREE_RED;
        target = target->parent;
      }
    }
  }
}

/**
 * @brief 레드블랙 트리에서 x 노드의 위치를 y 노드로 대체하는 함수
 *
 * @param t 대상이 되는 레드블랙 트리
 * @param x 대체될 노드
 * @param y x의 위치에 들어갈 노드
 */
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

/**
 * @brief 레드블랙 트리의 노드들의 메모리를 반환하는 재귀함수
 *
 * @param t 대상이 되는 레드블랙 트리
 */
void free_rbtree(rbtree *t)
{
  // 트리가 비어있다면 함수 종료
  if (t->root == t->nil)
  {
    return;
  }

  // 서브트리를 위한 새로운 레드블랙 트리 생성
  rbtree *subtree = (rbtree *)calloc(1, sizeof(rbtree));
  subtree->nil = t->nil;

  // 왼쪽/오른쪽 서브트리가 있으면 재귀적으로 메모리 반환

  if (t->root->left != t->nil)
  {
    subtree->root = t->root->left;
    free_rbtree(subtree);
  }

  if (t->root->right != t->nil)
  {
    subtree->root = t->root->right;
    free_rbtree(subtree);
  }

  // 루트 노드와 서브트리를 위해 할당한 메모리 반환
  free(t->root);
  free(subtree);
}

/**
 * @brief 레드블랙 트리를 재귀적으로 순회하며 arr에 값을 추가하는 함수
 *
 * @param t 순회할 레드블랙 트리
 * @param arr 순회한 결과를 저장할 배열
 * @param index 값을 추가할 배열의 인덱스
 * @param n 배열의 총 크기
 */
void inorder(const rbtree *t, key_t *arr, int *index, const size_t n)
{
  // 트리가 비어있으면 종료
  if (t->root == t->nil)
    return;

  // 배열이 가득찼으면 종료
  if (*index == n)
    return;

  rbtree *subtree = (rbtree *)calloc(1, sizeof(rbtree));

  // 왼쪽 서브트리 재귀적으로 순회
  subtree->nil = t->nil;
  subtree->root = t->root->left;
  inorder(subtree, arr, index, n);

  // 루트 순회
  if (*index == n)
    return;
  arr[*index] = t->root->key;
  *index += 1;

  // 오른쪽 서브트리 재귀적으로 순회
  subtree->nil = t->nil;
  subtree->root = t->root->right;
  inorder(subtree, arr, index, n);

  free(subtree);
}

/////////////////////////////////////////

/**
 * @brief 레드블랙 트리를 생성하는 함수
 *
 * @return rbtree*
 */
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

/**
 * @brief 레드블랙 트리를 삭제하고 관련된 모든 메모리를 해제하는 함수
 *
 * @param t 대상이 되는 레드블랙 트리
 */
void delete_rbtree(rbtree *t)
{
  // 트리의 모든 노드를 순회하며 각 노드에 대한 메모리 해제
  free_rbtree(t);

  // 트리의 nil 노드에 대한 메모리 해제
  free(t->nil);

  // 트리 자체를 위해 할당된 메모리 해제
  free(t);
}

/**
 * @brief 레드블랙 트리에 주어진 key 값을 가진 노드를 삽입하는 함수
 *
 * @param t key를 삽입할 레드블랙 트리
 * @param key 삽입하고자 하는 key 값
 * @return node_t* 삽입한 노드 반환
 */
node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // 삽입할 노드를 위한 메모리 할당
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  new_node->key = key;
  new_node->color = RBTREE_RED;
  new_node->left = t->nil;
  new_node->right = t->nil;

  node_t *parent = t->nil; // 삽입하는 노드의 부모가 될 노드
  node_t *cur = t->root;   // 노드를 삽입할 위치

  // 노드를 삽입할 위치 찾기
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

  // 삽입할 노드와 부모 연결시키기
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

  // 삽입 후 재조정
  insert_fixup(t, new_node);
  return new_node;
}

/**
 * @brief 레드블랙 트리에서 주어진 key 값을 찾는 함수
 *
 * @param t 검색할 레드블랙 트리
 * @param key 찾고자 하는 key 값
 * @return node_t* key 값에 해당하는 노드 반환. 만약 key 값이 트리에 없다면 NULL 반환
 */
node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t *current = t->root;

  // key 값에 해당하는 노드 찾기
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

  // 존재하지 않으면 NULL 반환
  return NULL;
}

/**
 * @brief 주어진 레드블랙 트리의 최소값 찾기
 *
 * @param t 대상이 되는 레드블랙 트리
 * @return node_t* 최소값을 가진 노드 반환
 */
node_t *rbtree_min(const rbtree *t)
{
  node_t *current = t->root;

  while (current->left != t->nil)
  {
    current = current->left;
  }

  return current;
}

/**
 * @brief 주어진 레드블랙 트리의 최대값 찾기
 *
 * @param t 대상이 되는 레드블랙 트리
 * @return node_t* 최대값을 가진 노드 반환
 */
node_t *rbtree_max(const rbtree *t)
{
  node_t *current = t->root;

  while (current->right != t->nil)
  {
    current = current->right;
  }

  return current;
}

/**
 * @brief 레드블랙 트리에서 주어진 노드를 삭제하는 함수
 *
 * @param t 노드를 삭제할 레드블랙 트리
 * @param p 삭제할 노드
 * @return int
 */
int rbtree_erase(rbtree *t, node_t *p)
{
  node_t *temp = p;
  color_t del_color = temp->color;
  node_t *replaced;

  if (temp->left == t->nil) // 왼쪽 자식이 없는 경우
  {
    replaced = temp->right;
    transplant(t, temp, temp->right);
  }
  else if (temp->right == t->nil) // 오른쪽 자식이 없는 경우
  {
    replaced = temp->left;
    transplant(t, temp, temp->left);
  }
  else // 자식이 2개 있는 경우
  {
    // 후임자 찾기
    rbtree *subtree = (rbtree *)calloc(1, sizeof(rbtree));
    subtree->root = p->right;
    subtree->nil = t->nil;
    temp = rbtree_min(subtree);
    free(subtree);

    del_color = temp->color;
    replaced = temp->right;

    p->key = temp->key;
    transplant(t, temp, temp->right);
  }

  // 삭제한 노드의 색상이 Black일 경우 재조정
  if (del_color == RBTREE_BLACK)
  {
    erase_fixup(t, replaced);
  }

  // 삭제한 노드에 대한 메모리 반환
  free(temp);
  return 0;
}

/**
 * @brief 레드블랙 트리에 저장된 값을 크기 n의 배열에 저장하는 함수
 *
 * @param t 대상이 되는 레드블랙 트리
 * @param arr 값을 저장할 배열
 * @param n 배열의 크기
 * @return int
 */
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  int *index = (int *)calloc(1, sizeof(int));
  *index = 0;
  // 중위 순회
  inorder(t, arr, index, n);
  free(index);
  return 0;
}