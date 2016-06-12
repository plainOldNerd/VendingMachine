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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm_structs_enums_defs.h"

#ifndef VM_UTILITY
#define VM_UTILITY

#define UNUSED(var) (void)var

/* Type definition for our boolean type */
typedef enum truefalse
{
    FALSE, TRUE, OS_SAID_FO
} BOOLEAN;

struct vm;

void read_rest_of_line(void);
void system_free( struct vm * vm );

#endif
