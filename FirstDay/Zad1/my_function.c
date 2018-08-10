#include <stdio.h>
#include <stdbool.h>

bool check_if_anagram(const char* word_1, int length_1, const char* word_2, int length_2)
{
	bool word_1_letters[128];
	bool word_1_is_letter_unused[128];

	for(int i=0;i<128;i++)
	{
		word_1_letters[i]=false;
		word_1_is_letter_unused[i] = false;
	}

	for(int i = 0;i<length_1; i++)
    {
		if(word_1[i] == ' ')
			continue;
        int actual_letter_ASCII = (int)word_1[i];
        word_1_letters[actual_letter_ASCII] = true;
		word_1_is_letter_unused[actual_letter_ASCII] = true;

    }

    for(int i = 0;i<length_2; i++)
    {
        int actual_letter_ASCII = (int)word_2[i];

		
        if(word_1_letters[actual_letter_ASCII])
            word_1_is_letter_unused[actual_letter_ASCII] = false;
        else if(word_1_letters[actual_letter_ASCII + 32])
            word_1_is_letter_unused[actual_letter_ASCII+32] = false;
        else if(word_1_letters[actual_letter_ASCII - 32])
            word_1_is_letter_unused[actual_letter_ASCII-32] = false;
        else if(actual_letter_ASCII == (int)' ')
            word_1_is_letter_unused[actual_letter_ASCII] = false;
        else
            return false;
    }

	for(int i=0;i<128;i++)
		if(word_1_is_letter_unused[i])
			return false;

	return true;
}