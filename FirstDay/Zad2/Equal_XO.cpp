#include <string.h>
#include <assert.h>
#include "my_function.c"

void test_cases()
{
	bool answer = XO("ooxx", strlen("ooxx"));
	assert(answer == true);

	answer = XO("xooxx", strlen("xooxx"));
	assert(answer == false);

	answer = XO("ooxXm", strlen("ooxXm"));
	assert(answer == true);

	answer = XO("zzoo", strlen("zzoo"));
	assert(answer == false);
}

int main(int argc, char *argv[])
{
	test_cases();
	return 0;
}