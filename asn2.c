// CS1 - ASN2 - Queues.
// Implementing Queues w/ Dynammic Memory.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Customer Customer;
typedef struct Queue Queue;
typedef struct Node Node;

#define MAXQUEUE 12
#define NAME 50

// Customer Struct
struct Customer {
    char name[NAME + 1];
    int numberTix;
    int lineNumber;
    int arrivalTime;
};

// Node Struct
struct Node {
    struct Customer * customerNode;
    struct Node * curNode;
};

// Queue Struct
struct Queue {
    int sizeOfQueue;
    struct Node * front;
    struct Node * end;
};

// Func to initializeQueue
void initializeQueue(Queue * myQueue, int numQueues)
{
    for(int i = 0; i < numQueues; i++)
    {
        myQueue[i].front = NULL;
        myQueue[i].end = NULL;
        myQueue[i].sizeOfQueue = 0;
    }
}

// Func to dynamically allocate, fill out and return the customer
Customer * createCustomer(char * name, int numberTix, int arrivalTime)
{
    Customer * myCustomer = (Customer *) malloc(sizeof(Customer));
    strcpy(myCustomer->name, name);
    myCustomer->numberTix = numberTix;
    myCustomer->arrivalTime = arrivalTime;
    
    return myCustomer;
}

// Func to add customer to queue
void enqueue(Queue * myQueue, Customer * myCustomer)
{
    Node * node = (Node *) malloc(sizeof(Node)); // Allocates memory for this node

    node->customerNode = myCustomer; // Initializes node to the custoemr
    node->curNode = NULL;

    if(myQueue->end == NULL)
    {
        myQueue->front = myQueue->end = node;
    }
    else
    {
        myQueue->end->curNode = node;
        myQueue->end = node;
    }
    myQueue->sizeOfQueue++;
};

// Func to remove customer from queue
void dequeue(Queue * myQueue)
{
    if(myQueue->front == NULL)
    {
        return;
    }

    Node * tempNode = myQueue->front;
    myQueue->front = myQueue->front->curNode;

    if (myQueue->front == NULL)
    {
        myQueue->end = NULL;
    }
    myQueue->sizeOfQueue--;
    free(tempNode);
};

// Func to check the front of the queue without dequeueing
Node * peek(Queue * myQueue)
{
    if (myQueue->front == NULL)
    {
        return NULL;
    }

    else 
    {
    return myQueue->front;
    }
};

// Func to return value if queue is empty
int checkEmpty(Queue * myQueue)
{
    if(myQueue->sizeOfQueue > 0)
    {
        return 0; //Return 0 if size of queue is greater than 0
    }
    else 
    {
        return 1;
    }
};

// Func to return size of queue
int sizeOf(Queue * myQueue){
    return myQueue->sizeOfQueue;
};

