#include <time.h>
// Testing if a multiple with running 1's is faster

int main()
{
	int a = 615;
	int b = 516;
	int k = 0;
	clock_t startClocks = clock();
	for (int i = 0; i < 100000000; i++)
	{
		int c = a * b * i;
		k += c;
	}
	clock_t endClocks = clock();
	printf("Total Clocks: %ld\n", endClocks - startClocks);

	float startTime = (float)startClocks / CLOCKS_PER_SEC;
	float endTime = (float)endClocks / CLOCKS_PER_SEC;
	printf("Total Time: %.2f\n", endTime - startTime);
}