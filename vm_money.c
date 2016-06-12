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

/* 
 * Displays a formatted list of the coins currently on hand in the 
 * vending machine. This fulills requirement 12 of the spec.
 * */
BOOLEAN display_coins(struct vm * vm)
{
    int i, j;
    char *written_denoms[NUMDENOMS] = { "5 cents", "10 cents", "20 cents",
        "50 cents", "1 dollar", "2 dollars", "5 dollars", "10 dollars" };

    printf( "    Coins Summary\n" );
    for( i=0; i<strlen( "Denomination | Count" ); ++i )
    {
        printf( "-" );

    }
    printf( "\nDenomination | Count\n" );
    for( i=0; i<strlen( "Denomination | Count" ); ++i )
    {
        printf( "-" );
    }
    printf( "\n" );
    for( i=0; i<NUMDENOMS; ++i )
    {
        printf( "%s", written_denoms[i] );
        for( j=0; j<strlen( "Denomination " )-strlen(written_denoms[i]);
                ++j )
        {
            printf( " " );
        }
        printf( "|    " );
        if( vm->coins[i].count < 10 )
        {
            printf( " " );
        }
        printf( "%u\n", vm->coins[i].count );
    }

    return TRUE;
}

/* 
 * Sets the count of each coin to the default value. This fulfills 
 * requirement 11 of the spec.
 * */
BOOLEAN reset_coin_count(struct vm* vm)
{
    int i;

    for( i=0; i<NUMDENOMS; ++i )
    {
        vm->coins[i].count = COIN_COUNT;
    }

    return FALSE;
}
