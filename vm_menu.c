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
#include "vm_menu.h"

/* 
 * Initialises the vm data structure declared in main to safe initial
 * values 
 * */
void menu_init(struct menu_item * menu)
{
    /* The UNUSED() function is designed to prevent warnings while your 
     * code is only partially complete. Delete this function call once 
     * you are using vm in your own code */
    /*  UNUSED(menu);  */
    int i;
    char menu_options[NUM_MENU_ITEMS][NAMELEN+1] = { "Display Items",
        "Purchase Items", "Save and Exit", "Add Item", "Remove Item",
        "Display Coins", "Reset Stock", "Reset Coins", "Abort Program" };
    /*  Look at Lect 8 p28 for function pointers  */
    BOOLEAN (* func_loc[])(struct vm *) = { display_items, purchase_item,
        save, add_item, delete_item, display_coins,
        reset_stock_count, reset_coin_count, abort_the_fucking_program };

    for( i=0; i<NUM_MENU_ITEMS; ++i )
    {
        strcpy( menu[i].name, menu_options[i] );
        menu[i].func = func_loc[i];
    }
}
