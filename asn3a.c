// CS1 - ASN3 - Where to Sit (Permutations)
#include<stdio.h>
#include<string.h>

#define MAXLEN 19
#define MAXN 10

// n -> number of people
// p -> numbers of pairs who don't want to sit together
// NAME -> name of person
// pc -> int to tell if person has popcorn
// cansit -> int 2D array to store what people can sit next to each other
int n, p; 
char name[MAXN][MAXLEN + 1];
int pc[MAXN];
int cansit[MAXN][MAXN];

int makePerms(int possible[], int alreadyChecked[], int index);
int checkSeat(int possible[]);

int checkSeat(int possible[]){
    for(int i = 0; i < n; i++) // Popcorn checker
    {
        if(pc[possible[i]]) // Checks if current person has popcorn
        {
            continue;
        }
        if(i > 0 && pc[possible[i - 1]]) // All other cases in the middle's access to popcorn (On their right)
        {
            continue;
        }
        if(i < n - 1 && pc[possible[i + 1]]) // Checks last person's access to popcorn (On their left)
        {
            continue;
        }
        
        return 0;
    }

    for(int i = 0; i < n - 1; i++) // People checker
    {
        if(cansit[possible[i]][possible[i + 1]] == 0) // Checks if these two people can sit next to eachother based on index
        {
            return 0;
        }
    }
    return 1;
}

int makePerms(int possible[], int alreadyChecked[], int index)
{
    if(index == n){ // If its the last index
        return checkSeat(possible); // Check If Seating is possible
    }

    int final = 0; // Declares and sets count to 0
    for(int i = 0; i < n; i++)
    {
        if(alreadyChecked[i] == 0){ // Checks if this permutation has been checked already
            alreadyChecked[i] = 1; // Sets to 1 so it has been checked for next check
            possible[index] = i; // Sets the item at index of possible to i to store first element of permutation
            final = final + makePerms(possible, alreadyChecked, index + 1); // Resursive call
            alreadyChecked[i] = 0; // Returns back to 0 so it can be checked for another set of permutations
        }
    }
    return final; // Return final value to previous recursive or main
}

int main(){
    // take inputs for n and p
    scanf("%d %d", &n, &p);

    // take inputs for names and whether or not they have popcorn
    for(int i = 0; i < n; i++){
        scanf("%s %d", name[i], &pc[i]);
    }

    // set all possible pairs to be able to sit together
    // cansit -> 2D array to store possible pair combinations
    for (int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cansit[i][j] = 1;
        }
    }

    // take inputs for people who cant sit together
    // res -> restricted pair
    int res1, res2;
    char nope1[MAXLEN + 1], nope2[MAXLEN + 1];
    for(int i = 0; i < p; i++){
        scanf("%s %s", nope1, nope2);
        for(int j = 0; j < n; j++){
            if(strcmp(name[j], nope1) == 0){
                res1 = j;
            }
            if(strcmp(name[j], nope2) == 0){
                res2 = j;
            }
        }
        cansit[res1][res2] = 0;
        cansit[res2][res1] = 0;
    }
    

    // possible -> array to store possible perms of names
    // alreadyChecked -> array to store iterations per array that were already checked
    // final -> count of possible perms
    int possible[n];
    int alreadyChecked[n];

    int final = makePerms(possible, alreadyChecked, 0); // Sets final value equal to the final value returned after recursive makePerms functions

    printf("%d\n", final);
    

    return 0;
}
