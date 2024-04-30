// CS1 - ASN5 - Binary Trees.
// Implementing a Loyalty Point Program within a Binary Tree.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>




// Structs and Defs provided by Meade

#define MAXLEN 19

typedef struct customer {
    char name [MAXLEN + 1];
    int point;
} customer;

typedef struct treenode { 
    customer * cPtr;
    int size;
    struct treenode * left;
    struct treenode * right;
} treenode;


// Functions

void changeNodeSize(treenode * head){

    // Adjust node size through process of:
    // - Start out size of node at 1
    // - Checks left node, if not NULL will increment up based on size of child node
    // - Checks right node, if not NULL will increment up based on size of child node

    head->size = 1;

    if(head->left != NULL) { 
        head->size += head->left->size;
    }

    if(head->right != NULL) {
        head->size += head->right->size;
    }
}

treenode * addNode(treenode * head, char name[MAXLEN + 1], int points){
    // If Tree is Empty, Insert node at root, Malloc, and Initialize
    if (head == NULL) {
        treenode * cur = (treenode *) malloc(sizeof(treenode));
        cur->cPtr = (customer *)malloc(sizeof(customer));
        cur->left = NULL;
        cur->right = NULL;
        cur->size = 1;
        strcpy(cur->cPtr->name, name);
        cur->cPtr->point = points;
        printf("%s %d\n", cur->cPtr->name, cur->cPtr->point);
        return cur;
    }

    // Comparing User Inputted String with String at Root
    // - If string is LEX. before then recur addNode for Left Node
    // - If string is LEX. after then recur addNode for Right Node
    // - If string is LEX. same then add points 

    if(strcmp(name, head->cPtr->name) < 0){
        head->left = addNode(head->left, name, points);
    }
    else if (strcmp(name, head->cPtr->name) > 0){
        head->right = addNode(head->right, name, points);
    }
    else {
        head->cPtr->point += points;
        printf("%s %d\n", head->cPtr->name, head->cPtr->point);

    }

    // Properly change size of node through func changNodeSize
    changeNodeSize(head);

    // Return node
    return head;
}

treenode * searchNode(treenode * head, char name[MAXLEN + 1]){
    // Base Case
    if (head == NULL) return NULL;

    // Comparing User Inputted String with String at Root
    // - If string is LEX. before then recur searchNode for Left Node
    // - If string is LEX. after then recur searchNode for Right Node
    // - If string is LEX. same then return root

    if (strcmp(name, head->cPtr->name) < 0) {
        return searchNode(head->left, name);
    } else if (strcmp(name, head->cPtr->name) > 0) {
        return searchNode(head->right, name);
    } else if (strcmp(name, head->cPtr->name) == 0) { 
        return head;
    } else {
        return NULL;
    }
}

int depthNode(treenode * head, char name[MAXLEN + 1], int curDepth){
    // Base Case returning -1 because 0 is an applicable return value
    if (head == NULL) return -1;

    // Comparing User Inputted String with String at Root
    // - If string is LEX. same then return the current Depth
    // - If string is LEX. before then recur depthNode for Left Node with the Depth incremented by 1
    // - If string is LEX. after then recur depthNode for Right Node with the Depth incremented by 1

    if (strcmp(name, head->cPtr->name) == 0){
        return curDepth;
    } else if (strcmp(name, head->cPtr->name) < 0) { 
        return depthNode(head->left, name, curDepth + 1);
    } else if (strcmp(name, head->cPtr->name) > 0) {
        return depthNode(head->right, name, curDepth + 1);
    }

    // Return -1 if NO statement above responds, SHOULD never be called anyway however
    return -1;
}

void subNode(treenode * head, char name[MAXLEN + 1], int subpoints){
    // Call searchNode function to check if name exists in tree
    treenode * ret = searchNode(head, name);

    // If node does NOT exist, print " *Name* is not found"
    // If node does exist
    // - If the points subtracted is greater than number of points at node, 
    //   - Points at node = 0, print Name and Points
    // - If the points subtracted is less than number of points at node
    //   - Points at node = Points at Node - SubPoints, print Name and Points

    if (ret == NULL) {
        printf("%s not found\n", name);
    } else {
        if (ret->cPtr->point < subpoints){
            ret->cPtr->point = 0;
            printf("%s %d\n", ret->cPtr->name, ret->cPtr->point);
        }
        else {
            ret->cPtr->point -= subpoints;
            printf("%s %d\n", ret->cPtr->name, ret->cPtr->point);
        }
    }
}

