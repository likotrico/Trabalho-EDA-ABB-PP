#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structures.h"

node* search_node(int, int, node*);
void search(int, int, tree*); 
node* find_mod(node *, int, type_field);
void print_order(node *, int);
void print_tree(tree*, int);

void mod_overflow_UP(node*, node*, node*);
void mod_overflow_DOWN(node*, node*, node*);
node* clone_node(node*);
void add_mod(node*, node* , type_field );
void insert(node*, node*);
void insert_node(int, tree*);

node* find_strictly_greater(int, int, node*);
int node_is_leaf(node*, int);
node* successor(int, int, tree*);

void remove(int , tree*);

#endif