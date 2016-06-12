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

/* 
 * Initialises the vm data structure declared in main to safe initial
 * values.
 * */
BOOLEAN vm_init(struct vm * vm)
{
    unsigned i, money_values[NUMDENOMS] = {5,10,20,50,100,200,500,1000};
    vm_node * head = NULL;
    vm->item_list = malloc( sizeof( vm_list ) );
    if( !(vm->item_list) )
    {
        return OS_SAID_FO;
    }

    for( i=0; i<NUMDENOMS; ++i )
    {
        /*  Note that i actually will be the enum values in order!!   */
        vm->coins[i].denom = i;
        /*  This is NOT a magic number!  It is an int, not unsigned, to
         *  have -1 initialised, for comparison later when loading from
         *  file
         */
        vm->coins[i].count = -1;
        vm->coins[i].value = money_values[i];
    }

    vm->item_list->head = head;
    vm->item_list->length = 0;

    strcpy( vm->next_id, "I0000" );

    return TRUE;
}

BOOLEAN load_money( struct vm * vm, FILE * coinsfile )
{
    int i=0, j;
    long int alleged_denom;
    char line[DIGITSperLINE+1], token[DIGITSperLINE+1];
    /*  We need an intialised char ** and this does the job  */
    char just_because[2], * end_token_ptr = just_because;
    char ** end_token = &end_token_ptr;
    denomination curr_denom;

    /*  Count how many lines there are in the file to make sure there are
     *  exactly NUMDENOMS lines, otherwise the file has been formatted
     *  incorrectly
     */
    while( fgets( line, DIGITSperLINE+1, coinsfile ) )
    {
        ++i;
    }
    if( i != NUMDENOMS )
    {
        printf( "The coins file has the wrong number of lines specified, "
                "or a line has too many \ncharacters.\n" );
        return FALSE;
    }

    /*  Go back to the start of the file and start scanning lines to read
     *  data from
     */
    fseek( coinsfile, 0, SEEK_SET );
    for( i=0; i<NUMDENOMS; ++i )
    {
        /*  scan in each line from the file and get the first token  */
        fgets( line, DIGITSperLINE+1, coinsfile );
        strcpy( token, strtok( line, MONEY_DELIM ) );
        alleged_denom = strtol( token, end_token, 10 );

        if( token[0] != '\0' && ** end_token != '\0' && 
                ** end_token != '\n' )
        {
            fprintf( stderr, "Invalid format in money file. \n"
                    "Something other than digits was encountered. \n" );
            return FALSE;
        }
        for( j=0; j<NUMDENOMS; ++j )
        {
            if( alleged_denom == vm->coins[j].value )
            {
                curr_denom = vm->coins[j].denom;
                break;
            }
        }
        if( j == NUMDENOMS )
        {
            fprintf( stderr, "Invalid money denomination was "
                    "encoutered. \n" );
            return FALSE;
        }
        if( vm->coins[curr_denom].count != -1 )
        {
            fprintf( stderr, "A money denomination was encountered more"
                    " than once. \n" );
            return FALSE;
        }
        /*  If the count for the current denomination HASN'T already been
         *  loaded from file
         */
        else
        {
            /*  Now, token should be the count of the denomination in 
             *  string form
             */
            strcpy( token, strtok( NULL, MONEY_DELIM ) );
            /*  For re-use of variables, alleged_denom will now represent
             *  the count of the denomination in long int form
             */
            alleged_denom = strtol( token, end_token, 10 );

            if( token[0] == '\0' || token[0] == '\n' )
            {
                fprintf( stderr, "Invalid format in money file. \n"
                        "No count was found for some denomination. \n" );
                return FALSE;
            }
            if( token[0] != '\0' && **end_token != '\0' && 
                    **end_token != '\n' )
            {
                fprintf( stderr, "Invalid format in money file. \n"
                        "Something other than digits was encountered. \n" );
                return FALSE;
            }
            if( alleged_denom >= 0 )
            {
                vm->coins[curr_denom].count = alleged_denom;
            }
            else
            {
                fprintf( stderr, "A negative count of some denomination "
                        "was encountered. \n" );
                return FALSE;
            }
        }

        /*  If there are too many MONEY_DELIM on one line
        */
        if( strtok( NULL, MONEY_DELIM ) )
        {
            printf( "Too many delimiters in a line in money file. \n" );
            return FALSE;
        }
    }
    return TRUE;
}

