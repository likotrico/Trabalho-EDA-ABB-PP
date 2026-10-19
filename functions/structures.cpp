#include "../headers/structures.h"

//CRIA A ESTRUTURA ÁRVORE
tree* create_tree(){
    tree* t = (tree*)malloc(sizeof(tree));
    int i;
    for(i=0; i<maximum_versions; i++){
        t->vector_of_roots[i] = NULL;
    }
    return t;
}

//CRIA A ESTRUTURA INTERNA DA MODIFICAÇÃO
internal_info create_mod(node *value, type_field field){
    internal_info f;
    f.version = version+1;
    f.value = value;
    f.field = field;
    
    return f;
}

//CRIA UM NOVO NÓ
node* create_node(int value){
    int i;
    node *n;
    n = (node*)malloc(sizeof(node));
    n->value = value;
    n->version = version+1;
    n->l_son = NULL;
    n->r_son = NULL;

    for(i=0;i<number_p;i++){
        n->return_pointers[i] = NULL;
    }

    for(i=0; i<2*number_p;i++){
        n->mods[i].version = -1;
        n->mods[i].value = NULL;
        n->mods[i].field = EMPTY;
    }

    return n;
}