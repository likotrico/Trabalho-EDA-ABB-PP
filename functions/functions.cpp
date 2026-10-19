#include <stdio.h>

#include "../headers/functions.h"

//FUNÇÃO FAZ A LEITURA DO NÓ NA VERSÃO REQUERIDA
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

    return n;
}

void clear_node(node *current_node){
    free(current_node);
    current_node = NULL;
}

//FUNÇÃO QUE ADICIONA UMA MODIFICAÇÃO EM UM NÓ NO CAMPO REQUERIDO
node* add_mod(node *current_node, node* value, type_field field){
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
        return NULL;
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

int node_is_leaf(node *current_node){
    if(current_node->l_son == NULL && current_node->r_son == NULL) return 1;
    return 0;
}

/*node* search_node(int value, int version_req, node* no){
    //printf("valor chegou: %d", value);
    if(value <= 0 ){
        printf("Negative value\n");
        return NULL;
    }
    if(no == NULL){
        printf("The node %d don't exist in the tree in version %d!\n", value, version_req);
        return NULL;
    }
    if(no->value == value) return no;
    int i;
    if(no->value > value){
        node *address = no->l_son;
        for(i=0; i<2*number_p; i++){
            
            if((no->mods[i].version <= version_req) && (no->mods[i].field == L_SON)) address = (node *)no->mods[i].value;
        }
        return search_node(value, version_req, address);
    }
    node *address = no->r_son;
    for(i=0; i<2*number_p;i++){
        if((no->mods[i].version <= version_req) && (no->mods[i].field == R_SON)) address = (node *)no->mods[i].value;
    }
    return search_node(value, version_req, address);
}

void search(int value, int version_req, tree *t){
    if(value <= 0 || t == NULL || version_req < 0 || version_req >= maximum_versions || version_req > version) printf("Invalid entry\n");
    else{
        node *n;
        n = search_node(value, version_req, t->vector_of_roots[version_req]);
        if(n != NULL){
            printf("The node %d exists in version %d and was added in version %d\n", n->value, version_req, n->version);
        }
    }
}

void print_node(int value, node* root){ //FUNÇÃO OBSOLETA
    if(!(value <= 0 || root == NULL)){
        node* aux = search_node(value, version, root); 
        printf("O no possui valor:%d\n", aux->value);
        printf("A primeira versao do no :%d\n", aux->version);
    }
}

void mod_overflow_UP(node* current_node, node* new_node, node* old_node){
    //encontrar o campo que o current_node está 
    //adicionar a modificação do campo encontrado com a informação do new_node
    if(current_node->l_son == old_node) add_mod(current_node, new_node, L_SON);
    if(current_node->r_son == old_node) add_mod(current_node, new_node, R_SON);
}

void mod_overflow_DOWN(node* current_node, node* new_node, node* old_node){
    int i;
    for(i = 0; i < number_p; i++){
        if(current_node->return_pointers[i] == old_node){
            current_node->return_pointers[i] = new_node;
            break;
        } 
    }
}

node* clone_node(node *current_node){ //CLONO UM NÓ COM AS MODIFICAÇÕES VAZIAS E APENAS COM O ÚLTIMO PONTEIRO DE RETORNO IGUAL
    node *novo_no = create_node(current_node->value);
    novo_no->l_son = current_node->l_son;
    novo_no->r_son = current_node->r_son;
    int i;
    for(i = 0; i < number_p; i++){
        novo_no->return_pointers[i] = current_node->return_pointers[i];
    }
    return novo_no;
}

node *add_mod(node *current_node, node *value, type_field field){
    //VERIFICANDO SE HÁ ESPAÇOS
    int i;
    for(i=0 ; i<2*number_p ; i++){
        if(current_node->mods[i].field == EMPTY) break;
    }
    //SE HÁ ESPAÇO NOS MODS
    if(i<2*number_p) current_node->mods[i] = create_field(value, field);
    
    //SE NÃO HÁ ESPAÇO NOS MODS (i = 2*number_p)
    else{
        //criar novo nó com a informação da modificação
        node *new_node = clone_node(current_node);
        if(field == L_SON) new_node->l_son = (node*)value;
        else if(field == R_SON) new_node->r_son = (node*)value;

        //avisar para quem aponta para o novo nó que ele é o da nova versão
        for(i = 0; i < number_p; i++){
            mod_overflow_UP(current_node->return_pointers[i], new_node, current_node);
        }

        //avisar para quem o novo nó aponta que ele é o da nova versão
        mod_overflow_DOWN(new_node->l_son, new_node, current_node);
        mod_overflow_DOWN(new_node->r_son, new_node, current_node);
    }
}





/*int remove(){

}*/

/*int successor(){

}*/