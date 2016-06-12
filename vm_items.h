/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #2
 * Full Name        : Andrew Mouat
 * Student Number   : 2109279U
 * Course Code      : COSC1076 - Advanced Programming Techniques
 * Program Code     : BP096 - B. Software Engineering
 * Start up code provided by Paul Miller and Virginia King
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include "vm_money.h"

#ifndef VM_ITEMS
#define VM_ITEMS

BOOLEAN display_items(struct vm*);
BOOLEAN add_item(struct vm*);
BOOLEAN purchase_item(struct vm*);
BOOLEAN delete_item(struct vm*);
BOOLEAN reset_stock_count(struct vm*);
BOOLEAN val_price_and_add( struct vm*, struct vm_node*, char* );
BOOLEAN set_next_id( struct vm *, int );

#endif
