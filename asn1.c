// CS1 - ASN1 - Theater Reservation.
// Dynamic Memory Application.


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Reservation Reservation;
typedef struct Row Row;
typedef struct Theater Theater;

#define INITIAL_ROW_CAP 2 // Initial Row Cap
#define MAX_RES 100000 // Max reservations
#define MAX_NAME 50 // Max name length

struct Reservation 
{
    int start, end; // Start and end of the reservation
    char * name; // Name of the person who reserved this range
};

struct Row 
{
    struct Reservation * array; // The array of reservations
    int max_reservations; // The spots in the array
    int num_reservations; // Occupied spots in the arry
};

struct Theater
{
    struct Row * row; // The array of rows in the theater
};


// Function that initializes the values of variables within * row struct
void initializeRow(Row * row)
{
    row->max_reservations = INITIAL_ROW_CAP; // Sets max_reservations to 2 which will be expanded later
    row->num_reservations = 0; // Initializes number of reservations to 0
    row->array = (Reservation *) malloc(row->max_reservations * sizeof(Reservation)); // Allocates enough memeory to array struct within row
    for (int i = 0; i < row->max_reservations; i++)
    {
        row->array[i].name = NULL; // Initializes pointer in array to NULL
    }
}


// Function that returns an int, 1 if requested seat (start and end) in a specific row are unobstructed by another value
// and will accept user inputted values into that reservation struct, else if int is 0 then will call break to return to main
int makePurchase(Theater * myTheater, int rowNum, int start, int end, char * name, int * numRows)
{
    rowNum--; // Decrement RowNum to account for indexing format

    // Checking Memory for Rows and if ROW is NULL or user requested row is less than current row count then allocate and initializa
    if(rowNum >= *numRows) 
    {
        myTheater->row = (Row *) realloc(myTheater->row, (rowNum + 1) * sizeof(Row)); // Allocates more memory to account for new or expanding row
        for(int i = *numRows; i <= rowNum; i++)
        {
            initializeRow(&myTheater->row[i]); // Initializes all values within specific row
        }
        *numRows = rowNum + 1; // Increment number of rows by userInputted row
    }

    // If num_reservations exceeds the max_reservations then allocate more memory and double max_reservations
    if(myTheater->row[rowNum].num_reservations >= myTheater->row[rowNum].max_reservations)
    {
        myTheater->row[rowNum].max_reservations = myTheater->row[rowNum].max_reservations * 2; 
        myTheater->row[rowNum].array = (Reservation *) realloc(myTheater->row[rowNum].array, myTheater->row[rowNum].max_reservations * sizeof(Reservation));

    }
    if(myTheater->row[rowNum].num_reservations == MAX_RES)
    {
        return 0;
    }

    // Iterate through Theater row then through array and verifies value CAN fit within the array of "seats", returns 0 if not
    // and breaks loop if "seat" can be sat
    for(int i = 0; i < myTheater->row[rowNum].num_reservations; i++)
    {
        if(start >= myTheater->row->array[i].start && start <= myTheater->row->array[i].end && end >= myTheater->row->array[i].end)
        {
            return 0;
        }
        else if(end >= myTheater->row->array[i].start && end <= myTheater->row->array[i].end && start <= myTheater->row->array[i].start)
        {
            return 0;
        }
        else if(end >= myTheater->row->array[i].start && end <= myTheater->row->array[i].end && start >= myTheater->row->array[i].start && start <= myTheater->row->array[i].end)
        {
            return 0;
        }
        else if(end >= myTheater->row->array[i].start && end >= myTheater->row->array[i].end && start <= myTheater->row->array[i].start && start <= myTheater->row->array[i].end)
        {
            return 0;
        }
        else
        {
            break;
        }
    }

// Sets a reservation and accepts user values into
    Reservation myReservation;
    myReservation.start = start;
    myReservation.end = end;
    myReservation.name = strdup(name);

// Increments num of reservations up
    myTheater->row[rowNum].array[myTheater->row[rowNum].num_reservations++] = myReservation;

    
    return 1; //Returns success value
}

int main()
{
    // Declare Initials
    Theater myTheater;
    myTheater.row = NULL;
    int numRows = 0;
    char userInput[20] = "NotImportant"; 

    while (strcmp(userInput, "QUIT") != 0) // While loop to check if UserInput is not QUIT, if so then loop ends
    {
        scanf("%s", userInput); // Take User Input of Wanted Function
        if(strcmp(userInput,"BUY") == 0) // If userInput = BUY
        {
            int rowUI, startUI, endUI; // Declarations
            char nameUI[MAX_NAME + 1];
            scanf("%d %d %d %s", &rowUI, &startUI, &endUI, nameUI); // Accept rest of User Input for BUY function
            int value = makePurchase(&myTheater, rowUI, startUI, endUI, nameUI, &numRows); // Call makePurchase function and set value equal to returned int
            if (value == 1) 
            {
                printf("SUCCESS\n"); 
            }
            else if (value == 0)
            {
                printf("FAILURE\n");
            }
        }
        else if(strcmp(userInput, "LOOKUP") == 0) //IF userInput = LOOKUP
        {
            int rowUI, seatUI; // Declarations
            scanf("%d %d", &rowUI, &seatUI); // Take userInput
            rowUI--; // Decrement value so it is accurate within the index array
            if(myTheater.row != NULL && rowUI < numRows) // IF theater DNE or rowUI is within the cap of number of reservations
            {
                int found = 0; // Declare integer to check if name was found
                for (int currentSeat = 0; currentSeat < myTheater.row[rowUI].num_reservations; currentSeat++) // Begin for loop to iterate through every seat
                {
                    if(seatUI >= myTheater.row[rowUI].array[currentSeat].start && seatUI <= myTheater.row[rowUI].array[currentSeat].end) // If requested seat is within the boundaries of another seating group
                    {
                        printf("%s\n", myTheater.row[rowUI].array[currentSeat].name); // Print the name of the person in seat
                        found = 1; 
                        break; //Break loop
                    }
                    
                } 
                if (found == 0) // If no one found prints No one
                { 
                        printf("No one\n");
                }
            }
            else // If no row found print No one
            {
                printf("No one\n");
            }
        }



    }



    // Uses a 2D array to iterate through every index within both structs and free them moving outward from the array of reservations to rows to the entire theater
    for (int i = 0; i < numRows; i++) 
    {
        for (int j = 0; j < myTheater.row[i].num_reservations; j++) 
        {
            free(myTheater.row[i].array[j].name);
        }
        free(myTheater.row[i].array);
    }
    free(myTheater.row); 

    return 0;
}
