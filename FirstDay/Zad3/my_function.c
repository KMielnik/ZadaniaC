#include <stdlib.h>
#include <stdio.h>

int compareDescending (const void *a, const void *b);
int pow10 (int power);

/**
* Given a number, return the maximum number that could be formed with digits of the number given.
* For example: number = 7389, return 9873
*/
int form_the_largest_number(int number)
{
    int digits[100];
    int how_many_digits = 0;

    for(int i=0;i<100;i++)
        digits[i]=-1;

    while(number != 0)
    {
        digits[how_many_digits] = number % 10;
        number/=10;
        how_many_digits++;
    }

    qsort(digits,100,sizeof(int),compareDescending);

    int largest_number=0;

    for(int i=0;i<how_many_digits;i++)
        largest_number+=(digits[how_many_digits-i-1] * pow10(i+1));

	return largest_number;
}

int compareDescending (const void *a, const void *b) 
{
    int ia = *(int*) a;
    int ib = *(int*) b;
    return ib-ia;
}

int pow10 (int power)
{
    int number = 1;
    for(int i=1;i<power;i++)
        number*=10;
    return number;
}