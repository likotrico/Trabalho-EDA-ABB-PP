#include "../headers/insert.h"
#include "../headers/functions.h"

node* insert(node *new_node, node *current_node, tree *current_tree){
    node *aux_node = read_node(current_node, version);
    node *return_node;

    if(new_node->value <= current_node->value){
        if(aux_node->l_son == NULL){
            new_node->return_pointers[0] = current_node;
            return_node = add_mod(current_node, new_node ,L_SON, current_tree); //codar ainda

        }
        else return_node = insert(new_node, aux_node->l_son, current_tree);
    }
    else{
        if(aux_node->r_son == NULL){
            new_node->return_pointers[0] = current_node;
            return_node = add_mod(current_node, new_node ,R_SON, current_tree); //codar ainda
        }
        else return_node = insert(new_node, aux_node->r_son, current_tree);
    }
    clear_node(aux_node);
    return return_node;
}

void insert_node(int value, tree *current_tree){
    if(value <= 0) printf("Negative value can't be insert!\n");
    else if(version+1 >= maximum_versions) printf("Maximum number of versions!\n");
        
    else{
        node *n = create_node(value);
        current_tree->vector_of_roots[version+1] = current_tree->vector_of_roots[version];
        
        //INSERINDO A RAIZ
        if(current_tree->vector_of_roots[version+1] == NULL){
            current_tree->vector_of_roots[version+1] = n;
        }
        //INSERINDO UM NÃO RAIZ
        else{
            insert(n, current_tree->vector_of_roots[version+1], current_tree);
        }
        version++;
        printf("node with value %d was sucessfully added in version %d\n", value, version);
    }
}