// CS1 - ASN4 - Projector Problems (Sorting)
// Implementing Sorting Methods to Obtain Best Angle of No Values.

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define DEBUG 1 //debugging purposes
#define EMPTY 0
#define INT_MAX 10000000 // for extreme cases

typedef struct group group;

// provided struct
struct group {
    double angle;
    int size;
};

// merge sort func from mr.meade but converted to struct
group * sortByAngle(group * myGroup, int n){
    // Base Case
    if (n <= 1) return NULL;

    // Computer the number of half the array
    int n2 = n / 2;

    sortByAngle(myGroup, n2);
    sortByAngle(myGroup + n2, n - n2);

    group * tmp = (group *) malloc (sizeof(group) * n);
    int fptr = 0;
    int bptr = n2;

    for (int i = 0; i < n; i++){
        if (fptr == n2){
            // front is empty
            tmp[i].angle = myGroup[bptr].angle;
            tmp[i].size = myGroup[bptr].size;
            bptr++;
        } else if (bptr == n){
            // back is empty
            tmp[i].angle = myGroup[fptr].angle;
            tmp[i].size = myGroup[fptr].size;
            fptr++;
        } else if (myGroup[fptr].angle < myGroup[bptr].angle){
            // front was smaller than the back
            tmp[i].angle = myGroup[fptr].angle;
            tmp[i].size = myGroup[fptr].size;
            fptr++;
        } else {
            // back was good enough
            tmp[i].angle = myGroup[bptr].angle;
            tmp[i].size = myGroup[bptr].size;
            bptr++;
        }
    } 

    // Move the temp values into the original array
    for (int i = 0; i < n; i++){
        myGroup[i].angle = tmp[i].angle;
        myGroup[i].size = tmp[i].size;
    }

    // Remove the temp array
    free(tmp);
}


int main() {
    int groupNum; //declarations
    int angleCheck;

    scanf("%d %d", &groupNum, &angleCheck); // accept input of number of ppl and angle to be checking with

    group * myGroup = (group *) calloc(2 * groupNum, sizeof(group)); // allocate memory

    int x, y, s = 0; // int declarations
    for(int i = 0; i < groupNum; i++){ // takes input on x, y, s values
        scanf("%d %d %d", &x, &y, &s);
        double tmp = atan2(y, x) * 180 / M_PI;
        myGroup[i].angle = tmp; // converts to angle from x y value
        myGroup[i].size = s;
        myGroup[i + groupNum].angle = tmp + 360; // saves angle and size to doubled indexes to consider wrap around
        myGroup[i + groupNum].size = s;
    }


    sortByAngle(myGroup, groupNum * 2); // calls sortByAngle with myGroup struct

    int a, b = 0; // declarations
    int cursize = 0;
    int bestsize = INT_MAX;
    double diff, bestAngle = 0;

    while (1){
        if(b >= groupNum * 2){ // Base Case
            break;

        }

        diff = fabs(myGroup[b].angle - myGroup[a].angle); // take absolutes value of difference to work around the resulting negatives values


        if(diff <= angleCheck){ // if difference < user inputted angle
            cursize += myGroup[b].size; // size increments up based on size of group at that index
            b++; // increment up second pointer
        }else{ // if difference > user inputted angle
            cursize -= myGroup[a].size; // decrement the size based on group at index of first pointer
            a++; // increment first pointer
            if(cursize < bestsize){ // if current size < best size then sets best size to current size
                bestsize = cursize;
            }
        }
    } 


    // works based off the idea that it is not technically inclusive on the sizes, so the differences represent the empty space in between pointers
    for (int i = 1; i < groupNum * 2; i++){ // for loop, while i < total number of items in array
        diff = myGroup[i].angle - myGroup[i - 1].angle; // take diff of second index angle and first index angle
        if (diff > bestAngle){ // if difference > best Angle 
            bestAngle = diff; // best Angle now equals that difference
        }
    }

    printf("%d\n", bestsize);
    printf("%.4f\n", bestAngle);

    free(myGroup);

    return 0;


}
