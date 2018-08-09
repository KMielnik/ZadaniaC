#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "my_function.c"

/** 
* Function returns true if word_1 and word_2 are anagrams to each other. Otherwise false.
* Case sensitivity doesn't matter.
*/

void test_cases()
{
	bool answer = check_if_anagram("LordVader", strlen("LordVader"), "VaderLord", strlen("VaderLord"));
	assert(answer == true);

	answer = check_if_anagram("silent", strlen("silent"), "listen", strlen("listen"));
	assert(answer == true);

	answer = check_if_anagram("funeral", strlen("funeral"), "real fun", strlen("real fun"));
	assert(answer == true);

	answer = check_if_anagram("Elon Musk", strlen("Elon Musk"), "Muskmelon", strlen("Muskmelon"));
	assert(answer == true);

	answer = check_if_anagram("Tieto", strlen("Tieto"), "Tietonator", strlen("Tietonator"));
	assert(answer == false);

	answer = check_if_anagram("Football", strlen("Football"), "Basketball", strlen("Basketball"));
	assert(answer == false);

	answer = check_if_anagram("F", strlen("F"), "", strlen(""));
	assert(answer == false);
}

int main()
{
	test_cases();
}

