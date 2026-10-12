#include <stdio.h>

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
    if(current_node->value == value) return no;
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
        printf("%d, ", current_node->value);
        print_order(right_son, version_req);
    }
}

void print_tree(tree* current_tree, int version_req){ 
    if(current_tree == NULL) printf("Trying print null tree!\n");
    else if(version_req < 0) printf("Negative value in version parameter\n");
    else if(version_req > version) printf("Trying print version %d, but the maximum version is %d\n", version_req, version);
    else{
        print_order(current_tree->vector_of_roots[version_req], version_req);
        printf("\n");
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
    else{
        //printf("VALOR DE i: %d\n", i);
        //printf("CHEGOU SEXO\n");
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
    node *aux;
    if(current_node->value <= root->value){
        aux = find_mod(root, version, L_SON);
        if(aux == NULL){
            current_node->return_pointers[0] = root;
            /*printf("\nAntes de entrar no add_mod\n");
            printf("%d\n", current_node->mods[0].field);
            printf("%d\n", current_node->mods[1].field);
            printf("%d\n");*/
            add_mod(root, (int)current_node ,L_SON);
        }
        else insert(current_node, aux);   
    }
    else{
        aux = find_mod(root, version, R_SON);
        if(aux == NULL){
            current_node->return_pointers[0] = root;
            add_mod(root, (int)current_node ,R_SON);
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
        //printf("NO VALUE: %d\n", n->value);
        //printf("querendo inserir na versao: %d\n", version+1);
        
        //INSERINDO A RAIZ
        if(t->vector_of_roots[version+1] == NULL){
            t->vector_of_roots[version+1] = n;
        }
        //INSERINDO UM NÃO RAIZ
        else{
            insert(n, t->vector_of_roots[version+1]);
        }
        version++;
        //printf("node with value %d was sucessfully added in version %d\n", value, version);
    }
}

/*int remove(){

}*/

/*int successor(){

}*/