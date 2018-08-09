#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
/** 
* Function returns an array of 5 elements.
* Those 5 elements are created randomly in the range 1 - 49.
* Numbers can't repeat.
*/
int* Lotto_drawing()
{
    srand((unsigned) time(NULL));

    int *numbers = (int*) calloc(5,sizeof(int));

    bool already_choosen[50];
    for(int i=0;i<49;i++)
        already_choosen[i] = false;
    for(int i=0;i<5;i++)
    {
        int random_number = rand() % 49 + 1;
        while(already_choosen[random_number])
        {
            random_number = rand() % 49 + 1;
        }
        already_choosen[random_number] = true;
        
        numbers[i] = random_number;
    }
	return numbers;
}

/* Please create test cases for this program. test_cases() function can return void, bool or int. */
void test_cases()
{
    //check if all numbers are in range
    int *numbers = Lotto_drawing();
    bool any_not_in_range = false;
    for(int i=0;i<5;i++)
        if(numbers[i] < 1 || numbers[i] > 49)
            any_not_in_range = true;
    assert(any_not_in_range == false);

    //check if numbers do not repeat themselves
    numbers = Lotto_drawing();
    bool any_repeated = false;
    bool range[50];
    for(int i=0;i<49;i++)
        range[i] = false;
    for(int i=0;i<5;i++)
    {
        int actual_number = numbers[i];
        if(range[actual_number] == true)
        {
            any_repeated = true;
            break;
        }
        range[actual_number] = true;
    }
    assert(any_repeated == false);
}