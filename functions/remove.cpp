#include "../headers/remove.h"
#include "../headers/functions.h"
#include "../headers/sucessor.h"

node* add_unknown_mod(node *current_node, node *new_node, node *old_node, tree *current_tree){
    node *n = read_node(current_node, version);
    if(n->l_son == old_node) return add_mod(current_node, new_node, L_SON, current_tree);
    else return add_mod(current_node, new_node, R_SON, current_tree);
}

int has_only_son(node *current_node, int version_req){
    node *aux = read_node(current_node, version_req);
    if((aux->l_son == NULL && aux->r_son != NULL) || (aux->l_son != NULL && aux->r_son == NULL)) return 1;
    return 0;
}

node* add_unknown_mod_to_remove(node *current_node, node *new_node, node *old_node, node* node_to_stop, tree *current_tree){
    if(current_node == node_to_stop) return new_node;
    node *n = read_node(current_node, version);
    if(n->l_son == old_node) return add_mod_to_remove(current_node, new_node, L_SON, node_to_stop,current_tree);
    else return add_mod_to_remove(current_node, new_node, R_SON, node_to_stop, current_tree);
}

node* add_mode_to_remove_overflowUP(node *current_node, node *new_son, node *old_son, node* node_to_stop, tree *current_tree){
    if(current_node == node_to_stop) return new_son;
    node *aux=read_node(current_node, version);
    if(aux->l_son == old_son) return add_mod_to_remove(current_node, new_son, L_SON, node_to_stop,current_tree);
    if(aux->r_son == old_son) return add_mod_to_remove(current_node, new_son, R_SON, node_to_stop,current_tree);
    return NULL;
}

node* add_mod_to_remove(node *current_node, node *new_son, type_field field, node* node_to_stop, tree *current_tree){
    //node *current_node, node* value, type_field field, tree *current_tree
    if(current_node == node_to_stop) return new_son;
    int i;  
    for(i=0 ; i<2*number_p ; i++){
        if(current_node->mods[i].field == EMPTY) break;
    }
    if(i<2*number_p){
        current_node->mods[i] = create_mod(new_son, field);
        return NULL;
    }
    else{
        node *cloned_node = clone_node(current_node);
        if(field == L_SON) cloned_node->l_son = new_son;
        if(field == R_SON) cloned_node->r_son = new_son;

        if(cloned_node->l_son != NULL) mod_overflow_DOWN(cloned_node->l_son, cloned_node, current_node);
        if(cloned_node->r_son != NULL) mod_overflow_DOWN(cloned_node->r_son, cloned_node, current_node);

        return add_mode_to_remove_overflowUP(cloned_node->return_pointers[0], cloned_node, current_node, node_to_stop,current_tree);
        
    }
}

void remove(int value, tree *current_tree){
    if(current_tree == NULL || value <= 0 ){
        printf("Invalid Entry!\n");
    }
    else{
        //node *inicial_root = current_tree->vector_of_roots[version];
        current_tree->vector_of_roots[version+1] = current_tree->vector_of_roots[version];
        node *node_to_remove = search_node(value, version, current_tree);
        
        if(node_to_remove != NULL){
            //CASO É FOLHA
            if(node_is_leaf(node_to_remove, version)){
                add_unknown_mod(node_to_remove->return_pointers[0], NULL, node_to_remove, current_tree);
            }
            //CASO NÃO É FOLHA E TEM APENAS UM FILHO
            else if(has_only_son(node_to_remove, version)){
                node *aux = read_node(node_to_remove, version);
                if(aux->l_son != NULL){
                    add_unknown_mod(node_to_remove->return_pointers[0], aux->l_son, node_to_remove, current_tree);
                }
                else{
                    add_unknown_mod(node_to_remove->return_pointers[0], aux->r_son, node_to_remove, current_tree);
                }
            }
            //CASO NÃO É FOLHA E TEM DOIS FILHOS
            else{
                node *sucessor_node = successor(value, version, current_tree);
                node *aux = read_node(sucessor_node, version);

                if(aux->r_son != NULL) aux->r_son->return_pointers[0] = sucessor_node->return_pointers[0];
                node *most_updated_r_son = add_unknown_mod_to_remove(sucessor_node->return_pointers[0], aux->r_son, sucessor_node, node_to_remove, current_tree);

                node *aux_2 = read_node(node_to_remove, version);
                if(aux_2->l_son != NULL) aux_2->l_son->return_pointers[0] = sucessor_node;
                if(most_updated_r_son != NULL) most_updated_r_son->return_pointers[0] = sucessor_node;

                sucessor_node->return_pointers[0] = node_to_remove->return_pointers[0];

                node *most_updated_sucessor_node = add_mod(sucessor_node, aux_2->l_son, L_SON, current_tree);
                add_mod(most_updated_sucessor_node, most_updated_r_son, R_SON, current_tree);

                //node *mod_overflow_UP(node *current_node, node *new_son, node *old_son, tree *current_tree){
                //(node *current_node, node *new_node, node *old_node, tree *current_tree){
                add_unknown_mod(sucessor_node->return_pointers[0], most_updated_sucessor_node, node_to_remove, current_tree);
            }
        }
        else{

        }
    }
    version++;
}