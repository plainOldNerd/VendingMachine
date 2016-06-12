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
BOOLEAN set_next_id( struct vm * vm, int id )
{
    char sprnt[IDLEN+1];
    int max_id = id;
    /*  
     *  In main, we will call this function and pass in 0 if the next_id
     *  is not scanned from the items file.
     */
    if( max_id == 0 )
    {
        vm_node * curr_node = vm->item_list->head;

        while( curr_node )
        {
            if( strtol( (curr_node->data->id)+1, NULL, 10 ) > max_id )
            {
                max_id = (int) strtol( ( curr_node->data->id )+1, NULL, 
                        10 );
            }

            curr_node = curr_node->next;
        }
        ++max_id;
    }
    if( max_id >= 1 && max_id < 10 )
    {
        sprintf( sprnt, "I000%d", max_id );
    }
    if( max_id >= 10 && max_id < 100 )
    {
        sprintf( sprnt, "I00%d", max_id );
    }
    if( max_id >= 100 && max_id < 1000 )
    {
        sprintf( sprnt, "I0%d", max_id );
    }
    if( max_id >= 1000 && max_id < 10000 )
    {
        sprintf( sprnt, "I%d", max_id );
    }
    if( max_id <=0 || max_id > 10000 )
    {
        fprintf( stderr, "\"next_id\" scanned from file was too small"
                " or too large. \n" );
    }
    strcpy( vm->next_id, sprnt );

    return TRUE;
}

/* 
 * Displays a formatted list of the items currently for sale in the 
 * machine. This will fulfill requirement 5 of the spec.
 * */
BOOLEAN display_items(struct vm* vm)
{
    int i;
    struct vm_node * curr_node;
    curr_node = vm->item_list->head;

    printf( "Items Menu \n" );
    printf( "---------- \n" );
    printf( "ID    | Name" );
    for( i=1; i<=NAMELEN-strlen("Name"); ++i )
    {
        printf( " " );
    }
    printf( " | Available | Price \n" );
    /*  Extra spaces for immediately before and after the '|' charaters  */
    for( i=1; i<=CHARSperLINE-DESCLEN+12; ++i )
    {
        printf( "-" );
    }
    printf( "\n" );
    while( curr_node )
    {
        printf( "%s | %s", curr_node->data->id, curr_node->data->name );
        for( i=0; i<=NAMELEN-strlen( curr_node->data->name ); ++i )
        {
            printf( " " );
        }
        printf( "| " );
        for( i=1; i<=strlen( "Available" )-NOH; ++i )
        {
            printf( " " );
        }
        /*  If on_hand is a single-digit number  */
        if( curr_node->data->on_hand < 10 )
        {
            printf( " " );
        }

        printf( "%u |$%5.2f \n", curr_node->data->on_hand,
                curr_node->data->price.dollars + 
                (float) curr_node->data->price.cents / 100 );

        curr_node = curr_node->next;
    }

    return TRUE;
}

/* 
 * Allows a customer to purchase an item for the vending machine. This
 * fulfills requirement 6 of the spec.
 * */
