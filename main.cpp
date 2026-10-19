#include <stdio.h>
#include <stdlib.h>

#include "functions/functions.cpp"
#include "functions/structures.cpp"
#include "functions/insert.cpp"
#include "functions/remove.cpp"
#include "functions/sucessor.cpp"

int main(){
    int i,j;
    tree *tr = create_tree();
    int tam = 3;
    int elementos[tam] = {10, 5, 15};

    //INSERINDO OS ELEMENTOS
    for(i=0;i<tam;i++){
        insert_node(elementos[i], tr);
    }

    //IMPRIMINDO TODAS AS VERSÕES
    printf("\n");
    for(i=0; i<=version;i++){
        printf("Estrutura da árvore na versão %d\n", i);
        print_order_tree(tr, i);
    }
    
    //TESTE DE SUCESSORES NAS VERSÕES
    printf("\n");
    node *n;
    for(i=0; i<tam;i++){
        for(j=0;j<=tam;j++){
            n = successor(elementos[i], j, tr);
            if(n!=NULL) printf("Sucessor de %d na versão %d é %d\n", elementos[i], j, n->value);
            else printf("Sucessor de %d na versão %d não existe\n",elementos[i], j);
        }
    }

    

    printf("\n");
    return 0;
}