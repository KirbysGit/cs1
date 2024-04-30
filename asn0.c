// CS1 - ASN0 - Sign of Trouble.
// Implementing Strings.

#include <stdio.h> // Declares libraries
#include <string.h>
#define SIZE 100001 // Declares value for SIZE

int main()
{
    char str1[SIZE]; // Declares string to hold 1st input
    char str2[SIZE]; // Declares string to hold 2nd input
    int freqarr[26] = {0}; // Declares array to hold frequency values
    int i = 0; // Declares increment value to 0
    int index; // Declares variable to hold index value for frequency array
    
    printf("Enter the original sign message: "); // Prompts user for the 1st string
    fgets(str1, sizeof(str1), stdin); // Sets input to str1 from stdin

    printf("Enter the new sign message: "); // Prompts user for the 2nd string
    fgets(str2, sizeof(str2), stdin);  // Sets input to str2 from stdin

    int o = strlen(str1); // Sets int o equal to length of str1
    int p = strlen(str2); // Sets int p equal to length of str2

    str1[o-1] = '\0'; // Sets newline character to null character
    str2[p-1] = '\0'; // Sets newline character to null character

    // Loop that increments freq. array per letter in 1st string

    while(str1[i] != '\0') // Verifies char at index is not null character
    {
        if (str1[i] != ' ') // Removes empty spaces from being checked when iterating through
        {
            index = str1[i] - 'A'; // Subtracts ASCII value of 'A' from ASCII value of the letter in "str1[i]"
            freqarr[index]++; // Increments up array index at specfic index value resulting from eqn directly above
        }
        i++; // Increments up increment value to move to next char of string
    }

    int newchar = 0; // Declares new value to hold the number of new letters needed
    i = 0; // Resets increment value to 0


    // Loop to both decrement the freq. array per character that is in both strings
    // as well as increment newchar value per new letter not holding a value in
    // frequency array

    while(str2[i] != '\0') // Verifies char at index is not null character
    {
        if (str2[i] != ' ') // Removes empty spaces from beign checked when iterating through array
        {

            index = str2[i] - 'A'; // Subtracts ASCII value of 'A' from ASCII value of the letter in "str2[i]"
            if (freqarr[index] > 0) // IF value of freq. array at index is greater than 0 then it will decrement it
            {
                freqarr[index]--;
            }
            else // If value of freq. array at index is less than or equal to zero then it will increment newchar value
            {
                newchar++;
            }
        }
        i++; // Increment up i
    }
    printf("Number of New Characters: %d\n", newchar); // Display number of new characters to user

    return 0; // End program
}
