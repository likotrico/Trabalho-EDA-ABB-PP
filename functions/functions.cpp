#include <stdio.h>

#include "../headers/functions.h"

node* search_node(int value, int version_req, node* no){
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
    if(current_node->l_son == old_node) add_mod(current_node, (int)new_node, L_SON);
    if(current_node->r_son == old_node) add_mod(current_node, (int)new_node, R_SON);
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

void add_mod(node *current_node, int value, type_field field){
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

void insert(node *current_node, node *root){
    if(current_node->value <= root->value){
        if(root->l_son == NULL){
            current_node->return_pointers[0] = root;
            add_mod(root, (int)current_node ,L_SON);
        }
        else insert(current_node, root->l_son);   
    }
    else{
        if(root->r_son == NULL){
            current_node->return_pointers[0] = root;
            add_mod(root, (int)current_node ,R_SON);
        }
        else insert(current_node, root->r_son);
    }
}

void insert_node(int value, tree *t){
    if(value <= 0) printf("Negative value can't be insert!\n");
    else if(version+1 >= maximum_versions) printf("Maximum number of versions!\n");
        
    else{
        node *n = create_node(value);
        t->vector_of_roots[version+1] = t->vector_of_roots[version];
        
        //INSERINDO A RAIZ
        if(t->vector_of_roots[version+1] == NULL){
            t->vector_of_roots[version+1] = n;
        }
        //INSERINDO UM NÃO RAIZ
        else{
            insert(n, t->vector_of_roots[version+1]);
        }
        version++;
        printf("node with value %d was sucessfully added in version %d\n", value, version);
    }
}

/*int remove(){

}*/

/*int successor(){

}*/