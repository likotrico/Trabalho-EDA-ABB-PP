#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "functions/functions.cpp"
#include "functions/structures.cpp"
#include "functions/insert.cpp"
#include "functions/remove.cpp"
#include "functions/sucessor.cpp"

using namespace std;

int main(void){
    tree *tr = create_tree();

    string func;
    int val, ver;

    while()cin >> func{
        if(func == "INC"){
            cin >> val;
            insert_node(val, tr);
        }else if(func == "REM") {
            cin >> val;
            remove(val, tr);
        }else if(func == "SUC"){
            cin >> val >> ver;

            cout << func << " " << val << " " << ver << "\n";
            successor_tree(val, ver, tr);
        }else if(func == "IMP"){
            cin >> ver;

            cout << func << " " << ver << "\n";
            print_order_tree(tr, ver);
        }
    }
    
    return 0;
}