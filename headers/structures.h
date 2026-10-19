#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>

int const number_p = 1;
int version = 0;
int const maximum_versions = 1001;
std::vector<int> queue;

enum type_field{
    EMPTY,
    L_SON,
    R_SON
};

struct node;

struct internal_info{
    int version;
    node *value;
    int field;
} ;

struct node{ 
    int value;
    int version;
    node *l_son = NULL;
    node *r_son = NULL;
    node *return_pointers[number_p];
    internal_info mods[number_p*2];
};

struct tree{
    node *vector_of_roots[maximum_versions];
};

tree* create_tree();
internal_info create_mod(node*, type_field);
node* create_node(int);

#endif