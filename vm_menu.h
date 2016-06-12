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

#include "vm.h"

#ifndef VM_MENU
#define VM_MENU

#define NUM_MENU_ITEMS 9

/* The data structure to hold information about each menu option, the 
 * name of the function to display and a pointer to the function that 
 * will implement that option. */
struct menu_item
{
    char name[NAMELEN+1];
    BOOLEAN (*func)(struct vm*);
};

typedef struct menu_item menu_item;

/* Builds the menu for the vending machine. */
void menu_init(struct menu_item *);

#endif
