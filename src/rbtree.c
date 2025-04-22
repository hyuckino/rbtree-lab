#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p -> nil = (node_t *)calloc(1, sizeof( node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->left = p->nil;
  p->nil->right = p->nil;
  p->nil->parent = p->nil;
  p->root= p->nil;
  return p;
}

static void left_rotate(rbtree *t, node_t *x){
  node_t *y = x -> right;
  x -> right = y -> left;
  if (y->left != t->nil)
    y->left->parent = x;
  y->parent = x->parent;
  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->left =x;
  x->parent = y;
}

static void right_rotate(rbtree *t, node_t *y){
  node_t *x = y -> left;
  y -> left = x -> right;
  if (x->right != t->nil)
    x->right->parent = y;
  x->parent = y->parent;
  if (y->parent == t->nil)
    t->root = x;
  else if (y == y->parent->left)
    y->parent->left = x;
  else
    y->parent->right = x;
  x->right =y;
  y->parent = x;
}

void delete_node(rbtree *t, node_t *node) {
  if (node == t->nil) return;

  delete_node(t, node->left);   // 왼쪽 서브트리 삭제
  delete_node(t, node->right);  // 오른쪽 서브트리 삭제
  free(node);                   // 자기 자신 삭제
}
void delete_rbtree(rbtree *t) {
  delete_node(t, t->root);
  free(t->nil);
  free(t);
} 

void rbtree_insert_fixup(rbtree *t, node_t *node)
{
  while (node->parent->color == RBTREE_RED){
    if (node->parent == node->parent->parent->left){
      node_t *y = node->parent->parent->right;
      if (y->color == RBTREE_RED){
        node->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent;
      }
      
      else{
        if (node == node->parent->right){
          node = node->parent;
          left_rotate(t, node);}
        node->parent->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        right_rotate(t, node->parent->parent);
          
        
      }
    }
    else{
      node_t *y = node->parent->parent->left;
      
      if (y->color == RBTREE_RED){
        node->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent;
      }
      else{
        if (node == node->parent->left){
          node = node->parent;
          right_rotate(t, node); }
        node->parent->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        left_rotate(t, node->parent->parent);
          
       
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // 새 노드 만들기
  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  new_node->key = key;
  new_node->parent = t->nil;
  //삽입 위치 찾기
  node_t *x = t->root;
  node_t *y = t->nil;

  while (x != t->nil){

    y=x;
    if (key < x->key)
      x = x->left;
    else x = x->right;
  
  }
  // new node 연결
  new_node->parent = y;
  if (y==t->nil)
    t->root = new_node;
  else if (new_node->key < y->key)
    y->left = new_node;
  else y->right = new_node;

  new_node->color = RBTREE_RED;
  new_node->left = t->nil;
  new_node->right = t->nil;

  rbtree_insert_fixup(t, new_node); 

  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *cur = t->root;
  
  while (cur != t->nil && cur->key != key){
    if (cur->key > key) cur=cur->left;
    else cur=cur->right;
   }
  if (cur == t->nil) return NULL;
  return cur;  
}

node_t *rbtree_min(const rbtree *t) {
  node_t *cur = t->root;
  if (cur == t->nil) return NULL;  // 빈 트리 처리

  while (cur->left != t->nil) {
    cur = cur->left;
  }

  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *cur = t->root;
  if (cur == t->nil) return NULL;  // 빈 트리 처리

  while (cur->right != t->nil) {
    cur = cur->right;
  }

  return cur;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){

  if (u->parent == t->nil) t->root = v;
  else if (u == u->parent->left) u->parent->left = v;
  else u -> parent -> right = v;
  v->parent = u->parent;  
}

node_t *tree_minimum(rbtree *t, node_t *z){
  while (z->left != t->nil) z= z->left;
  return z;
}
void rbtree_delete_fixup(rbtree *t, node_t *x){
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      node_t *w = x->parent->right;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color =RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color ==RBTREE_BLACK)
      {
        w->color =RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t,w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t,x->parent);
        x = t->root;
      }
    }
    else 
    {
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color =RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color ==RBTREE_BLACK)
      {
        w->color =RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t,w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t,x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *x;
  node_t *y = p;
  color_t y_original_color = y->color;

  if (p->left == t->nil){
    x = p->right;
    rbtree_transplant(t,p,p->right);
  }
  else if (p->right == t->nil){
    x = p->left;
    rbtree_transplant(t,p,p->left);
  }
  else 
  { 
    y = tree_minimum(t,p->right);
    y_original_color = y->color;
    x = y->right;
    if (y != p->right)
    {
      rbtree_transplant(t,y,y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    else {
      x->parent = y;
      
    }
    rbtree_transplant(t,p,y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if (y_original_color == RBTREE_BLACK) 
  {rbtree_delete_fixup(t, x);
  }
  return 0;
}

int inorder_traverse(const rbtree *t, node_t *node, key_t *arr, int index, const size_t n) {
  if (node == t->nil || index >= n) return index;

  // 왼쪽 서브트리 순회
  index = inorder_traverse(t, node->left, arr, index, n);

  // 현재 노드 저장
  if (index < n) {
    arr[index++] = node->key;
  }

  // 오른쪽 서브트리 순회
  index = inorder_traverse(t, node->right, arr, index, n);

  return index;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  inorder_traverse(t, t->root, arr, 0, n);
  return 0;
}