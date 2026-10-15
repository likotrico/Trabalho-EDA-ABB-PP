#include <stdio.h>
#include <stdint.h>
#include <iostream>

#include <cstdint>

#include "../headers/functions.h"

node* search_node(int value, int version_req, node* current_node){
    //printf("valor chegou: %d", value);
    if(value <= 0 ){
        printf("Negative value\n");
        return NULL;
    }
    if(current_node == NULL){
        printf("The node %d don't exist in the tree in version %d!\n", value, version_req);
        return NULL;
    }
    if(current_node->value == value) return current_node;
    if(current_node->value > value){
        //node *address = find_mod(no, version_req, L_SON);
        return search_node(value, version_req, find_mod(current_node, version_req, L_SON));
    }
    //node *address = find_mod(no, version_req, L_SON);
    return search_node(value, version_req, find_mod(current_node, version_req, L_SON));
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

node* find_mod(node *current_node, int version_req, type_field field){
    int i;
    node *target;
    //printf("procurando filho do no %d\n", current_node->value);
    if(field == L_SON){
        target = current_node->l_son;
        for(i=0; i<2*number_p; i++){
            /*printf("No loop do find_mod, interacao %d\n", i);
            printf("%d <= %d\n", current_node->mods[i].version, version_req);
            printf("%d == %d\n", current_node->mods[i].field, L_SON);*/
            if((current_node->mods[i].version <= version_req) && (current_node->mods[i].field == L_SON)){
                target = (node *)current_node->mods[i].value;
                //target = reinterpret_cast<uintptr_t>(current_node->mods[i].value);
                
            }
        }
        //if(target != NULL) printf("Para o no %d o seu filho mais recente e %d\n", current_node->value, target->value);
        return target;
    }
    else if(field == R_SON){
        target = current_node->r_son;
        for(i=0; i<2*number_p; i++){
            if((current_node->mods[i].version <= version_req) && (current_node->mods[i].field == R_SON)){
                target = (node *)current_node->mods[i].value;
            }
        }
        return target;
    }
    else{
        printf("Something went wrong in FIND_MOD function\n");
        return NULL;
    }
}

void print_order(node *current_node, int version_req){
    if(current_node == NULL) /*printf(" [Trying print null node!] ")*/;
    else{
        //encontrar o filho esquerdo da versão
        node *left_son = find_mod(current_node, version_req, L_SON);
        //encontrar o filho direito da versão
        node *right_son = find_mod(current_node, version_req, R_SON);

        //if(current_node->l_son != NULL) printf("%d, ", );
        print_order(left_son, version_req);
        //printf("%d, ", current_node->value);
        queue.push_back(current_node->value);
        print_order(right_son, version_req);
    }
}

void print_tree(tree* current_tree, int version_req){ 
    if(current_tree == NULL) printf("Trying print null tree!\n");
    else if(version_req < 0) printf("Negative value in version parameter\n");
    else if(version_req > version) printf("Trying print version %d, but the maximum version is %d\n", version_req, version);
    else{
        print_order(current_tree->vector_of_roots[version_req], version_req);
        //printf("Tamanho da fila %ld\n", queue.size());
        int i;
        //printf("size:%ld\n", queue.size());
        for(i=0; i<queue.size(); i++){
            //aux = queue.erase(queue.begin())
            //printf("size:%ld\n", queue.size());
            /*if(queue.size() == 1) printf("%d", *queue.erase(queue.begin()));
            else printf("%d, ", *queue.erase(queue.begin()));*/
            if(i == queue.size()-1) printf("%d", queue[i]);
            else printf("%d, ", queue[i]);
        }
        queue.clear();
        printf("\n");
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

void add_mod(node *current_node, node *value, type_field field){
    //VERIFICANDO SE HÁ ESPAÇOS
    int i;
    //printf("valor: %d", current_node->value);
    for(i=0 ; i<2*number_p ; i++){
        //printf("valor i no loop: %d\n", i);
        //printf("current node %d\n",current_node->mods[i].field);
        //printf("EMPTY %d\n",EMPTY);
        if(current_node->mods[i].field == EMPTY) {/*printf("entrou aqui?\n");*/break;}
    }
    //SE HÁ ESPAÇO NOS MODS
    if(i<2*number_p) current_node->mods[i] = create_field(value, field);
    
    //SE NÃO HÁ ESPAÇO NOS MODS (i = 2*number_p)
    else{//CHEGOU SEXO\n
        //criar novo nó com a informação da modificação
        node *new_node = clone_node(current_node);
        if(field == L_SON) new_node->l_son = value;
        else if(field == R_SON) new_node->r_son = value;

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
    node *aux;
    //printf("%d\n", root->value);
    //printf("%d\n", root->value);
    if(current_node->value <= root->value){
        //printf("BBBBBBB\n");
        //printf("%d\n", root->value);
        aux = find_mod(root, version, L_SON);
        /*if(aux == NULL){
            printf("aux NULL\n");
        }else printf("aux diff NULL\n");*/
        //printf("%d\n", aux->value);
        if(aux == NULL){
            current_node->return_pointers[0] = root;
            /*printf("\nAntes de entrar no add_mod\n");
            printf("%d\n", current_node->mods[0].field);
            printf("%d\n", current_node->mods[1].field);*/
            //printf("%d\n");
            add_mod(root, current_node ,L_SON);
        }
        else insert(current_node, aux);   
    }
    else{
        aux = find_mod(root, version, R_SON);
        if(aux == NULL){
            current_node->return_pointers[0] = root;
            add_mod(root, current_node ,R_SON);
        }
        else insert(current_node, aux);
    }
}

void insert_node(int value, tree *t){
    if(value <= 0) printf("Negative value can't be insert!\n");
    else if(version+1 >= maximum_versions) printf("Maximum number of versions!\n");
        
    else{
        node *n = create_node(value);
        t->vector_of_roots[version+1] = t->vector_of_roots[version];
        printf("NO VALUE: %d\n", n->value);
        printf("querendo inserir na versao: %d\n", version+1);
        
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

node* find_strictly_greater(int value, int version_req, node *current_node){
    if(current_node == NULL) return NULL;//ver o cenário que retorna int e aqui retorna -1 para sinalizar que o sucessor não existe
    node *current_l_son = find_mod(current_node, version_req, L_SON);//filho mais atualizado na versão requerida
    if(current_l_son == NULL) return current_node; //caso em que chegamos no sucessor
    return find_strictly_greater(value, version_req, current_l_son); //caso em que ainda não chegamos no elementos mais a esquerda
}

int node_is_leaf(node *current_node, int version_req){
    node *left_son = find_mod(current_node, version_req, L_SON);
    node *right_son = find_mod(current_node, version_req, R_SON);
    if(left_son == NULL && right_son == NULL) return 1;
    return 0;
}

node* successor(int value, int version_req, tree *current_tree){
    //VERIFICANDO SE AS ENTRADAS ESTÃO CORRETAS
    
    if(current_tree == NULL || value <= 0 || version_req < 0 || version_req > version){
        printf("Invalid Entry!\n");
        return NULL;
    }
    else{
        node *aux, *aux_lson, *aux_rson, *last_left;
        aux = current_tree->vector_of_roots[version_req];
        last_left = NULL;
        
        //raiz nula(falta tratar a situação que ocorre essa saída)
        if(aux == NULL) return NULL;
        //procurando o elemento com o value passado na estrutura
        while(aux != NULL){
            aux_lson = find_mod(aux, version_req, L_SON);//pegando o filho direito mais atualizado na versão requerida
            aux_rson = find_mod(aux, version_req, R_SON);//pegando o filho direito mais atualizado na versão requerida
            
            /*printf("aux: %d\n", aux->value);
            if(aux_lson != NULL) printf("aux_lson: %d\n", aux_lson->value);
            else printf("aux_lson: NULL\n");
            if(aux_rson != NULL) printf("aux_rson: %d\n", aux_rson->value);
            else printf("aux_rson: NULL\n");*/

            //o elemento de valor value existe na estrutura
            if(aux->value == value){
                //caso elemento é folha
                if(node_is_leaf(aux, version_req)) return last_left;
                //caso elemento não é folha
                else{
                    //caso filho direito é nulo
                    if(aux_rson == NULL) return last_left;
                    //caso filho direito não é nulo
                    else return find_strictly_greater(value, version_req, aux_rson);//encontrando o cara mais a esquerda da subarvore direita de aux
                }
            }
            //atualizando o nó analisado na estrutura
            if(value <= aux->value){
                last_left = aux;
                aux = aux_lson;
            }
            else aux = aux_rson;      
        }
        //caso o elemento não exista na estrutura
        return last_left;
    }
}

/*int remove(){

}*/