BOOLEAN purchase_item(struct vm * vm)
{
    int i, yet_to_pay, just_entered, money_entered[NUMDENOMS];
    int change_dollars, change_cents;
    int change_needed[NUMDENOMS];
    char token[IDLEN+2], just_because[2], *end_token_ptr = just_because;
    char ** end_token = &end_token_ptr;
    char * written_denoms[NUMDENOMS] = { "5c", "10c", "20c", "50c", "$1",
        "$2", "$5", "$10" };
    vm_node * chosen_node = vm->item_list->head;
    denomination denom_entered;

    /*  Initialise the money_entered and change_needed arrays  */
    for( i=0; i<NUMDENOMS; ++i )
    {
        money_entered[i] = 0;
        change_needed[i] = 0;
    }

    printf( "Please enter the ID of the item you want to buy:  " );
    fgets( token, IDLEN+2, stdin );
    token[strlen(token)-1] = '\0';
    while( chosen_node )
    {
        if( strcmp( chosen_node->data->id, token ) == 0 )
        {
            break;
        }
        chosen_node = chosen_node->next;
    }
    if( !chosen_node )
    {
        printf( "Sorry, no item has that ID. \n" );
        return TRUE;
    }

    if( chosen_node->data->on_hand <= 0 )
    {
        printf( "Sorry, there is none in stock of %s. \n", 
                chosen_node->data->name );
        return TRUE;
    }
    printf( "You chose \"%s - %s\".  \nThis costs $%u.%.2u.\n", 
            chosen_node->data->name, chosen_node->data->description,
            chosen_node->data->price.dollars, 
            chosen_node->data->price.cents );
    yet_to_pay = (int) 100*chosen_node->data->price.dollars +
        chosen_node->data->price.cents;
    /* 
     *  While there's more money to pay, get the money, make sure it's a
     *  valid denomination, subtract it from the remaining debt, and
     *  record how much of which denominations have been entered
     */
    while( yet_to_pay > 0 )
    {
        printf( "I'm hungry!  Feed me with cents.  Or press Cntl-D or"
                " Enter and I'll beg someone else.\n"
                "I could eat $%u.%.2u more:  ",
                (unsigned) yet_to_pay/100, (unsigned) yet_to_pay % 100 );
        if( !fgets( token, IDLEN+2, stdin ) )
        {
            printf( "\n" );
            return TRUE;
        }
        if( token[0] == '\n' )
        {
            return TRUE;
        }
        just_entered = (int) strtol( token, end_token, 10 );
        if( **end_token != '\n' || **end_token == '\0' )
        {
            printf( "You entered something other than digits. \n" );
            continue;
        }
        denom_entered = -1;
        for( i=0; i<NUMDENOMS; ++i )
        {
            if( just_entered == vm->coins[i].value )
            {
                denom_entered = i;
                break;
            }
        }
        if( i == NUMDENOMS )
        {
            printf( "You did not enter a valid denomination. \n" );
            continue;
        }
        yet_to_pay -= just_entered;
        ++money_entered[denom_entered];
    }

    /*  If we need to dispense change  */
    if( yet_to_pay < 0 )
    {
        change_dollars = (int) -yet_to_pay/100;
        change_cents = (int) -yet_to_pay%100;
    }
    /*  
     *  Now we need to calculate change, which we need to do as long as
     *  yet_to_pay is negative
     */
    i = NUMDENOMS-2;
    while( yet_to_pay < 0 && i >= 0 )
    {
        if( -yet_to_pay >= vm->coins[i].value && 
                vm->coins[i].count+money_entered[i]-change_needed[i] > 0 )
        {
            ++change_needed[i];
            yet_to_pay += vm->coins[i].value;
        }
        else
        {
            --i;
        }
    }
    /*  
     *  If i <= -1 then the loop terminated without being able to calculate
     *  the change
     */
    if( i <= -1 )
    {
        printf( "Sorry, your change could not be made.  Refunding your "
                "money. \n" );
        for( i=NUMDENOMS-1; i>=0; --i )
        {
            if( money_entered[i] > 0 )
            {
                printf( "%d x %s ", money_entered[i], written_denoms[i] );
            }
        }
        printf( "\n" );
    }
    /*  
     *  else the change has been calculated successfully and we must
     *  dispense the item, change, and modify the stock_onhand and count of
     *  the item and money respectively
     */
    else
    {
        for( i=0; i<NUMDENOMS; ++i )
        {
            vm->coins[i].count += money_entered[i];
        }
        printf( "Thank you.  Here's your %s, and your change of %d.%.2d: \n"
                , chosen_node->data->name, change_dollars, change_cents );
        for( i=NUMDENOMS-1; i>=0; --i )
        {
            if( change_needed[i] > 0 )
            {
                printf( "%d x %s ", change_needed[i], written_denoms[i] );
            }
            vm->coins[i].count -= change_needed[i];
        }
        printf( "\nEnjoy!\n" );
        --(chosen_node->data->on_hand);
    }

    return TRUE;
}

