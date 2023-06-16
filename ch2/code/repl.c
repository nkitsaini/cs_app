#include <stdbool.h>
#include <stdio.h>

bool is_big_endian()
{
	int i = 1;
	return ((unsigned char *)&i)[0] == 1;
}

// void show_bytes(unsigned char *start, int length)
// {
// 	printf("0x");

// 	int start = 0;
// 	int end = length;
// 	for (int i = length - 1; i >= 0; i--)
// 	{
// 		printf(" %.2X ", start[i]);
// 	}
// 	printf("\n");
// }

#define SHOW_BYTES(value, type) show_bytes((unsigned char *)&value, sizeof(type))

int main()
{
	int i = 0;
	while (true) {
		if (i%1000 == 0) {
			printf("Checking: %d\n", i);
		}
		float x = (float)i;
		if ((int)x != i) {
			printf("Found: %d\n", i);
			break;
		}
		i += 1;
	}
}
