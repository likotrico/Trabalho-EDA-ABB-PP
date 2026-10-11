#ifndef STRUCTURES_H
#define STRUCTURES_H

int number_p = 1;
int next_id = 0;
int version = 0;
int maximum_versions = 50;

enum type_field{
    EMPTY,
    //VALOR,
    L_SON,
    R_SON
};

struct internal_info{
    int version;
    int value;
    int field;
} ;

struct node{ 
    int value;
    int version;
    node *l_son = NULL;
    node *r_son = NULL;
    node **return_pointers;
    internal_info* mods;
};

struct tree{
    node** vector_of_roots;
};

tree* create_tree();
internal_info create_field(int, type_field);
node* create_node(int );

#endif