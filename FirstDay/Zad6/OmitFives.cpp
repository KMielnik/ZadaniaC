#include <assert.h>
#include "my_function.c"

void test_cases()
{
	int answer = numbers_without_five(4, 8);
	assert(answer == 4);

	answer = numbers_without_five(1, 9);
	assert(answer == 8);

	answer = numbers_without_five(4, 17);
	assert(answer == 12);

	answer = numbers_without_five(48,53);
	assert(answer == 2);
}

int main(int argc, char *argv[])
{
	test_cases();
	return 0;
}