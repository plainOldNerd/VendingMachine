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
#define NUMARGS 3
#define ADMIN_ONLY_AFTER 3
#define CHOICE_DIGITS 2

int main(int argc, char * argv[])
{
    int i;
    char chosen[CHOICE_DIGITS];
    struct vm vm;
    struct menu_item menu_items[NUM_MENU_ITEMS];
    BOOLEAN quit = FALSE, check_load;

    if( argc != NUMARGS )
    {
        fprintf( stderr, "Incorrect number of command line arguments. \n" );
        return EXIT_FAILURE;
    }
    vm.foodfile = argv[1];
    vm.coinsfile = argv[2];

    if( vm_init( &vm ) == OS_SAID_FO )
    {
        perror( "Failed to allocate memory for list" );
        return EXIT_FAILURE;
    }
    check_load = load_data( &vm, argv[1], argv[2] );
    if( check_load != TRUE )
    {
        if( check_load == OS_SAID_FO )
        {
            perror( "Failed to load items or money" );
        }
        return EXIT_FAILURE;
    }
    /*  If load_data did not scan the next_id from file  */
    if( strcmp( vm.next_id, "I0000" ) == 0 )
    {
        set_next_id( &vm, 0 );
    }

    menu_init( menu_items );
    while( !quit )
    {
        printf( "\nMain Menu: \n" );
        for( i=1; i<=NUM_MENU_ITEMS; ++i )
        {
            printf( "%d.  %s \n", i, menu_items[i-1].name );
            if( i == ADMIN_ONLY_AFTER )
            {
                printf( "Administrator Only Menu \n" );
            }
        }
        printf( "Select your option (1 - %d):  ", NUM_MENU_ITEMS );
        fgets( chosen, CHOICE_DIGITS, stdin );
        printf( "\n" );
        read_rest_of_line();
        i = (int) strtol( chosen, NULL, 10 );

        /*  
         *  Note that i is used for re-use of variables.  It could be called
         *  "choice"
         */
        if( !( i>=1 && i<=NUM_MENU_ITEMS ) )
        {
            printf( "A valid option was not chosen.  Please try again."
                    "\n" );
        }
        else
        {
            if( menu_items[i-1].func( &vm ) == OS_SAID_FO )
            {
                perror( "Failed to allocate memory for new item or new"
                        " list node" );
                return EXIT_FAILURE;
            }
            if( menu_items[i-1].func == save || 
                    menu_items[i-1].func == abort_the_fucking_program )
            {
                quit = TRUE;
            }
        }
    }

    return EXIT_SUCCESS;
}
