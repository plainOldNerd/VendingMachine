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

#ifndef VM_STRUCTS_ENUMS_DEFS
#define VM_STRUCTS_ENUMS_DEFS

/*************************************************************************
 *
 *   Items
 *
 ************************************************************************/
#define IDLEN 5
#define NAMELEN 40
#define DESCLEN 255
#define PRICELEN 5
#define NOH 2
#define DEFAULT_ONHAND 20
/*  Note that we have +4 here to account for the ITEM_DELIM charaters
 */
#define CHARSperLINE IDLEN+NAMELEN+DESCLEN+PRICELEN+NOH+4
#define ITEM_TOKENS 5
#define ITEM_DELIM "|"
/*  Note that we do not use strtok to split dollars from cents.
 *  We just increment a char pointer and then compare the char
 */
#define PRICE_DELIM '.'
#define ID_DELIM "I"

struct price
{
    unsigned dollars, cents;
};

struct stock_item
{
    char id[IDLEN+1];
    char name[NAMELEN+1];
    char description[DESCLEN+1];
    struct price price;
    unsigned on_hand;
};

typedef struct price price;
typedef struct stock_item stock_item;

/*************************************************************************
 *
 *   Money
 *
 ************************************************************************/
#define NUMDENOMS 8
#define COIN_COUNT 20
#define DIGITSperLINE 8
#define MONEY_DELIM ","

enum denomination
{
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR,
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};

struct coin
{
    enum denomination denom;
    /*  
     *  This will be initialised to -1 for the purpose of validation when
     *  scanning from file
     */
    int count;
    /*  Thihis will make a lot of things a lot easier!!  */
    unsigned value;
};

typedef enum denomination denomination;
typedef struct coin coin;

/************************************************************************
 *
 *   Vending Machine stuff
 *
 ***********************************************************************/
struct vm_node
{
    struct stock_item * data;
    struct vm_node * next;
};

struct vm_list
{
    struct vm_node * head;
    unsigned length;
};

struct vm
{
    struct vm_list * item_list;
    struct coin coins[NUMDENOMS];
    /*  Get rid of "incompatible type" warnings  */
    const char * foodfile;
    const char * coinsfile;
    /*
     *  In order to make sure that item IDs are never re-used, we need the
     *  next_id to be a kind of a global variable.  The only way to do that
     *  is to have it be a part of the vm struct.
     */
    char next_id[IDLEN+1];
};

typedef struct vm_node vm_node;
typedef struct vm_list vm_list;
typedef struct vm vm;

#endif
