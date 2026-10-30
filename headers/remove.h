#ifndef REMOVE_H
#define REMOVE_H

#include "structures.h"

node* add_unknown_mod(node*, node*, node*, tree*);
int has_only_son(node*, int);
node* add_unknown_mod_to_remove(node*, node*, node*, node*, tree*);
node* add_mode_to_remove_overflowUP(node*, node*, node*, node*, tree*);
node* add_mod_to_remove(node*, node*, type_field, node*, tree*);
void remove(int, tree *);

#endif