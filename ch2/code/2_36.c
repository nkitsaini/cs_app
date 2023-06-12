#include <stdint.h>

int tmult_ok(int a, int b)
{

	int64_t a_lg = (int64_t)a;
	int64_t b_lg = (int64_t)b;
	int64_t prod = a_lg * b_lg;

	int min = 0x80000000;
	int max = 0x7fffffff;

	int64_t int_min = (int64_t)min;
	int64_t int_max = (int64_t)max;

	return prod >= int_min && prod <= int_max;
}

int tmult_ok_good(int a, int b)
{

	int64_t a_lg = (int64_t)a;
	int64_t b_lg = (int64_t)b;
	int64_t prod = a_lg * b_lg;
	return prod == (int)prod;
}

void show_bytes(char *start, int length)
{
	printf("0x");
	for (int i = 0; i < length; i++)
	{
		printf("%.2X", start[i]);
	}
	printf("\n");
}

int main()
{
	int int_min = 0x80000000;
	int int_max = 0x7fffffff;
	int cases[][3] = {
		{1, 2, 1},
		{-1, -2, 1},
		{int_min, int_max, 0},
		{int_min, int_min, 0},
		{int_max, int_max, 0},
		{int_min, 0, 1},
		{int_max, 0, 1},
		{int_max, 2, 0},
		{int_max / 2, 2, 1},
		{int_min / 2, 2, 1},
		{int_min, 2, 0},
	};
	for (int i = 0; i < 11; i++)
	{
		int a = cases[i][0];
		int b = cases[i][1];
		int result = cases[i][2];
		printf("Expected %d = Got %d\n", result, tmult_ok(a, b));
	}
	// tmult_ok(-3, 2);
	return 0;
}
