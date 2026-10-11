#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structures.h"

node* search_node(int, int, node*);
void search(int, int, tree*); 
void print_node(int, node* ); //FUNÇÃO OBSOLETA

void mod_overflow_UP(node*, node*, node*);
void mod_overflow_DOWN(node*, node*, node*);
node* clone_node(node*);
void add_mod(node*, int , type_field );
void insert(node*, node*);
void insert_node(int, tree*);







#endif