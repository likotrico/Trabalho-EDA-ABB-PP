#include <stdio.h>
#include <stdlib.h>

#include "functions/functions.cpp"
#include "functions/structures.cpp"

int main(){
    printf("\n");

    node *n;
    tree *tr = create_tree();
    
    //insert_node(3, tr);
    //insert_node(1, tr);
    
    insert_node(10, tr);
    insert_node(5, tr);
    insert_node(15, tr);
    //printf("AAAAA\n");
    insert_node(4, tr);
    insert_node(6, tr);
    insert_node(14, tr);
    insert_node(16, tr);
    
    search(16, 7, tr);
    print_tree(tr, 7);

    printf("\n");
    printf("END OF EXECUTION!");
    printf("\n");
    return 0;
}