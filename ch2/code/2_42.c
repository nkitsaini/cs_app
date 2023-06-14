#include <stdio.h>

int div16(int x)
{
	int div_neg = (x + (1 << 4) - 1) >> 4;
	int div_pos = x >> 4;
	int is_neg = ((x >> 31) & 1);
	int is_neg_scaled = ((1 - is_neg) << 4) + is_neg;
	// printf("is_neg: %d, is_neg_scaled: %d\n", is_neg, is_neg_scaled);
	return (x + (1 << 4) - is_neg_scaled) >> 4;

	// return x/16
}
int div16_good(int x)
{
	int bias = (x >> 31) & 15; // 0 if x>=0 else 15
	return (x + bias) >> 4;
}

int main()
{
	printf("17/16  = %d\n", div16_good(17));
	printf("16/16  = %d\n", div16_good(16));
	printf("5/16   = %d\n", div16_good(5));
	printf("-5/16  = %d\n", div16_good(-5));
	printf("-16/16 = %d\n", div16_good(-16));
	printf("-17/16 = %d\n", div16_good(-17));
	return 0;
}