treenode * bottomRightMostNode(treenode * head) {
    // Base Case
    if (head == NULL) return NULL;
    
    // If the child of the head is NULL, then return that node as it is the Bottom Most Node to the Right
    // Else recur bottomRightMostNode

    if (head->right == NULL) return head;
    return bottomRightMostNode(head->right);
}

void forestFire(treenode * head) {
    // Modified Memory Clear Borrowed from Andrew SI Leader

    // Base Case
    if (head == NULL) return;

    // Recur forestFire for all left Nodes
    // Recur forestFire for all right Nodes
    forestFire(head->left);
    forestFire(head->right);

    // Free memory allocated earlier for customer and node Struct
    free(head->cPtr);
    free(head);
}

treenode * deleteNode(treenode * head, char name[MAXLEN + 1]){
    // Base Case
    if (head == NULL) return NULL;

    // Comparing User Inputted String with String at Root
    // - If string is LEX. before then recur deleteNode for Left Node
    // - If string is LEX. after then recur deleteNode for Right Node
    // - If string is LEX. same then 
    //   - If Left Child is NULL (One Child Scenario), set TMP node as Right Child, Free Memory for Right Child
    //   - If Right Child is NULL (One Child Scenario), set TMP node as Left Child, Free Memory for Left Child
    //   - Else (Two Child Scenario), call bottomRightMostNode to find the proper Predecessor, then swap that Node with bottomRightMostNode
    //     then recur deleteNode to properly get rid of the previous Node that had the two children

    if (strcmp(name, head->cPtr->name) < 0) { 
        head->left = deleteNode(head->left, name);

    } else if (strcmp(name, head->cPtr->name) > 0) { 
        head->right = deleteNode(head->right, name);

    } else if (strcmp(name, head->cPtr->name) == 0) { 
        if (head->left == NULL){
            treenode * tmp = head->right;
            free(head->cPtr);
            free(head);
            return tmp;
        } else if (head->right == NULL){
            treenode * tmp = head->left;
            free(head->cPtr);
            free(head);
            return tmp;
        } else {
            treenode * tmp = bottomRightMostNode(head->left);

            customer * tmpCustomer = head->cPtr;
            head->cPtr = tmp->cPtr;
            tmp->cPtr = tmpCustomer;

            head->left = deleteNode(head->left, tmp->cPtr->name);
        }
    }
    
    // Properly change the size of Node through func changeNodeSize
    changeNodeSize(head);

    // Return the current Node
    return head;
}

void printInOrder(treenode * head){
    // Served DEBUGGING purposes

    // Base Case
    if (head == NULL) return;

    // In Order Traversal Print Process
    printInOrder(head->left);
    printf("%s %d\n", head->cPtr->name, head->cPtr->point);
    printInOrder(head->right);
}

int countSmallerNode(treenode * head, char name[MAXLEN + 1]){
    // Base Case
    if (head == NULL) return 0;

    // Comparing User Inputted String with String at Root
    // - If string is LEX. before then 
    //   - If Left Child is not NULL take size of Left Child as it holds size for Rest of Subtree
    //   - Then since current node being looked at is LEX. before then increment Size by 1
    //   - Then recur countSmallerNode for Right subtree within Left Child
    //   - Then return Size
    // - Else
    //   - Recur countSmallerNode for Left Child

    if (strcmp(head->cPtr->name, name) < 0) {
        int size = 0;
        if (head->left != NULL) {
            size = head->left->size;
        }
        size = size += 1;
        size += countSmallerNode(head->right, name);
        return size;
    } else {
        return countSmallerNode(head->left, name);
    }
}

void changeToArray(treenode * head, treenode ** arr, int * index){
    // Base Case
    if (head == NULL) return;

    // Recur changeToArray for Left Subtrees
    changeToArray(head->left, arr, index);

    // Per Node in Tree that is being created in the Array
    // Insert node at Index, Malloc, and Initialize using a Ptr for Index to maintain value through Recursion

    arr[*index] = (treenode *) malloc(sizeof(treenode));
    arr[*index]->cPtr = (customer *)malloc(sizeof(customer));
    arr[*index]->left = NULL;
    arr[*index]->right = NULL;
    arr[*index]->size = 1;
    strcpy(arr[*index]->cPtr->name, head->cPtr->name);
    arr[*index]->cPtr->point = head->cPtr->point;
    (*index)++;

    // Recur changeToArray for Right Subtrees
    changeToArray(head->right, arr, index);
}

