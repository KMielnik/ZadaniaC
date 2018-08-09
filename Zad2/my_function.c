#include <stdbool.h>
/**
* Function checks if a string has the same amount of 'x' and 'o's. Function must be case insensitive.
*/
bool XO(const char *word, const int length)
{
    int Xcounter=0;
    int Ocounter=0;

    for(int i=0;i<length;i++)
    {
        if(word[i] == 'x' || word[i] == 'X')
            Xcounter++;
        if(word[i] == 'o' || word[i] == 'O')
            Ocounter++;
    }

    if(Xcounter == Ocounter)
        return true;
    else 
        return false;
}