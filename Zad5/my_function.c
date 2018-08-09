#include <stdio.h>
#include <stdlib.h>
/**
* Function duplicates letters based on their index. Each index means how many times the letter needs to be duplicated.
* See test cases for examples.
*/

char* accumulate(const char *word, const int length)
{
    char *result = (char*) calloc(300,sizeof(char));
    int position = 0;

    for(int i=0;i<length;i++)
    {
        int letter = (word[i] > 95) ? word[i] - 32 : word[i];   //uppercase letter
        
        result[position++] = (char) letter;
        for(int j=0;j<i;j++)
        {
            result[position++] = (char) (letter+32);
        }
        result[position++] = '-';
    }
    result[--position] = '\0';
    return result;
}