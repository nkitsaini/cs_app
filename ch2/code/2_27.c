#include <stdio.h>

int uadd_ok(unsigned x, unsigned y)
{
	unsigned sum = x+y;
	return sum >= x;
}

int main()
{
	unsigned u32_max = 0xFFFFFFFF;
	printf("1 = %d\n", uadd_ok(1, 2));
	printf("0 = %d\n", uadd_ok(u32_max, 2));
	printf("1 = %d\n", uadd_ok(u32_max, 0));
	printf("0 = %d\n", uadd_ok(2, u32_max));
	printf("1 = %d\n", uadd_ok(0, u32_max));
	printf("1 = %d\n", uadd_ok(u32_max / 2, u32_max / 2));
	printf("0 = %d\n", uadd_ok((u32_max / 2) + 2, u32_max / 2));
	return 0;
}