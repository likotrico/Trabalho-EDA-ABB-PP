#include <stdio.h>
#include <stdlib.h>

#include "functions/functions.cpp"
#include "functions/structures.cpp"

int main(){
    printf("\n");

    node *n;
    tree *tr = create_tree();
    
    insert_node(3, tr);
    insert_node(1, tr);
    
    search(1, 2, tr);
    
    printf("\n");
    return 0;
}