void sortSize(treenode ** forest, int n){
    // Modified Merge Sort Function Borrowed from Meade

    // Base Case
    if (n <= 1) return;

    // Compute the number of half the array
    int n2 = n / 2;

    // Recur sortSize for First Half of Array
    // Recur sortSize for Second Half of Array
    sortSize(forest, n2);
    sortSize(forest + n2, n - n2);

    // Allocating memory for Tmp Treenode * Array
    treenode ** tmp = (treenode **) malloc (sizeof(treenode *) * n);

    // Declaring Front and Back Pointers
    int fptr = 0;
    int bptr = n2;

    for (int i = 0; i < n; i++){
        if (fptr == n2){
            // Front is empty
            tmp[i] = forest[bptr];
            bptr++;
        } else if (bptr == n){
            // Back is empty
            tmp[i] = forest[fptr];
            fptr++;
        } else if (forest[fptr]->cPtr->point < forest[bptr]->cPtr->point){
            // Front was smaller than the back
            tmp[i]= forest[fptr];
            fptr++;
        } else {
            // Back was good enough
            tmp[i] = forest[bptr];
            bptr++;
        }
    } 

    // Move the Tmp Values into Arr
    for (int i = 0; i < n; i++){
        forest[i] = tmp[i];
    }

    // Free Tmp Array
    free(tmp);
    
}

int main () {

    // Take integer for number of lines of input (numLines)
    int numLines;
    scanf("%d", &numLines);

    // Variable Declarations
    char prompt[MAXLEN];
    treenode * head = NULL;
    char name[MAXLEN + 1];
    int points;

    // For i < Number of Lines Inputted by User
    // Take String Input for Prompt
    // - If Prompt is Add
    //   - Take Name and Number of Points, Call addNode set returned value equal to Head Node
    // - If Prompt is Sub
    //   - Take Name and Points to Subtract, Call subNode
    // - If Prompt is Del
    //   - Take name, Call deleteNode, Output Deleted Node's Name
    // - If Prompt is Search
    //   - Take Name, Call searchNode and set returned value equal to Tmp, then Call depthNode, thne Output Name, Points, Depth
    // - If Prompt is Count_Smaller
    //   - Call countSmallerNode and set equal to Res, then Output Res
    //
    // After all iterations, take all nodes of tree and input into an array, then sort array by Points, and output Array in Descending Order
    // call forestFire to free Memory 

    for (int i = 0; i < numLines; i++){
        scanf("%s", prompt);
        if (strcmp(prompt, "add") == 0){
            
            scanf("%s %d", name, &points);
            head = addNode(head, name, points);

        } else if (strcmp(prompt, "sub") == 0){

            scanf("%s %d", name, &points);
            subNode(head, name, points);

        } else if (strcmp(prompt, "del") == 0){

            scanf("%s", name);
            deleteNode(head, name);
            printf("%s deleted\n", name);

        } else if (strcmp(prompt, "search") == 0){
            scanf("%s", name);
            treenode * tmp = searchNode(head, name);
            int depth = depthNode(head, name, 0);
            if (tmp == NULL){
                printf("%s not found\n", name);
            }
            else { 
                printf("%s %d %d\n", tmp->cPtr->name, tmp->cPtr->point, depth);
            }
        } else if (strcmp(prompt, "count_smaller") == 0){
            scanf("%s", name);
            int res = countSmallerNode(head, name);
            printf("%d\n", res);
        }
    }

    //printInOrder(head);

    treenode * forest[head->size];
    int index = 0;
    changeToArray(head, forest, &index);
    sortSize(forest, head->size);
    
    for (int i = head->size - 1; i >= 0; i--){
        printf("%s %d\n", forest[i]->cPtr->name, forest[i]->cPtr->point);
    }
    
    for(int i = 0; i < head->size; i++){
        forestFire(forest[i]);
    }
    forestFire(head);

    return 0;
}