int main(){
    // Take input of n (number of customers) and b (number of open booths)
    int n, b;
    scanf("%d %d", &n, &b);

    // Declare and initialize each line
    Queue myQueue[MAXQUEUE];
    initializeQueue(myQueue, MAXQUEUE);

    Customer * myCustomer[n];
    // Take input for n number of customers
    for(int i = 0; i < n; i++){
        int numberTix, arrivalTime = 0;
        char name[NAME + 1];
        scanf("%s %d %d", name, &numberTix, &arrivalTime);

        // Declare customer and save to Customer array
        myCustomer[i] = createCustomer(name, numberTix, arrivalTime);

        int p = name[0] - 'A';
        int queueNumber = 0;
        
        if (p % 13 != 0){
            queueNumber = p % 13;
        }
        else if (p % 13 == 0){
            int flag, firstNonEmptyQueue = 0;
            for(int j = 0; j < MAXQUEUE; j++)
            {
                if(checkEmpty(&myQueue[j]) == 0)
                {
                    // If any queue has a size greater than 0, flag = 1, then loop breaks
                    flag = 1;
                    firstNonEmptyQueue = j; // Sets equal to real line number instead of index
                    break;
                }
            }
            int minQueue = myQueue[firstNonEmptyQueue].sizeOfQueue;
            if (flag == 1)
            { // If there is a non-empty queue 
                for (int k = 0; k < MAXQUEUE; k++)
                {
                    // if sizeOfQueue is smaller than next smallest queue and sizeOfQueue is greater than 0
                    if((myQueue[k].sizeOfQueue < minQueue) && (myQueue[k].sizeOfQueue != 0))
                    {
                        minQueue = myQueue[k].sizeOfQueue; // Sets minQueue to next least queue
                        queueNumber = k + 1; // Sets queueNumber equal to real line number
                    }
                }

            }
            else if (flag == 0) 
            { // If all queues are empty, queueNumber for customer is first line
                queueNumber = 1;
            }

            for (int j = 0; j < MAXQUEUE; j++)
            { // If at any point the min queue is equal to another queue
                if(myQueue[j].sizeOfQueue == minQueue)
                { // Will set qN to first appearance then break
                    queueNumber = j + 1;
                    break;
                }
            }
        }

        myCustomer[i]->lineNumber = queueNumber; // Sets the line number of the custoemr to queueNumber returned from forloop checkers
        enqueue(&myQueue[queueNumber-1], myCustomer[i]); // Adds customer to that queue
    }

    
    int numNEQ = 0; // Declares and obtains number of non-empty queues
    for(int i = 0; i < MAXQUEUE; i++)
    {
        if(checkEmpty(&myQueue[i]) == 0)
        {
            numNEQ++;
        }
    }

    // Declares values for both number of non-empty queues and remainder of queues
    int minNQPB = numNEQ / b; 
    int remNQPB = numNEQ % b;

    // Initializes another queue for booth
    Queue myBooth[b];
    initializeQueue(myBooth,b);

    // Sets the queue length for each booth
    for(int i = 0; i < b; i++)
    {
        myBooth[i].sizeOfQueue = minNQPB;
        if (remNQPB != 0)
        {
            myBooth[i].sizeOfQueue++;
            remNQPB--;
        }
    }

    // Creates an array for all of the queues
    int x = 0;
    int neqArr[numNEQ];
    for(int i = 0; i < MAXQUEUE; i++)
    {
        if(checkEmpty(&myQueue[i]) == 0)
        { // For all non-empty queues, will take the index
            neqArr[x] = i + 1; // And save it to the array
            x++; // Then increment up the array index
        }
    }

    // Declaration and intializations of previously used vales 
    int bIndex = 0;
    int nqLeft = numNEQ;
    int queueNum = 0;

    // Iterates through and enqueues all customers within myQueues[j] to corresponding queues 1, 2, and 3 within booth[i]
    for (int i = 0; i < b && bIndex < numNEQ; i++) 
    {
        if (bIndex < numNEQ) {
            int boothSize = b;

            if (bIndex == b - 1) 
            {
                boothSize = nqLeft;
            }
            for (int j = 0; j < boothSize; j++) 
            {
                int queueNum = neqArr[bIndex];
                while (sizeOf(&myQueue[queueNum - 1]) > 0) 
                {
                    Node *customerAtFront = peek(&myQueue[queueNum - 1]);
                    enqueue(&myBooth[i], customerAtFront->customerNode);
                    dequeue(&myQueue[queueNum - 1]);
                }
                bIndex++;
            }

            if (bIndex == b -1) 
            {
                nqLeft = nqLeft - boothSize;
            }
        }
    }

    // Nested loop to move the customerNodes from myBooth into a customerArray so I could then organize them based on time in an easier manner
    for (int i = 0; i < b; i++) 
    {
        // Creates new node
        Node * myNode = myBooth[i].front;
        Customer * myCustomers[MAXQUEUE];
        int numCus = 0;

        // Adds all existing customers per booth into an arr
        while (myNode != NULL) {
            myCustomers[numCus] = myNode->customerNode;
            myNode = myNode->curNode;
            numCus++;
        }

        // Then now that the customers are in an easily accesible array again I can sort them by arrival time using a double nested for loop
        for (int j = 0; j < numCus - 1; j++) 
        {
            for (int k = 0; k < numCus - j - 1; k++) 
            {
                if (myCustomers[k]->arrivalTime > myCustomers[k + 1]->arrivalTime) 
                {
                    Customer * tmp = myCustomers[k];
                    myCustomers[k] = myCustomers[k + 1];
                    myCustomers[k + 1] = tmp;
                }
            }
        }

        // Clear the myBooth to allow for customers to be entered back in but this time sorted by arrivalTime
        myBooth[i].front = myBooth[i].end = NULL;
        myBooth[i].sizeOfQueue = 0;

        // Add customers back into queue but sorted
        for (int j = 0; j < numCus; j++) 
        {
            enqueue(&myBooth[i], myCustomers[j]);
        }
    }



    int outTime[b]; // Use an array to hold vals for checkout time because of the possiblity of some being cumulative

    // Initialize arr to 0 
    for (int i = 0; i < b; i++) 
    {
        outTime[i] = 0;
    }
    
    // For loop to begin "checking out" customers
    for (int i = 0; i < b; i++) // For i < number of booths
    {
        printf("Booth %d:\n", i + 1); // Print booth number(not index)

        while (checkEmpty(&myBooth[i]) == 0) // While booth isn't empty
        {
            Node *customerAtFront = peek(&myBooth[i]); // Take customer at front of Booth's queue
            Customer *inCus = customerAtFront->customerNode; // Save customer from node

            int outTimePC; // Declares outTime for customer[i]

            if (outTime[i] > inCus->arrivalTime)  // If customer is not first customer
            {
                outTimePC = outTime[i]; // Time will be cumulative instead based off previous customer Time
            } 
            else  // If customer is first customer
            {
                outTimePC = inCus->arrivalTime; // Declares customer arrivalTime 
            }

            outTimePC = outTimePC + 30 + (inCus->numberTix * 5); // Solves for checkout tiem

            printf("%s from line %d checks out at time %d.\n", inCus->name, inCus->lineNumber, outTimePC);

            outTime[i] = outTimePC; // Re-declare checkout time for next possible customer

            dequeue(&myBooth[i]);
        }
        printf("\n");
    }

    // Free customer array memory
    for (int i = 0; i < n; i++) 
    {
        free(myCustomer[i]);
    }

    // Free booth's queue array memory
    for (int i = 0; i < b; i++) 
    {
        while (checkEmpty(&myBooth[i]) == 0) 
        {
            Node * customerAtFront = peek(&myBooth[i]);
            dequeue(&myBooth[i]);
            free(customerAtFront->customerNode);
            free(customerAtFront);
        }
    }

    // Free myQueue memory
    for (int i = 0; i < MAXQUEUE; i++)
     {
        while (checkEmpty(&myQueue[i]) == 0) 
        {
            Node * customerAtFront = peek(&myQueue[i]);
            dequeue(&myQueue[i]);
            free(customerAtFront->customerNode);
            free(customerAtFront);
        }
    }


    return 0; // Exit progarm
}

