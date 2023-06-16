#include "munit/munit.h"
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

typedef unsigned char * byte_pointer;

#define ASSERT_TRUE(x) do { \
	if (x != 1) { \
    printf("====== Assert True Failed. [%s:%d]\n", __FILE__, __LINE__); \
    printf(#x); \
    printf(" = "); \
    printf("%d\n", x); \
	} \
} while(false);

#define ASSERT_EQ_UINT(x, y) do { \
	if (x != y) { \
    printf("====== Assert UINT Eq Failed. [%s:%d]\n", __FILE__, __LINE__); \
    printf(#x); \
    printf(" = "); \
    printf("%d\n", x); \
    printf(#y); \
    printf(" = "); \
    printf("%y\n", x); \
	} \
} while(false);

#define ASSERT_FALSE(x) do { \
	if (x != 0) { \
    printf("====== Assert False Failed. [%s:%d]\n", __FILE__, __LINE__); \
    printf(#x); \
    printf(" = "); \
    printf("%d\n", x); \
	} \
} while(false);

// 2.55
void show_bytes(byte_pointer start, size_t length) {
	printf("0x");
	for(size_t i = 0; i < length; i ++) {
		printf("%.2X", start[i]);
	}
	printf("\n");
}

// 2.56
void run_show_bytes() {
	uint32_t a = 1;
	int b = -1;
	unsigned c = 1;
	show_bytes((byte_pointer)&a, sizeof(int));
	show_bytes((byte_pointer)&b, sizeof(int));
	show_bytes((byte_pointer)&c, sizeof(unsigned));
}
// 2.57
void show_short(short x) {
	show_bytes((byte_pointer)&x, sizeof(short));
}
void show_long(long x) {
	show_bytes((byte_pointer)&x, sizeof(long));
}

void show_int(int x) {
	show_bytes((byte_pointer)&x, sizeof(int));
}

void show_uint(unsigned int x) {
	show_bytes((byte_pointer)&x, sizeof(unsigned int));
}

void show_double(double x) {
	show_bytes((byte_pointer)&x, sizeof(double));
}

// 2.58
bool is_little_endian() {
	uint32_t a = 1;
	return ((byte_pointer)&a)[1] == 1;
}
// TODO: 2.59 (not sure if type int can be assumed, or how can it be done without that.)

// 2.60
unsigned replace_byte(unsigned x, int i, unsigned char b) {
	unsigned y = x;
	byte_pointer y_ptr = (byte_pointer)&y;
	int index = i;
	if (is_little_endian()) {
		index = sizeof(unsigned) - i - 1;
	}
	y_ptr[index] = b;
	return y;
}

void test_replace_byte() {
	bool failed = false;
	if (replace_byte(0x12345678, 2, 0xAB) != 0x12AB5678) {
		printf("Test 1 Failed :( (value=%u)\n", replace_byte(0x12345678, 2, 0xAB));
		failed = true;
	}

	if (replace_byte(0x12345678, 0, 0xAB) != 0x123456AB) {
		printf("Test 2 Failed :( (value=%.8X, expected=%.8X)\n", replace_byte(0x12345678, 0, 0xAB), 0x123456AB);
		failed = true;
	}
}

/* 2.61 A) Returns true if any bit of x is 1 */ 
bool is_any_bit_one(int x) {
	return x && 1;
}


void test_is_any_bit_one() {
	assert(is_any_bit_one(0) == 0);
	assert(is_any_bit_one(1) == 1);
	assert(is_any_bit_one(-1) == 1);
	assert(is_any_bit_one(-123) == 1);
}

/* 2.61 B) Returns true if any bit of x is 0 */ 
bool is_any_bit_zero(int x) {
	return ((x + 1) || 0);
}

void test_is_any_bit_zero() {
	assert(is_any_bit_zero(0) == 1);
	assert(is_any_bit_zero(1) == 1);
	assert(is_any_bit_zero(-1) == 0);
	assert(is_any_bit_zero(-123) == 1);
	assert(is_any_bit_zero(INT_MIN) == 1);
	assert(is_any_bit_zero(INT_MAX) == 1);
}

/* 2.61 B) Returns true if any bit of least significant byte in x is 1 */ 
bool is_any_lsb_one(int x) {
	return is_any_bit_one(x & 0xFF);
}

void test_is_any_lsb_one() {
	assert(is_any_lsb_one(0) == 0);
	assert(is_any_lsb_one(1) == 1);
	assert(is_any_lsb_one(-1) == 1);
	assert(is_any_lsb_one(INT_MIN) == 0);
	assert(is_any_lsb_one(INT_MAX) == 1);
}

/* 2.61 B) Returns true if any bit of most significant byte in x is 0 */ 
bool is_any_msb_zero(int x) {
	int shift_val = (sizeof(int) -1) << 3;
	int xright = x >> shift_val;

	// mask = 0xFFFFFF...FF00
	int mask = ~0 & 0x00;

	return is_any_bit_zero(xright | (~0 & 0x00));
}


void test_is_any_msb_zero() {
	assert(is_any_msb_zero(0) == 1);
	assert(is_any_msb_zero(1) == 1);
	assert(is_any_msb_zero(-1) == 0);
	assert(is_any_msb_zero(INT_MIN) == 1);
	assert(is_any_msb_zero(INT_MAX) == 1);
}

// 2.62
bool int_shifts_are_arithmetic() {
	int value = -1;

	// right shift
	value = value >> 8;

	return 1 - is_any_msb_zero(value);
}

// NOTE: not compatible across machines
void test_int_shifts_are_arithmetic() {
	assert(int_shifts_are_arithmetic() == 1);
}

// 2.63 A 
unsigned srl(unsigned x, int k) {
	// Perform arithmetic shift
	unsigned xsra = (int) x >> k;

	int bits = sizeof(unsigned) << 3;

	// TODO: remove if
	// if (k != 0) {
	unsigned untouched_bits = bits - k;

	// if k=0, there's no change
	unsigned is_changed_at_all = k && 1;

	// 1 => 1000000000000... => 011111111111111... => 1111111...11110 => 111111111111...111111
	// 0 => 0000000000000... => 000000000000000... => 0000000...00000 => 000000000000...000000
	unsigned all_mask = (((is_changed_at_all << (bits -1)) -is_changed_at_all) << 1) + is_changed_at_all;

	// 0 if is_changed_at_all=0 else no change.
	untouched_bits = untouched_bits & all_mask;

	// mask = 0000..{k}  1111.. {untouched_bits = bits -k}
	// 1 => 00000..100000000 => 00000..01111111111 => 0000..01111111111 (is_changed_at_all=1)
	// 1 => 00000..00000001 => 00000..00000000000 => 1111..1111111111 (is_changed_at_all=0)
	unsigned mask = (1 << untouched_bits) - 1 - (1-is_changed_at_all);
	return mask & xsra;
	// }
	// return x;
}

void test_srl() {
	munit_assert_uint(srl(1, 1), ==, 0);
	munit_assert_int(srl(1, 0), ==, 1);
	munit_assert_uint(srl(UINT_MAX, 3), ==, UINT_MAX>>3);
	munit_assert_uint(srl(UINT_MAX, 0), ==, UINT_MAX>>0);
}


// 2.63 B 
int sra(int x, int k) {
	// Perform logical shift
	int xsra = (unsigned) x >> k;
	int bits = sizeof(int) << 3;
	int signed_bit_pos = bits - k -1;
	int signed_bit = ((1 << signed_bit_pos) & xsra) && 1;

	// 1 => 1000000000000... => 011111111111111... => 1111111...11110 => 111111111111...111111
	// 0 => 0000000000000... => 000000000000000... => 0000000...00000 => 000000000000...000000
	unsigned all_mask_signed_bit = (((signed_bit << (bits -1)) -signed_bit) << 1) + signed_bit;


	unsigned untouched_bits = bits - k;

	// if k=0, there's no change
	unsigned is_changed_at_all = k && 1;

	// 1 => 1000000000000... => 011111111111111... => 1111111...11110 => 111111111111...111111
	// 0 => 0000000000000... => 000000000000000... => 0000000...00000 => 000000000000...000000
	unsigned all_mask = (((is_changed_at_all << (bits -1)) -1) << 1) + is_changed_at_all;

	// 0 if is_changed_at_all=0 else no change.
	untouched_bits = untouched_bits & all_mask;

	// mask = 0000..{k}  1111.. {untouched_bits = bits -k}
	// 1 => 00000..100000000 => 00000..01111111111 
	// 1 => 00000..00000001 => 00000..00000000000
	unsigned mask = (1 << untouched_bits) - 1;
	return ((~mask) & all_mask_signed_bit) | xsra;
}

void test_sra() {
	munit_assert_int(sra(1, 1), ==, 0);
	munit_assert_int(sra(1, 0), ==, 1);
	munit_assert_int(sra(INT_MAX, 3), ==, INT_MAX>>3);
	munit_assert_int(sra(INT_MAX, 0), ==, INT_MAX>>0);
}

// 2.64 (can assume 32 bit)
bool any_odd_one(unsigned x) {
	// TODO: can this be simplified?
	return 
	(x & 1) ||
	(x & 1<<2) ||  	
	(x & 1<< 4) ||  	
	(x & 1<< 6) ||  	
	(x & 1 << 8) ||  	
	(x & 1 << 10) ||  	
	(x & 1 << 12) ||  	
	(x & 1 << 14) ||  	
	(x & 1 << 16) ||  	
	(x & 1 << 18) ||  	
	(x & 1 << 20) ||  	
	(x & 1 << 22) ||  	
	(x & 1 << 24) ||  	
	(x & 1 << 26) ||  	
	(x & 1 << 28) ||  	
	(x & 1 << 30);  	
}

void test_any_odd_one() {
	munit_assert_int(any_odd_one(1), ==, 1);
	munit_assert_int(any_odd_one(0), ==, 0);
	munit_assert_int(any_odd_one(UINT_MAX), ==, 1);
}

// 2.65 (4star)
int odd_ones(unsigned x) {
	//
}

void test_odd_ones() {
	munit_assert_int(odd_ones(1), ==, 1);
	munit_assert_int(odd_ones(0), ==, 0);
	munit_assert_int(odd_ones(UINT_MAX), ==, 0);
	munit_assert_int(odd_ones(UINT_MAX-1), ==, 1);
}

void run_tests() {
	test_replace_byte();
	printf("1 Tests passed!\n");

	test_is_any_bit_one();
	printf("2 Tests passed!\n");

	test_is_any_bit_zero();
	printf("3 Tests passed!\n");

	test_is_any_lsb_one();
	printf("4 Tests passed!\n");

	test_is_any_msb_zero();
	printf("5 Tests passed!\n");

	test_int_shifts_are_arithmetic();
	printf("6 Tests passed!\n");

	test_srl();
	printf("7 Tests passed!\n");

	test_sra();
	printf("8 Tests passed!\n");

	test_any_odd_one();
	printf("9 Tests passed!\n");

	test_odd_ones();
	printf("10 Tests passed!\n");
}

int get_one() {
	return 1;
}
int main() {
	// printf("%d\n", -1 && -2);
	// printf("%d\n", -1 && -2);
	// int x = 1;
	// ASSERT_TRUE(x);
	// ASSERT_TRUE(0);
	// int y = 0;
	// ASSERT_TRUE(y);
	// assert( 1== 1);
	// munit_assert_int( x, ==, y);
	// ASSERT_EQ(x, 1);
	// ASSERT_EQ(get_one(), 0);
	// ASSERT_EQ(get_one(), 0);
	// ASSERT_EQ(1, 1);

	run_tests();
}

