#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structures.h"

//node* search_node(int, int, node*);
//void search(int, int, tree*); 
//void print_node(int, node* ); //FUNÇÃO OBSOLETA

//void mod_overflow_UP(node*, node*, node*);
//void mod_overflow_DOWN(node*, node*, node*);
//node* clone_node(node*);

//void insert(node*, node*);
//void insert_node(int, tree*);


//node* add_mod(node*, node* , type_field);


node* read_node(node*, int);
void clear_node(node*);

void mod_overflow_DOWN(node*, node*, node*);
node *mod_overflow_UP(node*, node*, node*, tree*);
node* add_mod(node*, node*, type_field, tree*);

void print_order(node *, int);
void print_order_tree(tree*, int);

int node_is_leaf(node*);

node *search_node(int, int, tree*);

#endif