#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// put char into a newly created node
Node * create_node(char ch) {
  Node * node = (Node *) malloc(sizeof(Node));
  assert(node); //confirm malloc didn't fail

  node->data = ch;
  node->next = NULL;
  return node;
}

// output the list iteratively
void print(const Node * cur) {
  while (cur != NULL) { //or if (cur)
    printf("%c ", cur->data);
    cur = cur->next;  // advance to next node
  }
}

int length(const Node * head) {
  int count = 0;
  while (head != NULL) {
    count++;
    head = head->next;
  }
  return count;
}

void add_after(Node * node, char val) {
  Node * nextNode = create_node(val);
  nextNode->next = node->next;
  node->next = nextNode;
}

void reverse_print(const Node * cur) {
  if (cur->next) {
    reverse_print(cur->next);
  }
  printf("%c ", cur->data);
}