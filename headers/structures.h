#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>

int const number_p = 1; //Valor de p. A quantidade máxima de ponteiros de retorno em cada nó.
int version = 0; //A variável que contém o número da última versão.
int const maximum_versions = 1001; //O número máximo de versões suportada.
std::vector<int> queue; //Uma estrutura criada para auxiliar na função de imprimir a ordem crescente dos elementos na estrutura em uma versão requerida.

//Possíveis campos que podem ser modificados em cada modificação.
enum type_field{
    EMPTY,
    L_SON,
    R_SON
};

struct node;

//A estrutura que armazena uma modificação em um nó.
struct internal_info{
    int version;
    node *value;
    int field;
} ;

//A estrutura do nó.
struct node{ 
    int value;
    int version;
    node *l_son = NULL;
    node *r_son = NULL;
    node *return_pointers[number_p];
    internal_info mods[number_p*2];
};

//A estrutura da árvore que contém as raízes de cada versão da árvore.
struct tree{
    node *vector_of_roots[maximum_versions];
};

internal_info create_mod(node*, type_field);
node* create_node(int);

#endif