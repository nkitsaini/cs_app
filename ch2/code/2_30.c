int tadd_ok(int x, int y)
{
	int z = x + y;
	// printf(">> %d + %d = %d\n", x, y, z);
	return !((x < 0 && y < 0 && z > 0) ||
			 (x > 0 && y > 0 && z < 0));
}

int main()
{
	int u32_min = 0x80000000;
	int u32_max = 0x7FFFFFFF;
	printf("1 = %d\n", tadd_ok(1, 2));
	printf("0 = %d\n", tadd_ok(u32_max, 2));
	printf("1 = %d\n", tadd_ok(u32_max, 0));
	printf("0 = %d\n", tadd_ok(2, u32_max));
	printf("1 = %d\n", tadd_ok(0, u32_max));
	printf("1 = %d\n", tadd_ok(u32_max / 2, u32_max / 2));
	printf("0 = %d\n", tadd_ok((u32_max / 2) + 2, u32_max / 2));

	printf("1 = %d\n", tadd_ok(-1, -2));
	printf("0 = %d\n", tadd_ok(u32_min, -2));
	printf("1 = %d\n", tadd_ok(u32_min, 0));
	printf("0 = %d\n", tadd_ok(-2, u32_min));
	printf("1 = %d\n", tadd_ok(0, u32_min));
	printf("1 = %d\n", tadd_ok(u32_min / 2, u32_min / 2));
	printf("0 = %d\n", tadd_ok((u32_min / 2) - 2, u32_min / 2));
	return 0;
}