BOOLEAN val_price_and_add( struct vm * vm, struct vm_node * new_node,
        char * token )
{
    char just_because[2], * cents_at, * end_token_ptr = just_because;
    char ** end_token = &end_token_ptr;
    long int dollars, cents;
    struct vm_node * curr_node, * prev_node;

    dollars = strtol( token, end_token, 10 );
    /*  
     *  Make sure that the line didn't end immediately after the
     *  ITEM_DELIM
     */
    if( token[0] == '\0' || token[0] == '\n' )
    {
        fprintf( stderr, "No price was entered for some item in the "
                "items file \n" );
        return FALSE;
    }
    /*  
     *  Make sure that the character after the dollars amount is a
     *  decimal point
     */
    if( token[0] != '\0' && token[0] != '\n' && **end_token != 
            PRICE_DELIM )
    {
        fprintf( stderr, "Invalid format in price in items file. \n"
                "Something other than digits or a \".\" was "
                "encountered. 1111\n" );
        return FALSE;
    }
    if( !( dollars >= 0 && dollars < 100 ) )
    {
        fprintf( stderr, "And incorrect amount of dollars was found "
                " in a price in the items file. \n" );
        return FALSE;
    }
    /*
     * Note that this is the same as tokanising again
     */
    if( **end_token != '.' )
    {
        fprintf( stderr, "No \".\" was found in a price in the items "
                "file. \n" );
        return FALSE;
    }
    cents_at = ++*end_token;
    /*  
     *  Now we try to validate the cents
     */
    if( **end_token == '\n' || **end_token == '\0' )
    {
        fprintf( stderr, "No cents was found in the price in an item "
                "in the items file. \n" );
        return FALSE;
    }
    cents = strtol( *end_token, end_token, 10 );
    if( **end_token != '\0' )
    {
        fprintf( stderr, "Invalid format in price in items file. \n"
                "Something other than digits or a \".\" was "
                "encountered. 2222\n" );
        return FALSE;
    }
    if( !( *end_token - cents_at == 2 ) )
    {
        fprintf( stderr, "Invalid format in item file. \n"
                "The cents amount in some line was not two digits long"
                ". \n" );
        return FALSE;
    }
    if( cents == 0 && !( *cents_at >= '0' && *cents_at <= '9' ) )
    {
        fprintf( stderr, "Invalid format in price in items file. \n"
                "Something other than digits or a \".\" was "
                "encountered. 3333\n" );
        return FALSE;
    }
    if( !( cents >= 0 && cents < 100 && cents % 5 == 0 ) )
    {
        fprintf( stderr, "An invalid amount of cents was found for in "
                "the price for some item. \n" );
        return FALSE;
    }
    new_node->data->price.dollars = dollars;
    new_node->data->price.cents = cents;

    curr_node = vm->item_list->head;
    /*  If the head is not NULL  */
    if( curr_node )
    {
        /*
         *  While the curr_node is not NULL and the new_node goes after
         *  the curr one
         */
        while( curr_node && 
                strcmp( curr_node->data->name, new_node->data->name ) < 0)
        {
            prev_node = curr_node;
            curr_node = curr_node->next;
        }
        new_node->next = curr_node;
        /*
         *  If the curr_node is not the head
         */
        if( !( curr_node == vm->item_list->head ) )
        {
            prev_node->next = new_node;
        }
        /*  
         *  If the new_item goes AFTER something else, then we have
         *  added it already, but if it goes BEFORE everything else,
         *  then we need to add it.
         */
        else
        {
            vm->item_list->head = new_node;
        }
    }
    /*  If the head is NULL  */
    else
    {
        vm->item_list->head = new_node;
        new_node->next = NULL;
    }

    ++(vm->item_list->length);
    /*  
     *  Of course, we need to increment the next_id. We re-use "dollars"
     *  because we can 
     */
    set_next_id( vm, 0 );

    return TRUE;
}

/* 
 * Adds an item to the vending machine. This fulfills requirement 8 of
 * the spec.
 * */
