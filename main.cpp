#include <stdio.h>
#include <stdlib.h>

#include "functions/functions.cpp"
#include "functions/structures.cpp"

int main(int argc, char *argv[]){
    printf("\n");

    int i;
    node *n;
    tree *tr = create_tree();
    
    /*//insert_node(3, tr);
    //insert_node(1, tr);*/
    
    /*insert_node(10, tr);
    insert_node(9, tr);
    insert_node(8, tr);
    insert_node(7, tr);*/

    insert_node(10, tr);
    insert_node(10, tr);
    insert_node(5, tr);
    insert_node(5, tr);
    insert_node(5, tr);
    insert_node(15, tr);
    insert_node(4, tr);
    insert_node(8, tr);
    insert_node(9, tr);
    insert_node(6, tr);
    insert_node(7, tr);
    insert_node(14, tr);
    insert_node(16, tr);
    insert_node(16, tr);
    
    //search(16, 7, tr);
    //print_tree(tr, 4);
    //print_tree(tr, 10);

    /*n = successor(10, 2, tr);
    if(n != NULL) printf("sucessor: %d\n", n->value);
    else printf("sucessor: diff\n");*/
    print_tree(tr, version);

    remove(10, tr);
    print_tree(tr, version);
    remove(10, tr);
    print_tree(tr, version);

    remove(14, tr);
    print_tree(tr, version);

    remove(15, tr);
    print_tree(tr, version);

    remove(16, tr);
    print_tree(tr, version);

    printf("raiz mais atual:%d\n", tr->vector_of_roots[version]->value);
    /*for(i=0; i<2*number_p;i++){
        n = tr->vector_of_roots[7]->mods[i].value;
        if(n != NULL) printf("n:%d v:%d c:%d\n", n->value, tr->vector_of_roots[7]->mods[i].version, tr->vector_of_roots[7]->mods[i].field);
        else printf("n:NULL v:%d c:%d\n", tr->vector_of_roots[7]->mods[i].version, tr->vector_of_roots[7]->mods[i].field);

        
    }*/

    //print_tree(tr, 11);

    printf("\n");
    printf("END OF EXECUTION!");
    printf("\n");
    return 0;
}