#include "../headers/structures.h"

tree* create_tree(){
    tree* t = (tree*)malloc(sizeof(tree));
    int i;
    t->vector_of_roots = (node**)malloc(sizeof(node*)*maximum_versions);
    for(i=0; i<maximum_versions; i++){
        t->vector_of_roots[i] = NULL;
    }
    return t;
}

internal_info create_field(int value, type_field field){
    // = (internal_info*)malloc(sizeof(internal_info));
    internal_info f;
    f.version = version+1;
    f.value = value;
    f.field = field;
    
    return f;
}

node* create_node(int value){
    node* n;
    n = (node*)malloc(sizeof(node));
    n->value = value;
    n->version = version+1;
    n->return_pointers = (node**)malloc(sizeof(node*)*number_p);
    n->l_son = NULL;
    n->r_son = NULL;
    int i;
    for(i=0;i<number_p;i++){
        n->return_pointers[i] = NULL;
    }
    n->mods = (internal_info*)malloc(sizeof(internal_info)*2*number_p);
    for(i=0; i<2*number_p;i++){
        n->mods[i].version = -1;
        n->mods[i].value = -1;
        n->mods[i].field = EMPTY;
    }
    printf("node with value %d successfully created\n",value);
    return n;
}