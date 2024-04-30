#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Required Defined Vals
#define MAXLEN 19
#define TABLESIZE 300007

// Declaration Of Global Variables
int Complexity = 0;
int totalCash = 100000;


// Required Structs and Funcs
typedef struct item {
    char name[MAXLEN + 1];
    int quantity;
    int saleprice;
} item;

typedef struct node {
    item * iPtr;
    struct node * next;
} node;

typedef struct hashtable {
    node ** lists;
    int size;
} hashtable;

int hashfunc(char * word, int size) {
    int len = strlen(word);
    int res = 0;
    for (int i = 0; i < len; i++) {
        res = (1151 * res + (word[i] - 'a')) % size;
    }
    return res;
}


// InitializeTable Func to Create, ALlocate Memory, Initialize, and Return a HashTable Variable

hashtable * initializeTable() {
    hashtable * myTable = malloc(sizeof(hashtable));
    myTable->size = TABLESIZE;
    myTable->lists = malloc(sizeof(node *) * TABLESIZE);
    for (int i = 0; i < TABLESIZE; i++) {
        myTable->lists[i] = NULL;
    }
    return myTable;
}

// SearchItem Func to Take in HashTable Var of Use and Name, and Call HashFunc to Obtain Index 
// at which the Name would be found, if value returned is NOT NULL Increment Complexity And
// Compare Name of Parameter to Name at Node, if val returned is 0, returns Item, Else, "Increments"
// Linked List down another Node to check.

item * searchItem (hashtable * myTable, char * name) {
    int atNode = hashfunc(name, myTable->size);
    node * list = myTable->lists[atNode];
    while (list != NULL) {
        Complexity++;
        if (strcmp(list->iPtr->name, name) == 0) { 
            return list->iPtr;
        }
        list = list->next;
    }
    return NULL;
}

// createItem Func to Take in Name, Quantity, Price that Allocates Memory for Item, Initializes Name,
// Quantity, and SalePrice vals based on Vals in Parameters then returns created Item

item * createItem (char * name, int quant, int price) {
    item * myItem = malloc(sizeof(item));
    strcpy(myItem->name, name);
    myItem->quantity = quant;
    myItem->saleprice = price;
    return myItem;
}

// insertItem to Take in HashTable Var and Item, Calls HashFunc to obtain Index of which Item should be 
// located, then Allocates Memory for New Node, and Initializes Node's value to Item from Parameter, and
// Sets Node Pointer to newly created Node (myNode), then increments Complexity per call.

void insertItem(hashtable * myTable, item * item) {
    int atNode = hashfunc(item->name, myTable->size);
    node * myNode = malloc(sizeof(node));
    myNode->iPtr = item;
    myNode->next = myTable->lists[atNode];
    myTable->lists[atNode] = myNode;
    Complexity++;
}

// buyItem to take in HashTable var, Name, Quantity, Price, then Call searchItem to verify Item exists and 
// return found Item, if an Item is found, add Newly Bought Qty to Current Qty, if an Item is not found, 
// Call createItem which will return a new Item Pointer, of which we will insert into insertItem func which
// will insert Item at designated HashValue, then return newItem

item * buyItem(hashtable * myTable, char * name, int quant, int price) {
    item * myItem = searchItem(myTable, name);
    if (myItem != NULL) { 
        myItem->quantity += quant;
        return myItem;
    } else {
        item * newItem = createItem(name, quant, 0);
        insertItem(myTable, newItem);
        return newItem;
    }
}

// sellItem func to Take in Hashtable var, Name, Quantity. Then Call searchItem to verify Item exists and
// then return Item, if Item IS found, if Quantity in "Store" is greater than Quantity to be Purchased, then 
// subtracts Quantity to Buy from Current Quantity, and Calculates Cash to be Returned to Global Var, then
// return Item, Else, if Quantity in "Store" is less than Quantity to be Purchased, then sells Quantity that
// is available then returns Item

item * sellItem(hashtable * myTable, char * name, int quant) { 
    item * myItem = searchItem(myTable, name);
    if (myItem != NULL) {
        if (myItem->quantity - quant >= 0) {
            totalCash += quant * myItem->saleprice;
            myItem->quantity -= quant;
            return myItem;
        } else {
            totalCash += myItem->quantity * myItem->saleprice;
            myItem->quantity = 0;
            return myItem;
        }

    }
}

// changePrice func to Take in Hashtable var, Name, Price, Search Item to verify it exists, If the Item exists
// Sets Price to Price Inputted by User then returns Item

item * changePrice(hashtable * myTable, char * name, int price) {
    item * myItem = searchItem(myTable, name);
    if(myItem != NULL) {
        myItem->saleprice = price;
        return myItem;
    }
}


int main() {
    // Initializes hashTable
    hashtable * myTable = initializeTable();

    // Takes input for Number of Lines to take Input
    int numCmds = 0;
    scanf("%d", &numCmds);

    // Variable Declarations
    char userInput[MAXLEN + 1];
    char itemName[MAXLEN + 1];
    int totalPrice, itemQuant, newPrice = 0;

    // While numCmds is greater than 0
    while(numCmds != 0) {
        // Take String Input for Requested Cmd
        scanf("%s", userInput);

        // If Cmd is Buy
        if( strcmp(userInput, "buy") == 0) {

            // Take input for Name, Quantity, and Price of Items
            scanf("%s %d %d", itemName, &itemQuant, &totalPrice);

            // Calls buyItem func and sets returned value to myItem
            item * myItem = buyItem(myTable, itemName, itemQuant, totalPrice);

            // If Cash on Hand minus Price of Items is less than 0, Price Goes to 0
            if (totalCash - totalPrice < 0) {
                totalCash = 0;
            } 
            else  { // Else Price subtracts from total Cash
                totalCash -= totalPrice;
            }

            // Prints out Values to User
            printf("%s %d %d\n", itemName, myItem->quantity, totalCash);

        } else if ( strcmp(userInput, "sell") == 0) {
            // Take Input for Name, and Quantity to Sell
            scanf("%s %d", itemName, &itemQuant);
            
            // Call sellItem and set returned Item to myItem
            item * myItem = sellItem(myTable, itemName, itemQuant);
            
            // Print out Values to User
            printf("%s %d %d\n", itemName, myItem->quantity, totalCash);

        } else if ( strcmp(userInput, "change_price") == 0) {
            // Take Input for Name, and Price to Set Item to
            scanf("%s %d", itemName, &newPrice);

            // Call changePrice and set returned item to myItem
            item * myItem = changePrice(myTable, itemName, newPrice);
        }

        // Decrement numCmds
        numCmds--;
    }


    // Display totalCash on Hand and total Complexity after All Cmds to User
    printf("%d\n", totalCash);
    printf("%d\n", Complexity);

    return 0;
}
