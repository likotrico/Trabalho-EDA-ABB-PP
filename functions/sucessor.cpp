#include "../headers/sucessor.h"

node* find_strictly_greater(int value, int version_req, node *current_node){
    if(current_node == NULL) return NULL;
    node *aux = read_node(current_node, version_req);
    if(aux->l_son == NULL) return current_node; //caso em que chegamos no sucessor
    return find_strictly_greater(value, version_req, aux->l_son); //caso em que ainda não chegamos no elementos mais a esquerda
}

//FUNÇÃO QUE RETORNA O NÓ SUCESSOR DE UM VALOR EM UMA DADA VERSÃO NA ESTRUTURA
node* successor(int value, int version_req, tree *current_tree){
    node *aux, *last_left;
    node *current_node = current_tree->vector_of_roots[version_req];
    last_left = NULL;
        
    if(current_node == NULL) return NULL;
    //procurando o elemento com o value passado na estrutura
    while(current_node != NULL){
        aux = read_node(current_node, version_req);
        //o elemento de valor value existe na estrutura
        if(aux->value == value){
            //caso elemento é folha
            if(node_is_leaf(aux)) return last_left; //codar função
            //caso elemento não é folha
            else{
                //caso filho direito é nulo
                if(aux->r_son == NULL) return last_left;
                //caso filho direito não é nulo
                else return find_strictly_greater(value, version_req, aux->r_son);//encontrando o cara mais a esquerda da subarvore direita de aux
            }
        }
        //atualizando o nó analisado na estrutura
        if(value <= aux->value){
            last_left = current_node;
            current_node = aux->l_son;
        }
        else current_node = aux->r_son;
        clear_node(aux);
    }
    //caso o elemento não exista na estrutura
    return last_left;
}

//FUNÇÃO QUE IMPRIME O VALOR DO SUCESSOR DE UM VALOR EM UMA DADA VERSÃO NA ESTRUTURA
void successor_tree(int value, int version_req, tree *current_tree){
    if(current_tree == NULL || value <= 0 || version_req < 0 || version_req > version) printf("Invalid Entry!\n");
    node *n = successor(value, version_req, current_tree);
    if(n!=NULL){
        printf("sucessor de %d na versão %d é %d\n", value, version_req, n->value);
        //printf("%d\n", n->value);
    }
    else printf("inf\n");
}