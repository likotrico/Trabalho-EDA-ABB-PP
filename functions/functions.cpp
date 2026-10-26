#include <stdio.h>

#include "../headers/functions.h"

//FUNÇÃO FAZ A LEITURA DO NÓ NA VERSÃO REQUERIDA.
//RETORNA O NÓ NA VERSÃO REQUERIDA COM OS FILHOS ATUALIZADOS.
node* read_node(node *current_node, int version_req){
    node *n = (node*)malloc(sizeof(node));
    int i;
    n->value = current_node->value;
    n->version = current_node->version;
    
    for(i=0; i<number_p;i++){
        n->return_pointers[i] = current_node->return_pointers[i];    
    }

    n->l_son = current_node->l_son;
    n->r_son = current_node->r_son;

    for(i=0;i<2*number_p;i++){
        if(current_node->mods[i].version > version_req) break;
        if(current_node->mods[i].field == L_SON) n->l_son = current_node->mods[i].value;
        if(current_node->mods[i].field == R_SON) n->r_son = current_node->mods[i].value;
    }

    for(i=0; i<2*number_p;i++){
        n->mods[i].version = -1;
        n->mods[i].value = NULL;
        n->mods[i].field = EMPTY;
    }

    return n;
}

//LIMPA A MEMÓRIA DE UM NÓ.
void clear_node(node *current_node){
    free(current_node);
    current_node = NULL;
}

//CLONA UM NÓ, CUJO OS MODIFICADORES ESTÃO CHEIOS.
node *clone_node(node *current_node){
    node *new_node = read_node(current_node, version);
    new_node->version = version+1;
    return new_node;
}

//FUNÇÃO AUXILIAR QUE ATUALIZA OS PONTEIROS DE RETORNO DOS FILHOS DE UM NÓ CLONADO.
void mod_overflow_DOWN(node *current_node, node *new_father, node *old_father){
    int i;
    for(i=0;i<number_p;i++){
        if(current_node->return_pointers[i] == old_father) current_node->return_pointers[i] = new_father;
    }
}


node *mod_overflow_UP(node *current_node, node *new_son, node *old_son, tree *current_tree){
    node *aux=read_node(current_node, version);
    if(aux->l_son == old_son) return add_mod(current_node, new_son, L_SON, current_tree);
    if(aux->r_son == old_son) return add_mod(current_node, new_son, R_SON, current_tree);
    return NULL;
}

node *mod_overflow_UP_ROOT(tree *current_tree, node *new_root){
    current_tree->vector_of_roots[version+1] = new_root;
    return new_root;
}

//FUNÇÃO QUE ADICIONA UMA MODIFICAÇÃO EM UM NÓ NO CAMPO REQUERIDO
node* add_mod(node *current_node, node* value, type_field field, tree *current_tree){
    //VERIFICANDO SE HÁ ESPAÇOS
    int i;
    for(i=0 ; i<2*number_p ; i++){
        if(current_node->mods[i].field == EMPTY) break;
    }
    //SE HÁ ESPAÇO NOS MODS
    if(i<2*number_p){
        current_node->mods[i] = create_mod(value, field);
        return current_node;
    }
    //SE NÃO HÁ ESPAÇO NOS MODS
    else{
        node *cloned_node = clone_node(current_node);
        if(field == L_SON) cloned_node->l_son = value;
        if(field == R_SON) cloned_node->r_son = value;

        if(cloned_node->l_son != NULL) mod_overflow_DOWN(cloned_node->l_son, cloned_node, current_node);
        if(cloned_node->r_son != NULL) mod_overflow_DOWN(cloned_node->r_son, cloned_node, current_node);

        if(cloned_node->return_pointers[0] == NULL){
            mod_overflow_UP_ROOT(current_tree, cloned_node);
        }
        else{
            mod_overflow_UP(cloned_node->return_pointers[0], cloned_node, current_node, current_tree);
            
        }
        return cloned_node;

    }
}

//FUNÇÃO RECURSIVA PARA INSERIR NA FILA OS ELEMENTOS EM ORDEM CRESCENTE
void print_order(node *current_node, int version_req){
    if(current_node == NULL) /*printf(" [Trying print null node!] ")*/;
    else{
        node *aux = read_node(current_node, version_req);

        print_order(aux->l_son, version_req);
        queue.push_back(current_node->value);
        print_order(aux->r_son, version_req);

        clear_node(aux);
    }
}

//FUNÇÃO QUE IMPRIME A ESTRUTURA DA ÁRVORE NA VERSÃO REQUERIDA
void print_order_tree(tree* current_tree, int version_req){ 
    if(current_tree == NULL) printf("Trying print null tree!\n");
    else if(version_req < 0) printf("Negative value in version parameter\n");
    else if(version_req > version) printf("Trying print version %d, but the maximum version is %d\n", version_req, version);
    else{
        print_order(current_tree->vector_of_roots[version_req], version_req); //coda ainda
        int i;
        for(i=0; i<queue.size(); i++){
            if(i == queue.size()-1) printf("%d", queue[i]);
            else printf("%d, ", queue[i]);
        }
        queue.clear();
        printf("\n");
    }
}

int node_is_leaf(node *current_node, int version_req){
    node *aux = read_node(current_node, version_req);
    if(aux->l_son == NULL && aux->r_son == NULL){
        clear_node(aux);
        return 1;
    }
    clear_node(aux);
    return 0;
}

node *search_node(int value, int version_req, tree *current_tree){
    if(current_tree == NULL) return NULL;
    if(value < 0) return NULL;
    node *current_node = current_tree->vector_of_roots[version_req];
    node *aux, *result = NULL;
    while(current_node != NULL){
        aux = read_node(current_node, version_req);
        if(aux->value == value){
            result = current_node;
            break;
        }
        if(aux->value >= value){
            current_node = aux->l_son;
        }
        else current_node = aux->r_son;
        //clear_node(aux);
    }
    clear_node(aux);
    return result;
}