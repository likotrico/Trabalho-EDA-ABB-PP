#include <stdio.h>
#include <stdlib.h>

#include "functions/functions.cpp"
#include "functions/structures.cpp"
#include "functions/insert.cpp"
#include "functions/remove.cpp"
#include "functions/sucessor.cpp"

int main(){
    
    tree *tr = create_tree();
    insert_node(10, tr);
    insert_node(5, tr);
    insert_node(15, tr);
    
    /*node *n = read_node(tr->vector_of_roots[version], version);
    if(n!=NULL){
        printf("n:%d\n", n->value);
        if(n->l_son!=NULL) printf("n_lson:%d\n", n->l_son->value);
        else printf("n_lson:NULL\n");

        if(n->r_son!=NULL) printf("n_rson:%d\n", n->r_son->value);
        else printf("n_rson:NULL\n");
    }*/
    
    printf("\n");
    return 0;
}