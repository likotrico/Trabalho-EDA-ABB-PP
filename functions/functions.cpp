#include <stdio.h>
#include <stdint.h>
#include <iostream>

#include <cstdint>

#include "../headers/functions.h"

node* search_node(int value, int version_req, node* current_node){
    //printf("valor chegou: %d", value);
    if(current_node != NULL) printf("no no %d | procurando %d\n", current_node->value, value);
    else printf("node null\n");
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
    return search_node(value, version_req, find_mod(current_node, version_req, R_SON));
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
        //if(target != NULL)printf("target:%d\n", target->value);
        //else printf("target:NULL\n");
        for(i=0; i<2*number_p; i++){
            if((current_node->mods[i].version <= version_req) && (current_node->mods[i].field == R_SON)){
                //printf("%d <= %d\n", current_node->mods[i].version, version_req);
                //printf("entrou aqui?\n");
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
        queue.push_back(current_node->value);
        //printf("%d, ", current_node->value);
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

void mod_overflow_UP(node* current_node, node* new_node, node* old_node, tree *current_tree){
    //encontrar o campo que o current_node está 
    //adicionar a modificação do campo encontrado com a informação do new_node

    //TEM QUE COLOCAR PARA PROCURAR O FILHO NA ÚLTIMA VERSÃO PARA VERIFICAR A CONDIÇÃO DO IF
    //procurar filho direito e esquerdo mais atual
    //if(current_node == NULL) printf("\ncurrent node: NULL\n");
    //else printf("\ncurrent node: diff NULL\n");
    printf("mod_overflow_UP\n");
    printf("current: %d %d| newnode %d %d| oldnode %d %d\n", current_node->value,&current_node, new_node->value,&new_node, old_node->value, &old_node);
    //printf("informando:%d que %d substitui %d\n", current_node->value, new_node->value, old_node->value);
    
    node *lson = find_mod(current_node, version, L_SON); //find_mod(node *current_node, int version_req, type_field field)
    
    node *rson = find_mod(current_node, version, R_SON);

    if(version_root_change){
        lson = find_mod(search_node(current_node->value, version+1, current_tree->vector_of_roots[version+1]), version+1, L_SON);
        rson = find_mod(search_node(current_node->value, version+1, current_tree->vector_of_roots[version+1]), version+1, R_SON);
    }
    
    printf("%d\n",lson->value);

    if(lson->value == old_node->value) add_mod(current_node, new_node, L_SON, current_tree);
    if(rson->value == old_node->value) add_mod(current_node, new_node, R_SON, current_tree);
}

void mod_overflow_DOWN(node* current_node, node* new_node, node* old_node){
    printf("mod_overflow_DOWN\n");
    printf("current %d %d| newnode %d %d| oldnode %d %d\n", current_node->value,&current_node, new_node->value,&new_node, old_node->value, &old_node);
    int i;
    for(i = 0; i < number_p; i++){
        if(current_node->return_pointers[i]->value == old_node->value){
            current_node->return_pointers[i] = new_node;
            break;
        } 
    }
}

node* clone_node(node *current_node, tree *current_tree){ //CLONO UM NÓ COM AS MODIFICAÇÕES VAZIAS E APENAS COM O ÚLTIMO PONTEIRO DE RETORNO IGUAL
    
    node *novo_no = create_node(current_node->value);
    if(version_root_change){
            //printf("entrou aqui kct?\n");
            novo_no->l_son = find_mod(search_node(novo_no->value, version+1, current_tree->vector_of_roots[version+1]), version+1, L_SON);
            novo_no->r_son = find_mod(search_node(novo_no->value, version+1, current_tree->vector_of_roots[version+1]), version+1, R_SON);
    }else{
        novo_no->l_son = find_mod(current_node, version, L_SON);//current_node->l_son;
        novo_no->r_son = find_mod(current_node, version, R_SON);//current_node->r_son;
    }
    int i;
    printf("Criando clone de %d\n", current_node->value);
    //printf("fi esq %d\n", novo_no->l_son->value);
    //printf("fi dir %d\n", novo_no->r_son->value);
    for(i = 0; i < number_p; i++){
        novo_no->return_pointers[i] = current_node->return_pointers[i];
    }
    return novo_no;
}

void add_mod(node *current_node, node *value, type_field field, tree *current_tree){
    //VERIFICANDO SE HÁ ESPAÇOS
    printf("add_mod\n");
    if(value != NULL) printf("current: %d %d| value: %d %d| field %d\n", current_node->value,&current_node, value->value, &value, field);
    else printf("current: %d %d| value: NULL | field %d\n", current_node->value,&current_node, field);
    
    int i;
    for(i=0 ; i<2*number_p ; i++){
        if(current_node->mods[i].field == EMPTY) break;
    }
    //SE HÁ ESPAÇO NOS MODS
    if(i<2*number_p) current_node->mods[i] = create_field(value, field);
    
    //SE NÃO HÁ ESPAÇO NOS MODS (i = 2*number_p)
    else{//CHEGOU SEXO\n
        printf("NAO TEM ESPACO\n");
        //printf("no %d nao tem espaco ao tentar adicionar %d na versao %d\n", current_node->value, value->value, version+1);
        //criar novo nó com a informação da modificação
        node *new_node = clone_node(current_node, current_tree);
        
        /*if(version_root_change){
            printf("entrou aqui kct?\n");
            new_node->l_son = search_node(new_node->value, version+1, current_tree->vector_of_roots[version+1])->l_son;
            new_node->r_son = search_node(new_node->value, version+1, current_tree->vector_of_roots[version+1])->r_son;
        }*/

        if(field == L_SON) new_node->l_son = value;
        else if(field == R_SON) new_node->r_son = value;

        if(new_node->l_son!=NULL)printf("fi esq %d\n", new_node->l_son->value);
        else printf("fi esq NULL\n");
        if(new_node->r_son!=NULL) printf("fi dir %d\n", new_node->r_son->value);
        else printf("fi dir NULL\n");

        //avisar para quem aponta para o novo nó que ele é o da nova versão
        for(i = 0; i < number_p; i++){
            if(current_node->return_pointers[i] != NULL) mod_overflow_UP(current_node->return_pointers[i], new_node, current_node, current_tree);
            else{
                //printf("Clonando raiz:%d\n", new_node->value);  
                //adicionar a árvore
                current_tree->vector_of_roots[version+1] = new_node;
                version_root_change = 1;
            }
        }
        //avisar para quem o novo nó aponta que ele é o da nova versão
        if(new_node->l_son != NULL) mod_overflow_DOWN(new_node->l_son, new_node, current_node);
        if(new_node->r_son != NULL) mod_overflow_DOWN(new_node->r_son, new_node, current_node);
    }
}

void insert(node *current_node, node *root, tree *current_tree){
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
            add_mod(root, current_node ,L_SON, current_tree);
        }
        else insert(current_node, aux, current_tree);   
    }
    else{
        aux = find_mod(root, version, R_SON);
        if(aux == NULL){
            current_node->return_pointers[0] = root;
            add_mod(root, current_node ,R_SON, current_tree);
        }
        else insert(current_node, aux, current_tree);
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
            
            insert(n, t->vector_of_roots[version+1], t);
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

int node_have_only_one_son(node *current_node, int version_req){
    node *left_son = find_mod(current_node, version_req, L_SON);
    node *right_son = find_mod(current_node, version_req, R_SON);
    if(left_son == NULL && right_son != NULL) return 1;
    if(left_son != NULL && right_son == NULL) return 1;
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

void remove_leaf(node *father, node *removed_node, tree *current_tree){
    //verificar qual filho na versão mais atualizada o nó removido é igual
    node *father_lson = find_mod(father, version, L_SON); //find_mod(node *current_node, int version, type_field field)
    //node *current_node_rson = find_mod(current_node, version, R_SON);
    //adicionar a modificação no campo que o filho foi removido
    if((father_lson != NULL)&&(father_lson->value == removed_node->value)){
        add_mod(father, NULL, L_SON, current_tree);
    }
    else{
        add_mod(father, NULL, R_SON, current_tree);
    }
}

void remove_OS_signal(node *father, node *removed_node, node *son, tree *current_tree){
    printf("remove_OS_signal\n");
    printf("father %d | removed %d | son %d\n", father->value, removed_node->value, son->value);
    node *father_lson = find_mod(father, version, L_SON);
    if(father_lson->value == removed_node->value){
        add_mod(father, son, L_SON, current_tree);
    }
    else{
        add_mod(father, son, R_SON, current_tree);
    }
    son->return_pointers[0] = father;
}

void remove_node_with_one_son(node *removed_node, tree *current_tree){
    node *father = removed_node->return_pointers[0];
    node *son = find_mod(removed_node, version, L_SON);
    printf("removed_node_with_one_son\n");
    if(son != NULL){
        printf("removed %d | son %d\n", removed_node->value, son->value);
        remove_OS_signal(father, removed_node, son, current_tree);
    }
    else{
        son = find_mod(removed_node, version, R_SON);
        printf("removed %d | son %d\n", removed_node->value, son->value);
        remove_OS_signal(father, removed_node, son, current_tree);
    }
}

void remove(int value, tree *current_tree){
    if(current_tree == NULL || value <= 0 ){
        printf("Invalid Entry!\n");
    }
    else{
        node *inicial_root = current_tree->vector_of_roots[version];
        printf("node:%d\n", value);
        node *n = search_node(value, version, current_tree->vector_of_roots[version]);
        int n_is_lson_of_his_father = 0;
        int n_is_rson_of_his_father = 0;
        int n_is_root = 0;

        //caso o elemento exista na estrutura
        if(n != NULL){
            node *father_n = n->return_pointers[0];
            //caso o elemento é uma folha
            if(node_is_leaf(n, version)){
                
                if(n->value == inicial_root->value){ //raiz folha
                    if(version_root_change == 0) current_tree->vector_of_roots[version+1] = NULL;
                }
                else{
                    
                    printf("father_n:%d\n", father_n->value);
                    remove_leaf(father_n, n, current_tree);
                    if(version_root_change == 0) current_tree->vector_of_roots[version+1] = inicial_root;
                    printf("Node %d was removed in version %d!\n", n->value, version+1);
                }
            }
            //caso o elemento não é folha
            else{
                //caso o elemento tem apenas um filho
                if(node_have_only_one_son(n, version)){
                    if(n->value == inicial_root->value){ //raiz com 1 filho
                        node *aux = find_mod(n, version, L_SON);
                        if(aux != NULL){
                            aux->return_pointers[0] = NULL;
                            if(version_root_change == 0) current_tree->vector_of_roots[version+1] = aux;
                        }
                        else{
                            aux = find_mod(n, version, R_SON);
                            aux->return_pointers[0] = NULL;
                            if(version_root_change == 0) current_tree->vector_of_roots[version+1] = aux;
                        }
                    }
                    else{
                        remove_node_with_one_son(n, current_tree);
                        if(version_root_change == 0) current_tree->vector_of_roots[version+1] = inicial_root;
                        printf("Node %d was removed in version %d!\n", n->value, version+1);
                    }
                }
                else{
                    //caso o elemento tem dois filhos
                    node *sucessor_n = successor(n->value, version, current_tree);
                    //caso o successor é folha
                    printf("value:%d\n", value);
                    printf("sucessor:%d\n", sucessor_n->value);
                    if(node_is_leaf(sucessor_n, version)){
                        remove_leaf(sucessor_n->return_pointers[0], sucessor_n, current_tree);
                    }
                    //caso o sucessor tem um único filho (não existe o caso onde o sucessor tem 2 filhos)
                    else{
                        remove_node_with_one_son(sucessor_n, current_tree);
                    }    

                    if(n->value == inicial_root->value){
                        inicial_root = sucessor_n;
                        n_is_root = 1;

                        if(version_root_change) n = search_node(value, version+1, current_tree->vector_of_roots[version+1]);
                        printf("n: porra %d\n", n->value);
                    }
                    else{
                        
                        father_n = search_node(father_n->value, version+1, current_tree->vector_of_roots[version+1]);  
                        node *father_n_lson;
                        
                        if(version_root_change) father_n_lson = find_mod(father_n, version+1, L_SON);
                        else father_n_lson = find_mod(father_n, version, L_SON);
                    
                        if(version_root_change) n = search_node(value, version+1, current_tree->vector_of_roots[version+1]);

                        if((father_n_lson != NULL)&&(father_n_lson->value == n->value)){
                            add_mod(father_n, sucessor_n, L_SON, current_tree);
                            n_is_lson_of_his_father = 1;
                        }
                        else{
                            add_mod(father_n, sucessor_n, R_SON, current_tree);
                            n_is_rson_of_his_father = 1;
                        }

                    }
                    //printf("AQUIIIIIIIII PORRAAA\n");
                    sucessor_n->return_pointers[0] = father_n;
                    

                    //verificar se o filho é diferente do sucessor
                    
                    if(version_root_change){
                        if(!(n_is_root)) sucessor_n = search_node(sucessor_n->value, version+1, current_tree->vector_of_roots[version+1]);
                        printf("AQUIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n");
                        printf("LEU ESSE CARAI %d\n", sucessor_n->value);  
                    }
                    //else sucessor_n = search_node(sucessor_n->value, version+1, current_tree->vector_of_roots[version+1]);

                    node *n_lson; //= find_mod(n, version, L_SON);
                    
                    if(version_root_change) {n_lson = find_mod(n ,version+1, L_SON);printf("aqui11\n");}
                    else n_lson = find_mod(n, version, L_SON);
                    
                    /*if(n_lson != NULL){
                            printf("n_lson %d\n", n_lson->value);
                            if(find_mod(search_node(n_lson->value, version+1, current_tree->vector_of_roots[version+1]), version+1, L_SON) != NULL){
                                printf("fi esquerdo %d\n", find_mod(search_node(n_lson->value, version+1, current_tree->vector_of_roots[version+1]), version+1, L_SON)->value);
                            }else printf("fi esquerdo NULL\n");

                            if(n_lson->r_son != NULL){
                                printf("fi direito %d\n", n_lson->r_son->value);
                            }else printf("fi direito NULL\n");
                    }else printf("n_lson NULL\n");*/

                    

                    /*if(find_mod(sucessor_n, version+1, L_SON)!=NULL){
                            printf("fi esquerdo %d\n", find_mod(sucessor_n, version+1, L_SON)->value);
                        }else printf("fi esquerdo NULL\n");*/

                    if(n_lson->value != sucessor_n->value){
                        n_lson->return_pointers[0] = sucessor_n;
                        add_mod(sucessor_n, n_lson, L_SON, current_tree);
                    }
                    else{ //ACHO QUE ESSE CASO NUNCA OCORRE POIS O SUCESSOR DO N NUNCA VAI SER ALGUÉM DA SUA SUBARVORE A ESQUERDA
                        n_lson->return_pointers[0] = father_n;
                        if(n_is_lson_of_his_father) add_mod(father_n, n_lson, L_SON, current_tree);
                        if(n_is_rson_of_his_father) add_mod(father_n, n_lson, R_SON, current_tree);
                    }
                    
                    node *n_rson ;//= find_mod(n, version, R_SON);
                    
                    if(version_root_change){n_rson = find_mod(n, version+1, R_SON);printf("foi opção 1\n");}
                    else {n_rson = find_mod(n, version, R_SON);printf("foi opção 2\n");}
                    
                    if(n_rson != NULL){
                            printf("n_rson %d\n", n_rson->value);
                            if(n_rson->l_son != NULL){
                                printf("fi esquerdo %d\n", n_rson->l_son->value);
                            }else printf("fi esquerdo NULL\n");

                            if(n_rson->r_son != NULL){
                                printf("fi direito %d\n", n_rson->r_son->value);
                            }else printf("fi direito NULL\n");
                            if(n_rson-> value != sucessor_n->value){
                                n_rson->return_pointers[0] = sucessor_n;
                                add_mod(sucessor_n, n_rson, R_SON, current_tree);
                                printf("\n");
                                printf("porra%d\n", sucessor_n->return_pointers[0]->value);
                                printf("\n");
                            }
                            else{
                                n_rson->return_pointers[0] = father_n;
                                if(n_is_lson_of_his_father) add_mod(father_n, n_rson, L_SON, current_tree);
                                if(n_is_rson_of_his_father) add_mod(father_n, n_rson, R_SON, current_tree);
                            }
                    }else printf("n_rson NULL\n");
                    //printf("inicialroot %d\n", inicial_root->value);
                    if(version_root_change == 0 || n_is_root) current_tree->vector_of_roots[version+1] = inicial_root;
                } 
            }
        }
        //caso o elemento não exista na estrutura
        else{ //esse else não é necessário
            if(version_root_change == 0) current_tree->vector_of_roots[version+1] = inicial_root;
            printf("Node don't exist!\n");
        }
        //atualizando a versão independente de remover um nó ou não
        version_root_change = 0;
        version++;
    }  
}