BOOLEAN add_item(struct vm* vm)
{
    char name[NAMELEN+2],description[DESCLEN+2];
    char price[PRICELEN+2];
    /*  As in load_items we need a char ** and this does the job  */
    stock_item * new_item = malloc( sizeof( stock_item ) );
    vm_node * new_node = malloc( sizeof( vm_node ) );
    if( !new_item || !new_node )
    {
        return OS_SAID_FO;
    }

    printf( "The next item ID, if the item is successfully added, "
            "will be %s \n", vm->next_id );

    /*  Now let's get and validate the details  */
    /*  Start with the item name  */
    printf( "Please enter the item name:  " );
    fgets( name, NAMELEN+2, stdin );
    if( name[strlen(name)-1] != '\n' )
    {
        fprintf( stderr, "A name was entered with too many characters."
                "\n" );
        read_rest_of_line();
        free( new_item );
        free( new_node );
        return FALSE;
    }
    else
    {
        name[strlen(name)-1] = '\0';
    }
    /*  Now the description  */
    printf( "Please enter the item description:  " );
    fgets( description, DESCLEN+2, stdin );
    if( description[strlen(description)-1] != '\n' )
    {
        fprintf( stderr, "A description was entered with too many "
                "characters. \n" );
        read_rest_of_line();
        free( new_item );
        free( new_node );
        return FALSE;
    }
    else
    {
        description[strlen(description)-1] = '\0';
    }
    /*  Now for the hard part - the price  */
    printf( "Please enter the price ($$.cc or $.cc):  " );
    fgets( price, PRICELEN+2, stdin );
    if( price[strlen(price)-1] != '\n' )
    {
        fprintf( stderr, "A price was entered with too many characters."
                "\n" );
        read_rest_of_line();
        free( new_item );
        free( new_node );
        return FALSE;
    }
    else
    {
        price[strlen(price)-1] = '\0';
    }

    /*  Now everything is fine, let's add the new_item to the vm  */
    strcpy( new_item->id, vm->next_id );
    strcpy( new_item->name, name );
    strcpy( new_item->description, description );
    new_item->on_hand = DEFAULT_ONHAND;

    new_node->data = new_item;

    if( !val_price_and_add( vm, new_node, price ) )
    {
        free( new_item );
        free( new_node );
        return FALSE;
    }

    printf( "The item \"%s - %s\" has been added to the menu. \n",
            new_item->name, new_item->description );
    return TRUE;
}

/* 
 * Removes an item from the vending machine and frees all dynamically 
 * allocated memory as required. This fulfills requirement 9 of the 
 * spec.
 * */
BOOLEAN delete_item(struct vm* vm)
{
    char id[IDLEN+2];
    vm_node * curr_node, * prev_node;

    printf( "Enter the ID of the item to remove:  " );
    fgets( id, IDLEN+2, stdin );
    if( id[strlen(id)-1] != '\n' )
    {
        printf( "You entered too many characters for an ID. \n"
                "No item will be removed. \n" );
        read_rest_of_line();
        return FALSE;
    }
    else
    {
        id[strlen(id)-1] = '\0';
        curr_node = vm->item_list->head;
        while( curr_node )
        {
            if( strcmp( curr_node->data->id, id) == 0 )
            {
                if( curr_node == vm->item_list->head )
                {
                    printf( "\"%s - %s - %s\" has been removed from the" 
                            "system. \n", id, curr_node->data->name,
                            curr_node->data->description );
                    vm->item_list->head = curr_node->next;
                    free( curr_node->data );
                    free( curr_node );
                    --vm->item_list->length;
                    return TRUE;
                }
                else
                {
                    printf( "\"%s - %s - %s\" has been removed from the "
                            "system. \n", id, curr_node->data->name,
                            curr_node->data->description );
                    prev_node->next = curr_node->next;
                    free( curr_node->data );
                    free( curr_node );
                    --vm->item_list->length;
                    return TRUE;
                }
            }
            prev_node = curr_node;
            curr_node = curr_node->next;
        }
        printf( "No item has that ID.  Please try again. \n" );
        return FALSE;
    }

    --(vm->item_list->length);
    return TRUE;
}

/* 
 * Sets the count of each stock item to the default value. This fulfills 
 * requirement 10 of the spec.
 * */
BOOLEAN reset_stock_count(struct vm* vm)
{
    struct vm_node * curr_node = vm->item_list->head;

    while( curr_node )
    {
        curr_node->data->on_hand = DEFAULT_ONHAND;
        curr_node = curr_node->next;
    }

    return FALSE;
}
