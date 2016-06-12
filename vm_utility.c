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

#include "vm_utility.h"

/* 
 * Clears the input buffer.
 * */
void read_rest_of_line(void)
{
    int ch;
    while(ch = getc(stdin), ch!='\n' && ch != EOF) 
        ;
    clearerr(stdin);
}

/* 
 * Frees all dynamically allocated data.
 */
void system_free(struct vm * vm)
{
    struct vm_node * curr_node = vm->item_list->head, * prev_node;

    while( curr_node )
    {
        prev_node = curr_node;
        curr_node = curr_node->next;
        vm->item_list->head = curr_node;
        free( prev_node->data );
        free( prev_node );
        --(vm->item_list->length);
    }
    free( vm->item_list );
    /*  free( Tibet );  */
}
