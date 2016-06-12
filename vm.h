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

#include "vm_items.h"

#ifndef VM
#define VM

BOOLEAN vm_init(struct vm * vm);
BOOLEAN load_data(struct vm * vm, const char * item_fname, 
        const char * coins_fname);
BOOLEAN save( struct vm * vm );
/*
 *  Note that abort( arguments ) is built into C.
 *  For more information, see the man page.  I did.  It's the only way I
 *  could figure out what was wrong with my calling this function abort,
 *  which took me two hours or so!!
 */
BOOLEAN abort_the_fucking_program( struct vm * vm );

#endif
