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

unsigned replace_byte_sol(unsigned x, int i, unsigned char b) {
	unsigned mask = (((unsigned) 0xff) << (i << 3));
	unsigned pos_byte = (((unsigned) b) << (i << 3));

	return (x & ~mask) | pos_byte;
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

/* 2.61 C) Returns true if any bit of least significant byte in x is 1 */ 
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

/* 2.61 D) Returns true if any bit of most significant byte in x is 0 */ 
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

/*
* HELPER 
* 1 => 111111111111111111
* 0 => 000000000000000000
*/
unsigned full_mask_of_size(bool all_one, size_t size) {
	// 1 => 1000000000000... => 011111111111111... => 1111111...11110 => 111111111111...111111
	// 0 => 0000000000000... => 000000000000000... => 0000000...00000 => 000000000000...000000
	unsigned all_mask = (((all_one << (size -1)) - all_one) << 1) + all_one;
	return all_mask;
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

	unsigned all_mask = full_mask_of_size(is_changed_at_all, bits);

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

	unsigned all_mask_signed_bit = full_mask_of_size(signed_bit, bits);


	unsigned untouched_bits = bits - k;

	// if k=0, there's no change
	unsigned is_changed_at_all = k && 1;

	unsigned all_mask = full_mask_of_size(is_changed_at_all, bits);

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
	(x & 1<<1) ||
	(x & 1<<3) ||  	
	(x & 1<< 5) ||  	
	(x & 1<< 7) ||  	
	(x & 1 << 9) ||  	
	(x & 1 << 11) ||  	
	(x & 1 << 13) ||  	
	(x & 1 << 15) ||  	
	(x & 1 << 17) ||  	
	(x & 1 << 19) ||  	
	(x & 1 << 21) ||  	
	(x & 1 << 23) ||  	
	(x & 1 << 25) ||  	
	(x & 1 << 27) ||  	
	(x & 1 << 29) ||  	
	(x & 1 << 31);  	
}

bool any_odd_one_sol(unsigned x) {
	return !!(0xAAAAAAAA & x);
}

void test_any_odd_one() {
	munit_assert_int(any_odd_one(1), ==, 0);
	munit_assert_int(any_odd_one(0), ==, 0);
	munit_assert_int(any_odd_one(UINT_MAX), ==, 1);

	munit_assert_int(any_odd_one_sol(1), ==, 0);
	munit_assert_int(any_odd_one_sol(0), ==, 0);
	munit_assert_int(any_odd_one_sol(UINT_MAX), ==, 1);

}

// 2.65 (4star)
int odd_ones(unsigned x) {
	unsigned curr = x;

	// initial 16
	unsigned half1 = curr >> 16;
	curr = half1^curr;

	unsigned half2 = curr >> 8;
	curr = half2^curr;

	unsigned half3 = curr >> 4;
	curr = half3^curr;
	
	unsigned half4 = curr >> 2;
	curr = half4^curr;

	unsigned half5 = curr >> 1;
	curr = half5^curr;

	return curr & 1;
}

int odd_ones_alternate(unsigned x) {
	int sum = 0;
	for (int i = 0; i < 32; i++) {
		sum += ((1 << i) & x) && 1;
	}
	return (sum%2)==1;
}

void test_odd_ones() {
	munit_assert_int(odd_ones(1), ==, 1);
	munit_assert_int(odd_ones(0), ==, 0);
	munit_assert_int(odd_ones(UINT_MAX), ==, 0);
	munit_assert_int(odd_ones(UINT_MAX-1), ==, 1);

	// full-search (takes somewhere in order of 10 minutes)
	unsigned end = 0xFFFFFFFF;

	// quick-search
	end = 0xFFFFF;

	for(unsigned i = 0; i < end; i ++) {
		if (end == 0xFFFFFFFF && i%0xFFFFFF == 0) {
			printf("%u\n", i);
		}
		munit_assert_int(odd_ones(i), ==, odd_ones_alternate(i));
	}
}


/* 2.66 
* Generate mask indicating leftmost 1 in x. Assume w=32.
* For example, OxFFOO —>0x8000, and 0x6600 ——>0x4000.
* If x = 0, then return 0.
*/
int leftmost_one(unsigned x) {
	unsigned org_x = x;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return (x >> 1) + (!!org_x);
}

void test_leftmost_one() {
	munit_assert_uint(leftmost_one(0xff00), ==, 0x8000);
	munit_assert_uint(leftmost_one(0x6600), ==, 0x4000);
}


/*
* 2.67
* A. The code shifts by more then (0, w-1). C does not document this behaviour.
* B)
*/
int int_size_is_32_b() {

	// Set most significant bit (msb) of 32-bit machines
	int set_msb = 1 << 31;

	// Add itself twice and 
	int beyond_msb = (set_msb + set_msb);

	return set_msb && !beyond_msb;

}

/*
* 2.67 C)
*/
int int_size_is_32_c() {
	int set_31_msb = ((1 << 15) << 15) << 1;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshift-overflow"
	int beyond_msb = ((1 << 15) << 15) << 2;
#pragma GCC diagnostic pop
	return set_31_msb && !beyond_msb;
}

/*
* 2.68)
* Mask with least signficant n bits set to 1
* Examples: n = 6 ——>Ox3F, n = 17 ——>Ox1FFFF
* Assume 1 <= n <= w
*/
int lower_one_mask(int n) {
	return full_mask_of_size(1, n);
}

void test_lower_one_mask(){
	munit_assert_int(lower_one_mask(6), ==, 0x3f);
	munit_assert_int(lower_one_mask(17), ==, 0x1ffff);
}



void run_tests() {
	int test_count = 0;
	test_replace_byte();
	test_count += 1;
	printf("%2d Tests passed!\n", test_count);

	test_is_any_bit_one();
	test_count += 1;
	printf("%2d Tests passed!\n", test_count);

	test_is_any_bit_zero();
	test_count += 1;
	printf("%2d Tests passed!\n", test_count);

	test_is_any_lsb_one();
	test_count += 1;
	printf("%2d Tests passed!\n", test_count);

	test_is_any_msb_zero();
	test_count += 1;
	printf("%2d Tests passed!\n", test_count);

	test_int_shifts_are_arithmetic();
	test_count += 1;
	printf("%2d Tests passed!\n", test_count);

	test_srl();
	test_count += 1;
	printf("%2d Tests passed!\n", test_count);

	test_sra();
	test_count += 1;
	printf("%2d Tests passed!\n", test_count);

	test_any_odd_one();
	test_count += 1;
	printf("%2d Tests passed!\n", test_count);

	test_odd_ones();
	test_count += 1;
	printf("%2d Tests passed!\n", test_count);

	test_leftmost_one();
	test_count += 1;
	printf("%2d Tests passed!\n", test_count);
	
	test_lower_one_mask();
	test_count += 1;
	printf("%2d Tests passed!\n", test_count);
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

