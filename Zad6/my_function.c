#include <stdio.h>
/**
* Function takes a 'start' and 'end' number of a range, and should return the count of all numbers except numbers with 5 in it.
* For example: start = 4, end = 8. It should return 4, because: 4, 6, 7, 8
*/
int numbers_without_five(int start, int end)
{
    int count = 0;
    for(int i=start;i<=end;i++)
    {
        int actual_number=i;
        bool has_five = false;

        while(actual_number!=0)
        {
            if(actual_number % 10 == 5)
            {
                has_five = true;
                break;
            }
            actual_number/=10;
        }
        if(!has_five)
        count++;
    }
	return count;
}