BOOLEAN load_items( struct vm * vm, FILE * foodfile )
{
    long int ID, onhand;
    char line[CHARSperLINE+1], * token, price[PRICELEN+2] = "";
    /*  We need a char ** to be initialised, this does the job  */
    char just_because[2], * end_token_ptr = just_because;
    char ** end_token = &end_token_ptr;

    /*  While we have more lines  */
    while( fgets( line, CHARSperLINE, foodfile ) )
    {
        /*  
         *  We strtok( line ) once, and then increment count_tokens
         *  while( strtok( NULL ) ).  So count_tokens is init to 1
         */
        vm_node * new_node = malloc( sizeof( vm_node ) );
        stock_item * new_item = malloc( sizeof( stock_item ) );
        if( !new_node || !new_item )
        {
            return OS_SAID_FO;
        }

        /*  
         *  First, see if we are scanning an "next_id = Issss", where
         *  s represents some digit, from tems file
         *
         *  Otherwise, make sure the ID is the right length, it starts with
         *  an 'I', 'I' is followed only by digits and the number is
         *  positive
         */
        token = strtok( line, ITEM_DELIM );
        if( strncmp( token, "next_id = I", 11 ) == 0 )
        {
            long int id_num;
            char just_because[2], * end_token_ptr = just_because;
            char ** end_token = &end_token_ptr;

            strtok( token, ID_DELIM );
            token = strtok( NULL, ID_DELIM );
            id_num = strtol( token, end_token, 10 );
            if( id_num < 1 )
            {
                fprintf( stderr, "\"next_id\" was scanned from the items "
                        "file with an invalid number. \n" );
                free( new_item );
                free( new_node );
                return FALSE;
            }
            /*  If id_num is not IDLEN-1 digits long  */
            if( *end_token - token != IDLEN-1 )
            {
                fprintf( stderr, "\"next_id\" was scanned with a wrong "
                        "length from the items file. \n" );
                free( new_item );
                free( new_node );
                return FALSE;
            }
            if( !set_next_id( vm, (int) id_num ) )
            {
                free( new_item );
                free( new_node );
                return FALSE;
            }
            continue;
        }
        /*  Note that this assumes the file is not completely empty.  */
        if( !token )
        {
            fprintf( stderr, "An incorrect number of specifications was "
                    "found on some line in the items file. \n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }

        if( strlen( token ) != IDLEN )
        {
            fprintf( stderr, "An item ID is in incorrect format."
                    "\n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        if( token[0] != 'I' )
        {
            fprintf( stderr, "An item ID is in incorrect format."
                    "\n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        ID = strtol( token+1, end_token, 10 );
        if( ID < 1 )
        {
            fprintf( stderr,"The ID number cannot be less than one."
                    "\n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        if( token[0] != '\0' && !( **end_token == '\0' || 
                    **end_token == '\n' ) )
        {
            fprintf( stderr, "Invalid format in item ID number. \n"
                    "Something other than digits was encountered."
                    "\n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        strcpy( new_item->id, token );

        /*  
         *  Now look at the item name
         */
        token = strtok( NULL, ITEM_DELIM );
        if( !token )
        {
            fprintf( stderr, "An incorrect number of specifications was "
                    "found on some line in the items file. \n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        if( token[0] == '\n' || token[0] == '\0' )
        {
            fprintf( stderr, "A name was not entered for some item in "
                    "the items file. \n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }

        if( strlen( token ) > NAMELEN )
        {
            fprintf( stderr, "A name in the items list was too long. \n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        strcpy( new_item->name, token );

        /*  
         *  Well, that was easy, now look at the item description
         */
        token = strtok( NULL, ITEM_DELIM );
        if( !token )
        {
            fprintf( stderr, "An incorrect number of specifications was "
                    "found on some line in the items file. \n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        if( token[0] == '\n' || token[0] == '\0' )
        {
            fprintf( stderr, "A description was not entered for some item "
                    "in the item file. \n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        strcpy( new_item->description, token );

        /*  
         *  That was even easier.  Look at the item price in
         *  val_price_and_add
         */
        token = strtok( NULL, ITEM_DELIM );
        if( !token )
        {
            fprintf( stderr, "An incorrect number of specifications was "
                    "found on some line in the items file. \n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        if( strlen( price ) > PRICELEN )
        {
            fprintf( stderr, "The price was too many characters long in "
                    "some line in the items file. \n" );
            free( new_item );
            free( new_node );
            return FALSE;
        }
        strcpy( price, token);

        /*  
         *  And finally for the stock on hand
         */
        token = strtok( NULL, ITEM_DELIM );
        if( !token )
        {
            fprintf( stderr, "An incorrect number of specifications was "
                    "found on some line in the items file. \n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        if( token[0] == '\n' || token[0] == '\0' )
        {
            fprintf( stderr, "There was no stock on hand available for "
                    "some item in the items file. \n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        if( strlen( token ) > NOH+1 )
        {
            fprintf( stderr, "The number on hand in some line in the "
                    "items file was too large. \n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        onhand = strtol( token, end_token, 10 );
        if( ( onhand == 0 &&  !( *token >= '0' && *token <= '9' )  ) 
                || !( **end_token == '\n' || **end_token == '\0' ) )
        {
            printf( "*token = %c **end_token = %c", *token, **end_token );
            fprintf( stderr, "Something other than digits was encountered "
                    "in the stock on hand in some line in the items file."
                    "\n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        if( onhand < 0 )
        {
            fprintf( stderr, "A negative amount of stock was encountered "
                    "for some item in the items file. \n" );
            free( new_node );
            free( new_item );
            return FALSE;
        }
        token = strtok( NULL, ITEM_DELIM );
        if( token )
        {
            fprintf( stderr, "An incorrect number of specifications was "
                    "found on some line in the items file. \n" );
            free( new_item );
            free( new_node );
            return FALSE;
        }
        new_item->on_hand = onhand;

        new_node->data = new_item;

        if( !val_price_and_add( vm, new_node, price ) )
        {
            free( new_item );
            free( new_node );
            return FALSE;
        }
    }

    return TRUE;
}

/* 
 * Loads data from the .dat files into memory 
 * */
BOOLEAN load_data(struct vm * vm, const char * item_fname, 
        const char * coins_fname)
{
    FILE * items_file, * coins_file;

    items_file = fopen( vm->foodfile, "r" );
    coins_file = fopen( vm->coinsfile, "r" );

    /*  If one or two of these pointers are NULL then the files don't
     *  exist!!
     */
    if( !items_file )
    {
        perror( "One or both of the files do not exist! \n" );
        if( !coins_file )
        {
            fclose( coins_file );
        }
        fclose( items_file );
        return FALSE;
    }
    else
    {
        if( !coins_file )
        {
            perror( "One or both of the files do not exist! \n" );
            fclose( coins_file );
            return FALSE;
        }
    }

    /*  Let's load money data first  */
    if( !load_money( vm, coins_file ) )
    {
        fclose( coins_file );
        fclose( items_file );
        fprintf( stderr, "The program will not be run. \n" );
        return FALSE;
    }

    /*  Now let's load the item data.  Yayyyyyyyyyyyyyy, fun!
    */
    if( !load_items( vm, items_file ) )
    {
        fclose( coins_file );
        fclose( items_file );
        fprintf( stderr, "The program will not be run. \n" );
        return FALSE;
    }
    fclose( coins_file );
    fclose( items_file );
    return TRUE;
}

/* 
 * Saves the updated stock count to the stock file specified on the 
 * command line. This partially fulfill requirement 7 of the spec.
 * */
BOOLEAN save(struct vm * vm)
{
    int i;
    vm_node * curr_node = vm->item_list->head;
    FILE * itemsfile = fopen( vm->foodfile, "w" );
    FILE * moneyfile = fopen( vm->coinsfile, "w" );

    while( curr_node )
    {
        fprintf( itemsfile, "%s%s%s%s%s%s%u%c%.2u%s%u\n", 
                curr_node->data->id, ITEM_DELIM,
                curr_node->data->name, ITEM_DELIM,
                curr_node->data->description, ITEM_DELIM,
                curr_node->data->price.dollars, PRICE_DELIM,
                curr_node->data->price.cents, ITEM_DELIM,
                curr_node->data->on_hand );
        curr_node = curr_node->next;
    }
    fprintf( itemsfile, "next_id = %s\n", vm->next_id );
    fclose( itemsfile );

    for( i=0; i<NUMDENOMS; ++i )
    {
        fprintf( moneyfile, "%u%s%d\n", vm->coins[i].value, MONEY_DELIM,
                vm->coins[i].count );
    }
    fclose( moneyfile );

    system_free( vm );
    return TRUE;
}

BOOLEAN abort_the_fucking_program( struct vm * vm )
{
    system_free( vm );
    return TRUE;
}
