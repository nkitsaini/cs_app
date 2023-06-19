#include "munit/munit.h"
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char *byte_pointer;

/* Converts val into type without changing any bits. Useful for float
 * conversions. */
#define INTO(type, val) *(type *)&val;

#define ASSERT_TRUE(x)                                                         \
  do {                                                                         \
    if (x != 1) {                                                              \
      printf("====== Assert True Failed. [%s:%d]\n", __FILE__, __LINE__);      \
      printf(#x);                                                              \
      printf(" = ");                                                           \
      printf("%d\n", x);                                                       \
    }                                                                          \
  } while (false);

#define ASSERT_EQ_UINT(x, y)                                                   \
  do {                                                                         \
    if (x != y) {                                                              \
      printf("====== Assert UINT Eq Failed. [%s:%d]\n", __FILE__, __LINE__);   \
      printf(#x);                                                              \
      printf(" = ");                                                           \
      printf("%d\n", x);                                                       \
      printf(#y);                                                              \
      printf(" = ");                                                           \
      printf("%y\n", x);                                                       \
    }                                                                          \
  } while (false);

#define ASSERT_FALSE(x)                                                        \
  do {                                                                         \
    if (x != 0) {                                                              \
      printf("====== Assert False Failed. [%s:%d]\n", __FILE__, __LINE__);     \
      printf(#x);                                                              \
      printf(" = ");                                                           \
      printf("%d\n", x);                                                       \
    }                                                                          \
  } while (false);

// 2.55
void show_bytes(byte_pointer start, size_t length) {
  printf("0x");
  for (size_t i = 0; i < length; i++) {
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
void show_short(short x) { show_bytes((byte_pointer)&x, sizeof(short)); }
void show_long(long x) { show_bytes((byte_pointer)&x, sizeof(long)); }

void show_int(int x) { show_bytes((byte_pointer)&x, sizeof(int)); }

void show_float(float x) { show_bytes((byte_pointer)&x, sizeof(float)); }

void show_uint(unsigned int x) {
  show_bytes((byte_pointer)&x, sizeof(unsigned int));
}

void show_double(double x) { show_bytes((byte_pointer)&x, sizeof(double)); }

// 2.58
bool is_little_endian() {
  uint32_t a = 1;
  return ((byte_pointer)&a)[1] == 1;
}
// TODO: 2.59 (not sure if type int can be assumed, or how can it be done
// without that.)

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
  unsigned mask = (((unsigned)0xff) << (i << 3));
  unsigned pos_byte = (((unsigned)b) << (i << 3));

  return (x & ~mask) | pos_byte;
}

void test_replace_byte() {
  bool failed = false;
  if (replace_byte(0x12345678, 2, 0xAB) != 0x12AB5678) {
    printf("Test 1 Failed :( (value=%u)\n", replace_byte(0x12345678, 2, 0xAB));
    failed = true;
  }

  if (replace_byte(0x12345678, 0, 0xAB) != 0x123456AB) {
    printf("Test 2 Failed :( (value=%.8X, expected=%.8X)\n",
           replace_byte(0x12345678, 0, 0xAB), 0x123456AB);
    failed = true;
  }
}

/* 2.61 A) Returns true if any bit of x is 1 */
bool is_any_bit_one(int x) { return x && 1; }

void test_is_any_bit_one() {
  assert(is_any_bit_one(0) == 0);
  assert(is_any_bit_one(1) == 1);
  assert(is_any_bit_one(-1) == 1);
  assert(is_any_bit_one(-123) == 1);
}

/* 2.61 B) Returns true if any bit of x is 0 */
bool is_any_bit_zero(int x) { return ((x + 1) || 0); }

void test_is_any_bit_zero() {
  assert(is_any_bit_zero(0) == 1);
  assert(is_any_bit_zero(1) == 1);
  assert(is_any_bit_zero(-1) == 0);
  assert(is_any_bit_zero(-123) == 1);
  assert(is_any_bit_zero(INT_MIN) == 1);
  assert(is_any_bit_zero(INT_MAX) == 1);
}

/* 2.61 C) Returns true if any bit of least significant byte in x is 1 */
bool is_any_lsb_one(int x) { return is_any_bit_one(x & 0xFF); }

void test_is_any_lsb_one() {
  assert(is_any_lsb_one(0) == 0);
  assert(is_any_lsb_one(1) == 1);
  assert(is_any_lsb_one(-1) == 1);
  assert(is_any_lsb_one(INT_MIN) == 0);
  assert(is_any_lsb_one(INT_MAX) == 1);
}

/* 2.61 D) Returns true if any bit of most significant byte in x is 0 */
bool is_any_msb_zero(int x) {
  int shift_val = (sizeof(int) - 1) << 3;
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
  // 1 => 1000000000000... => 011111111111111... => 1111111...11110 =>
  // 111111111111...111111 0 => 0000000000000... => 000000000000000... =>
  // 0000000...00000 => 000000000000...000000
  unsigned combined = all_one && size;
  unsigned all_mask = (((combined << (size - 1)) - combined) << 1) + combined;
  return all_mask;
}

void test_full_mask_of_size() {
  munit_assert_uint(full_mask_of_size(1, 1), ==, 0x1);
  munit_assert_uint(full_mask_of_size(1, 2), ==, 0x3);
  munit_assert_uint(full_mask_of_size(1, 3), ==, 0x7);
  munit_assert_uint(full_mask_of_size(1, 32), ==, UINT_MAX);
  munit_assert_uint(full_mask_of_size(1, 0), ==, 0);
  munit_assert_uint(full_mask_of_size(1, 0), ==, 0);
  munit_assert_uint(full_mask_of_size(1, 0), ==, 0);
  munit_assert_uint(full_mask_of_size(1, 0), ==, 0);
  munit_assert_uint(full_mask_of_size(1, 0), ==, 0);
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
  unsigned xsra = (int)x >> k;

  int bits = sizeof(unsigned) << 3;

  // if (k != 0) {
  unsigned untouched_bits = bits - k;

  // if k=0, there's no change
  unsigned is_changed_at_all = k && 1;

  unsigned all_mask = full_mask_of_size(is_changed_at_all, bits);

  // 0 if is_changed_at_all=0 else no change.
  untouched_bits = untouched_bits & all_mask;

  // mask = 0000..{k}  1111.. {untouched_bits = bits -k}
  // 1 => 00000..100000000 => 00000..01111111111 => 0000..01111111111
  // (is_changed_at_all=1) 1 => 00000..00000001 => 00000..00000000000 =>
  // 1111..1111111111 (is_changed_at_all=0)
  unsigned mask = (1 << untouched_bits) - 1 - (1 - is_changed_at_all);
  return mask & xsra;
}

void test_srl() {
  munit_assert_uint(srl(1, 1), ==, 0);
  munit_assert_int(srl(1, 0), ==, 1);
  munit_assert_uint(srl(UINT_MAX, 3), ==, UINT_MAX >> 3);
  munit_assert_uint(srl(UINT_MAX, 0), ==, UINT_MAX >> 0);
}

// 2.63 B
int sra(int x, int k) {
  // Perform logical shift
  int xsra = (unsigned)x >> k;
  int bits = sizeof(int) << 3;
  int signed_bit_pos = bits - k - 1;
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
  munit_assert_int(sra(INT_MAX, 3), ==, INT_MAX >> 3);
  munit_assert_int(sra(INT_MAX, 0), ==, INT_MAX >> 0);
}

// 2.64 (can assume 32 bit)
bool any_odd_one(unsigned x) {
  // Question (former todo): can this be simplified?
  // Ofcourse, see actual solution below this function
  return (x & 1 << 1) || (x & 1 << 3) || (x & 1 << 5) || (x & 1 << 7) ||
         (x & 1 << 9) || (x & 1 << 11) || (x & 1 << 13) || (x & 1 << 15) ||
         (x & 1 << 17) || (x & 1 << 19) || (x & 1 << 21) || (x & 1 << 23) ||
         (x & 1 << 25) || (x & 1 << 27) || (x & 1 << 29) || (x & 1 << 31);
}

bool any_odd_one_sol(unsigned x) { return !!(0xAAAAAAAA & x); }

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
  curr = half1 ^ curr;

  unsigned half2 = curr >> 8;
  curr = half2 ^ curr;

  unsigned half3 = curr >> 4;
  curr = half3 ^ curr;

  unsigned half4 = curr >> 2;
  curr = half4 ^ curr;

  unsigned half5 = curr >> 1;
  curr = half5 ^ curr;

  return curr & 1;
}

int odd_ones_alternate(unsigned x) {
  int sum = 0;
  for (int i = 0; i < 32; i++) {
    sum += ((1 << i) & x) && 1;
  }
  return (sum % 2) == 1;
}

void test_odd_ones() {
  munit_assert_int(odd_ones(1), ==, 1);
  munit_assert_int(odd_ones(0), ==, 0);
  munit_assert_int(odd_ones(UINT_MAX), ==, 0);
  munit_assert_int(odd_ones(UINT_MAX - 1), ==, 1);

  // full-search (takes somewhere in order of 10 minutes)
  unsigned end = 0xFFFFFFFF;

  // quick-search
  end = 0xFFFFF;

  for (unsigned i = 0; i < end; i++) {
    if (end == 0xFFFFFFFF && i % 0xFFFFFF == 0) {
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
int lower_one_mask(int n) { return full_mask_of_size(1, n); }

void test_lower_one_mask() {
  munit_assert_int(lower_one_mask(6), ==, 0x3f);
  munit_assert_int(lower_one_mask(17), ==, 0x1ffff);
}

/*
 * 2.69
 * Do rotating left shift. Assume 0 <= n < w
 * Examples when x = 0x12345678 and w = 32:
 * n4 —>0x23456781, n20 —>0x67812345
 */
unsigned rotate_left(unsigned x, int n) {
  unsigned left_shift = x << n;
  size_t w = sizeof(unsigned) << 3;
  size_t l_shift_count = w - n;
  size_t shift_count1 = l_shift_count >> 2;
  size_t shift_count2 = l_shift_count - shift_count1;

  unsigned rotated_left_shift =
      left_shift | ((x >> shift_count1) >> shift_count2);

  return rotated_left_shift;
}

void test_rotate_left() {
  munit_assert_uint(rotate_left(0x12345678, 4), ==, 0x23456781);
  munit_assert_uint(rotate_left(0x12345678, 20), ==, 0x67812345);
}

/*
 * 2.70
 * Return 1 when x can be represented as an n—bit, 2’s-complement
 * number; 0 otherwise
 * Assume 1 <= n <= w
 */
int fits_bits(int x, int n) {
  int w = sizeof(x) << 3;

  // masked value will be x itself if n bit can satisfy positive number.
  int satisfies_if_positive = !(x ^ (x & full_mask_of_size(1, n)));

  // 000000000000 11111111111 (n-1)
  int right_side = x & (full_mask_of_size(1, n - 1));

  int left_side_one = full_mask_of_size(1, w - n + 1) << (n - 1);
  int left_side_zero = full_mask_of_size(0, w - n) << (n - 1);

  // negative || positive
  return !((right_side | left_side_one) ^ x) || !(x ^ right_side);
}

int fits_bits_sol(int x, int n) {
  int w = sizeof(int) << 3;
  int offset = w - n;
  return (x << offset >> offset) == x;
}

void test_fits_bits() {
  munit_assert_int(fits_bits(1, 1), ==, 0); // signed bit will take everything
  munit_assert_int(fits_bits(1, 2), ==, 1);
  munit_assert_int(fits_bits(1, 32), ==, 1);
  munit_assert_int(fits_bits(-1, 1), ==, 1);
  munit_assert_int(fits_bits(-2, 1), ==, 0);
  munit_assert_int(fits_bits(-1, 2), ==, 1);
  munit_assert_int(fits_bits(-2, 2), ==, 1);

  // sol
  munit_assert_int(fits_bits_sol(1, 1), ==,
                   0); // signed bit will take everything
  munit_assert_int(fits_bits_sol(1, 2), ==, 1);
  munit_assert_int(fits_bits_sol(1, 32), ==, 1);
  munit_assert_int(fits_bits_sol(-1, 1), ==, 1);
  munit_assert_int(fits_bits_sol(-2, 1), ==, 0);
  munit_assert_int(fits_bits_sol(-1, 2), ==, 1);
  munit_assert_int(fits_bits_sol(-2, 2), ==, 1);
}

// 6.71
typedef unsigned packed_t;

// 6.71 A) Will not maintain the sign when it is int
// 6.71 B)
int xbyte(packed_t word, int bytenum) {
  int shift = 32 - ((bytenum + 1) << 3);

  return ((int)(word << shift)) >> 24;
}

void test_xbyte() {
  munit_assert_int(xbyte(-1, 0), ==, -1);
  munit_assert_int(xbyte(-1, 1), ==, -1);
  munit_assert_int(xbyte(-1, 2), ==, -1);
  munit_assert_int(xbyte(-1, 3), ==, -1);

  munit_assert_int(xbyte(0, 0), ==, 0);
  munit_assert_int(xbyte(0, 1), ==, 0);
  munit_assert_int(xbyte(0, 2), ==, 0);
  munit_assert_int(xbyte(0, 3), ==, 0);

  munit_assert_int(xbyte(1, 0), ==, 1);
  munit_assert_int(xbyte(1, 3), ==, 0);

  munit_assert_int(xbyte(255, 0), ==, -1);
  munit_assert_int(xbyte(255, 2), ==, 0);
}

// 2.72 A) Because size_t is unsigned and the calculation gets converted to
// uint, which makes the check useless

// 2.72 B)
void copy_int(int val, void *buf, int maxbytes) {
  if (maxbytes - (int)sizeof(val) >= 0) {
    memcpy(buf, (void *)&val, sizeof(val));
  }
}

void copy_int_sol(int val, void *buf, int maxbytes) {
  if (maxbytes >= (int)sizeof(val)) {
    memcpy(buf, (void *)&val, sizeof(val));
  }
}

/*
 * If sign is 0 returns 0 otherwise value.
 */
int silence_value(int value, unsigned sign) {
  int mask = full_mask_of_size(sign, sizeof(int) << 3);
  return value & mask;
}

// Either of
int select_value(int select_b, int a, int b) {
  int mask = full_mask_of_size(1, sizeof(int));
  a = silence_value(a, !select_b);
  b = silence_value(b, select_b);
  return a | b;
}

// 2.73
int saturating_add(int x, int y) {
  int res = x + y;

  // msb
  int x_sign = ((unsigned)x) & 0x80000000;

  // msb
  int y_sign = ((unsigned)y) & 0x80000000;

  // msb
  int res_sign = ((unsigned)res) & 0x80000000;

  int is_overflow = (x_sign == y_sign) && (x_sign != res_sign);

  int r1 = select_value(!(is_overflow && !x_sign), INT_MAX, res);
  int r2 = select_value(!(is_overflow && x_sign), INT_MIN, r1);
  return r2;
}

void test_saturating_add() {
  munit_assert_int(saturating_add(1, 1), ==, 2);
  munit_assert_int(saturating_add(INT_MAX, INT_MAX), ==, INT_MAX);
  munit_assert_int(saturating_add(INT_MAX - 1, INT_MAX - 1), ==, INT_MAX);
  munit_assert_int(saturating_add(INT_MIN, INT_MIN), ==, INT_MIN);
  munit_assert_int(saturating_add(INT_MIN + 1, INT_MIN + 1), ==, INT_MIN);
  munit_assert_int(saturating_add(1, -2), ==, -1);
  munit_assert_int(saturating_add(-2, 1), ==, -1);
}

int tadd_ok(int x, int y) {

  int res = x + y;

  // msb
  int x_sign = ((unsigned)x) & 0x80000000;

  // msb
  int y_sign = ((unsigned)y) & 0x80000000;

  // msb
  int res_sign = ((unsigned)res) & 0x80000000;

  return !((x_sign == y_sign) && (x_sign != res_sign));
}

// 2.74
int tsub_ok(int x, int y) {
  int res = x - y;
  int res_sign = res & INT_MIN;
  int y_sign = y & INT_MIN;
  int x_sign = x & INT_MIN;
  return select_value(y == INT_MIN, tadd_ok(x, ~y + 1), !!x_sign);
}

void test_tsub_ok() {
  munit_assert_int(tsub_ok(1, 1), ==, 1);
  munit_assert_int(tsub_ok(INT_MAX, INT_MAX), ==, 1);
  munit_assert_int(tsub_ok(INT_MAX - 1, INT_MAX - 1), ==, 1);
  munit_assert_int(tsub_ok(INT_MIN, INT_MIN), ==, 1);
  munit_assert_int(tsub_ok(INT_MIN + 1, INT_MIN + 1), ==, 1);
  munit_assert_int(tsub_ok(1, -2), ==, 1);
  munit_assert_int(tsub_ok(-2, 1), ==, 1);
  munit_assert_int(tsub_ok(1, 1), ==, 1);

  munit_assert_int(tsub_ok(INT_MIN, 1), ==, 0);
  munit_assert_int(tsub_ok(INT_MIN, INT_MAX), ==, 0);
  munit_assert_int(tsub_ok(INT_MAX, -1), ==, 0);
  munit_assert_int(tsub_ok(INT_MAX, INT_MIN), ==, 0);

  munit_assert_int(tsub_ok(INT_MAX - 1, -1), ==, 1);
}

// 2.75
// TODO: cannot understand solution

int signed_high_prod(int x, int y);
int signed_high_prod(int x, int y) {
  assert(false); // unimplemented
}
unsigned unsigned_high_prod(unsigned x, unsigned y) {
  return (unsigned)signed_high_prod(x, y);
}

// 2.76
void *my_calloc(size_t nmemb, size_t size) {
  if (nmemb == 0 || size == 0) {
    return NULL;
  }

  size_t total_size = nmemb * size;

  if (total_size / size != nmemb) {
    return NULL; // overflow
  }

  void *ptr = malloc(total_size);
  if (ptr == NULL) {
    return NULL;
  }

  memset(ptr, 0, total_size);
  return ptr;
}

// 2.77
void mul_exprs() {
  int x = 0;

  // A) 17
  x = (x << 4) + x;

  // B) -7
  x = x - (x << 3);

  // C) 60
  x = (x << 6) - (x << 2);

  // D) -112
  x = (x << 4) - (x << 7);
}

// 2.78 (TOOD: redo- checked the solution before solving)
int divide_power2(int x, int k) {
  int is_neg = !!(x & INT_MIN);
  x = select_value(is_neg, x, x + (1 << k) - 1); // add bias
  return x >> k;
}

void test_divide_power2() {
  munit_assert_int(divide_power2(1, 2), ==, 1 / (1 << 2));
  munit_assert_int(divide_power2(-3, 2), ==, -3 / (1 << 2));
  munit_assert_int(divide_power2(-4, 2), ==, -4 / (1 << 2));
  munit_assert_int(divide_power2(-7, 2), ==, -7 / (1 << 2));
  munit_assert_int(divide_power2(-8, 2), ==, -8 / (1 << 2));
  munit_assert_int(divide_power2(4, 2), ==, 4 / (1 << 2));
  munit_assert_int(divide_power2(INT_MAX, 2), ==, INT_MAX / (1 << 2));
}

// 2.79
int mul3div4(int x) { return divide_power2(((x << 1) + x), 2); }

void test_mul3div4() {
  munit_assert_int(mul3div4(1), ==, 0);
  munit_assert_int(mul3div4(-3), ==, (-3 * 3) / 4);
  munit_assert_int(mul3div4(-4), ==, (-4 * 3) / 4);
  munit_assert_int(mul3div4(-7), ==, (-7 * 3) / 4);
  munit_assert_int(mul3div4(-8), ==, (-8 * 3) / 4);
  munit_assert_int(mul3div4(4), ==, (4 * 3) / 4);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
  munit_assert_int(mul3div4(INT_MAX), ==, (INT_MAX * 3) / 4);
#pragma GCC diagnostic pop
}

int mul_12(int x) { return (x << 4) - (x << 2); }

// 2.80
int threefourths(int x) {
  int div1 = divide_power2(x, 1);
  int div2 = divide_power2(x, 2);

  int rem1 = x - (div1 << 1);
  int rem2 = x - (div2 << 2);

  return div1 + div2 + divide_power2((rem1 << 1) + rem2, 2);
}

void test_threefourths() {
  munit_assert_int(threefourths(1), ==, 0);
  munit_assert_int(threefourths(-3), ==, -2);
  munit_assert_int(threefourths(-4), ==, -3);
  munit_assert_int(threefourths(-7), ==, -5);
  munit_assert_int(threefourths(-8), ==, -6);
  munit_assert_int(threefourths(4), ==, 3);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
  munit_assert_int(threefourths(INT_MAX), ==,
                   (int)(((long int)INT_MAX * 3) / 4));
#pragma GCC diagnostic pop
}

// 2.81	a)
/* returns 1{w-k}0{k} */
unsigned generate_patterns_a(size_t k) {
  unsigned all_one = (unsigned)-1;
  return all_one << k;
}

void test_generate_patterns_a() {
  munit_assert_uint(generate_patterns_a(3), ==, 0xFFFFFFF8);
  munit_assert_uint(generate_patterns_a(0), ==, 0xFFFFFFFF);
}

// 2.81	b)
unsigned generate_patterns_b(size_t j, size_t k) {
  // 1{w}
  unsigned res = (unsigned)-1;

  // 1{w-k-j}0{k+j}
  res = res << (k + j);

  // 0{w-k-j}1{k+j}
  res = ~res;

  res &= generate_patterns_a(j);

  return res;
}

void test_generate_patterns_b() {
  munit_assert_uint(generate_patterns_b(1, 1), ==, 2);
  munit_assert_uint(generate_patterns_b(0, 0), ==, 0);
  munit_assert_uint(generate_patterns_b(2, 1), ==, 4);
  munit_assert_uint(generate_patterns_b(2, 2), ==, 12);
}

// 2.82
// Given
// int x = random();
// int y = random();
// unsigned ux = (unsigned) x;
// unsigned uy = (unsigned) y;
//
// 1. (x<y) == (-x > -y), No, when X is INT_MIN
// 2. ((x+y)<<4) + y-x == 17*y + 15*x, true
// 3. ~x + ~y + 1 = ~(x+y) true
// 4. (ux-uy) == -(unsigned)(y-x) true
// 5. ((x >> 2) << 2) <= x; true

void repl_282() {
  int x = INT_MAX / 15;
  int y = 0;
  unsigned ux = (unsigned)x;
  unsigned uy = (unsigned)y;

  int xn = -x;
  int yn = -y;

  int xn2 = -x > -y;

  // Weird bug, -x > -y is true? WHY?
  // -x > y is false? (even when is zero :( )
  // printf("x=%d, -x=%d, y=%d, -y=%d, %d==%d, %d\n",x, -x, y, -y, (x<y), (-x >
  // -y), (x<y) == (-x > -y)); printf("xn=%d, yn=%d, xn<yn=%d, xn>yn=%d ... %d
  // .... %d\n",xn, yn, (xn < yn), (xn > yn), (int)((int)(-x) > (int)(-y)),
  // xn2); printf("xn=%d, yn=%d, xn<yn=%d, xn>yn=%d ... %d .... %d\n",xn, yn,
  // (xn < yn), (xn > yn), (int)((int)(INT_MIN) > (int)(-y)), xn2);
  printf("%d\n", (((x + y) << 4) + y - x) == (17 * y + 15 * x));

  // printf('%d\n', (x<y) == (-x > -y));
  // printf('%d\n', (x<y) == (-x > -y));
}

// 2.83

void repl_283() {

  /*
  A)
  x = 0.yyyyyyyyyyyyy (y = k-bit)
  Y = B2U_k(y) (calculating decimal representation of y)

  x = ??? (Y, k)
  x = (Y >> k) + (Y >> (k*2)) + (Y >> (k*3))

  x = Y/(2^k) + Y/(2^(k*2)) + Y/(2^{k*3	})

  sol:
  n = 0.yyyyyyyyyyyyyy
  n << k = y.yyyyyyyyy
  (n <<k ) -n = y
  (n * 2^k) -n = y

  n = y/((2^k) -1)





  B)
  101 => k = 3, Y = 5
  5/(8) + 5/64 + 5/64*64 = 0.714111328125

  0110 => k = 4, Y = 6  = 0.399993896484375

  010011 => k = 6, Y = 19  = 0.3015872836112976






  */
}

unsigned f2u(float x) { return *((unsigned *)&x); }

// 2.84
int float_le(float x, float y) {
  unsigned ux = f2u(x);
  unsigned uy = f2u(y);

  unsigned sx = ux >> 31;
  unsigned sy = uy >> 31;

  // ux, uy, sx, sy
  return select_value(sx != sy,
                      select_value(sx == 1, (ux & INT_MAX) <= (uy & INT_MAX),
                                   (uy & INT_MAX) <= (ux & INT_MAX)),
                      sx == 1 || ((ux & INT_MAX) == 0 && (uy & INT_MAX) == 0));
}

void test_float_le() {
  munit_assert_int(float_le(1.2, 2.3), ==, 1);
  munit_assert_int(float_le(2.3, 1.2), ==, 0);
  munit_assert_int(float_le(-2.3, 1.2), ==, 1);

  munit_assert_int(float_le(-0, +0), ==, 1);
  munit_assert_int(float_le(+0, -0), ==, 1);

  munit_assert_int(float_le(-2.3, -2.3), ==, 1);
  munit_assert_int(float_le(-2.3, -3.3), ==, 0);
}

// 2.85
void repl_285() {
  /*
  k-bit exponent
  n-bit fraction



  7.0
  4 * 7/4 => 4 * 1.75 => 4 * 0b0.11

  n-bit fraction = .11
  k-bit exponent = E = v - (2^(k-1)) = 4
  e = 4 + 2^(k-1)

  k               n
   ............. ...............
  01000000000100 110000000000000


  k               n
   ............. ...............
  01000000000100 110000000000000

  k               n
   ............. ...............
  01000000000100 110000000000000


  largest odd number
        k
  .............
  0111111111111
  E = 2^k -1 - 2**(k-1)

  1.111111111111111111111


  */
}

void repl_2_89() {
  // TODO:
  int x = 1;
  int y = (1 << 30) + 2;
  int z = -y;

  double dx = x;
  double dy = y;
  double dz = z;

  printf("%f, %f\n", (dx + dy) + dz, dx + (dy + dz));
}

float u2f(unsigned x) { return INTO(float, x); }

// 2.90
float fpwr2(int x) {
  // assuming 32-bit
  // bias = 2**(7) -1 => 127
  // min = -126 -24), max = 126 ~127
  unsigned exp, frac;
  unsigned u;
  // unsigned __;
  if (x < -149) { // return 0.0
    exp = 0;
    frac = 0;
  } else if (x < -126) { // denormalized
    exp = 0;
    frac = 1 << (x + 149);
  } else if (x < 128) { // normalized
    exp = x + 127;
    frac = 0;
  } else { // inf
    exp = 0xff;
    frac = 0;
  }
  u = exp << 23 | frac;
  return u2f(u);
}

typedef unsigned float_bits;

bool is_nan(float_bits f) {

  unsigned exp = (f >> 23) & 0xFF;
  unsigned factor = f & ((1 << 23) - 1);

  if (factor != 0 && exp == 0xFF) {
    return true;
  }
  return false;
}

void test_is_nan() {
  unsigned print_every = 0xFFFFFF;

  // full-search (takes somewhere in order of 10 minutes)
  unsigned end = 0xFFFFFFFF;

  // quick-search
  end = 0xFFFFF;

  munit_assert_uint(is_nan(end), ==, isnan(u2f(end)));
  for (unsigned i = 0; i < end; i++) {
    if (i % print_every == 0) {
      printf("is_nan: [%d/%d]\n", i / print_every, UINT_MAX / print_every);
    }
    munit_assert_uint(is_nan(i), ==, isnan(u2f(i)));
  }
}

bool is_pos_nan(float_bits f) { return is_nan(f) && !(f >> 31); }

// 2.92
float_bits float_negate(float_bits f) {
  if (is_nan(f)) {
    return f;
  }

  unsigned sign = f >> 31;
  return ((~sign) << 31) | (f & (((unsigned)1 << 31) - 1));
}

void test_float_negate() {
  munit_assert_uint(float_negate(f2u(-1.0)), ==, f2u(1.0));
  munit_assert_uint(float_negate(f2u(1)), ==, f2u(-1));
  munit_assert_uint(float_negate(f2u(-123)), ==, f2u(123));

  float infinite = 1 / 0.0;
  munit_assert_uint(float_negate(f2u(1 / 0.0)), ==, f2u(-1 / 0.0));
  munit_assert_uint(float_negate(f2u(-1 / 0.0)), ==, f2u(1 / 0.0));

  // Should not do anything in case of nan values
  munit_assert_uint(float_negate(f2u(infinite - infinite)), ==,
                    f2u(infinite - infinite));

  // full-search (takes somewhere in order of 10 minutes)
  unsigned end = 0xFFFFFFFF;

  // quick-search
  end = 0xFFFFF;
  unsigned print_every = 0xFFFFFF;

  for (unsigned i = 0; i < end; i++) {
    if (i % print_every == 0) {
      printf("float_negate: [%d/%d]\n", i / print_every,
             UINT_MAX / print_every);
    }
    // printf("%d\n", i);
    // printf("%f\n", u2f(i));
    // printf("%.8x\n", i);
    // printf("%.8x\n", float_negate(i));
    if (is_nan(i)) {
      munit_assert_uint(float_negate(i), ==, i);
    } else {
      munit_assert_uint(float_negate(i), ==, f2u(-u2f(i)));
    }
  }
}

// 2.93
float_bits float_absval(float_bits f) {
  // The book suggests to leave NaN untouched, but c implementation
  // changes the -Nan s
  if (is_pos_nan(f)) {
    return f;
  }
  unsigned sign_bit = f >> 31;
  if (sign_bit) {
    f = f & ~(1 << 31);
  }
  return f;
}

void test_float_absval() {
  unsigned print_every = 0xFFFFFF;

  // full-search (takes somewhere in order of 10 minutes)
  unsigned end = 0xFFFFFFFF;

  // quick-search
  end = 0xFFFFF;

  munit_assert_uint(float_absval(end), ==, f2u((float)fabs(u2f(end))));
  for (unsigned i = 0; i < end; i++) {
    if (i % print_every == 0) {
      printf("[%d/%d]\n", i / print_every, UINT_MAX / print_every);
    }
    munit_assert_uint(float_absval(i), ==, f2u((float)fabs(u2f(i))));
  }
}

// 2.94
float_bits float_twice(float_bits f) {
  if (is_nan(f)) {
    return f;
  }
  unsigned exp = (f >> 23) & 0xff;
  unsigned sign = f >> 31;
  unsigned factor = (f) & ((1 << 23) - 1);

  // infinite
  if (exp == 0xff && factor == 0) {
    return f;

    // normalized
  } else if (exp != 0) {
    if (exp + 1 == 0xff) { // make infinite
      return 0x7f800000 | (sign << 31);
    }
    exp += 1;
    return (sign << 31) | exp << 23 | factor;

    // de-normalized
  } else {

    // either it'll remain de-normalized.
    // or the 1 will get used up in exp
    // both will do the task.
    return (sign << 31) | factor << 1;
  }
}

void test_float_twice() {
  unsigned print_every = 0xFFFFFF;

  // full-search (takes somewhere in order of 10 minutes)
  unsigned end = 0xFFFFFFFF;

  // quick-search
  end = 0xFFFFF;

  munit_assert_uint(float_absval(end), ==, f2u((float)fabs(u2f(end))));
  // for(unsigned i=0; i < end; i++ ) {
  for (unsigned i = 0; i < end; i++) {
    if (i % print_every == 0) {
      printf("float_twice [%d/%d]\n", i / print_every, UINT_MAX / print_every);
    }
    if (is_nan(i)) {
      munit_assert_uint(float_twice(i), ==, i);
    } else {
      munit_assert_uint(float_twice(i), ==, f2u(2.0 * u2f(i)));
    }
  }
}

// 2.95
float_bits float_half(float_bits f) {
  if (is_nan(f)) {
    return f;
  }
  unsigned exp = (f >> 23) & 0xff;
  unsigned sign = f >> 31;
  unsigned factor = (f) & ((1 << 23) - 1);

  // infinite
  if (exp == 0xff && factor == 0) {
    return f;
    // end - result will remain normalized
  } else if (exp > 1) {

    exp -= 1;

    return (sign << 31) | exp << 23 | factor;

    // currently normalized, end-result de-normalized or normalized (depending
    // on rounding)
  } else if (exp == 1) {
    exp = 0;

    unsigned factor_end_prev = factor & 1;

    factor = factor >> 1;
    factor += 1 << 22; // add the normalized one
    unsigned factor_end_now = factor & 1;

    // round-to-even
    if (factor_end_prev != 0) {
      if (factor_end_now == 1) {
        // will overflow to cover the exp in case of normalized result;
        factor += 1;
      }
    }

    return (sign << 31) | factor;

    // currently de-normalized, end-result de-normalized
  } else {

    unsigned factor_end_prev = factor & 1;

    factor = factor >> 1;
    unsigned factor_end_now = factor & 1;

    // round-to-even
    if (factor_end_prev != 0) {
      if (factor_end_now == 1) {
        // will overflow to cover the exp in case of normalized result;
        factor += 1;
      }
    }

    // either it'll remain de-normalized.
    // or the 1 will get used up in exp
    // both will do the task.
    return (sign << 31) | factor;
  }
}

void test_float_half() {
  unsigned print_every = 0xFFFFFF;

  // full-search (takes somewhere in order of 10 minutes)
  unsigned end = 0xFFFFFFFF;

  // quick-search
  end = 0xFFFFF;

  munit_assert_uint(float_absval(end), ==, f2u((float)fabs(u2f(end))));
  for (unsigned i = 0; i < end; i++) {
    if (i % print_every == 0) {
      printf("float_half: [%d/%d]\n", i / print_every, UINT_MAX / print_every);
    }
    if (is_nan(i)) {
      munit_assert_uint(float_half(i), ==, i);
    } else {
      munit_assert_uint(float_half(i), ==, f2u(0.5 * u2f(i)));
    }
  }
}

// 2.96
// returns 0x80000000 if f is Nan or conversion overflows;
int float_f2i(float_bits f) {
  unsigned inconvertable = 0x80000000;
  if (is_nan(f)) {
    return inconvertable;
  }

  unsigned exp = (f >> 23) & 0xff;
  unsigned is_positive = 1 - (f >> 31);
  unsigned factor = (f) & ((1 << 23) - 1);

  // bias = 127
  // max int = INT_MAX = 2**31 - 1, E < 2**31 = E= 31 + 127 = 158 (check
  // less_then) min int = INT_MIN = -2**31 = -1 * 2^(31), E = 31 + 127 = 158
  // (check less_then_equal)

  if (is_positive) {
    if (f >= (unsigned)0x4F000000) {
      return inconvertable;
    }
    unsigned rv = f;
  } else {
    if (f >= (unsigned)0xCF000000) {
      return inconvertable;
    }
  }

  unsigned rv = factor;

  if (exp != 0) { // add the normalized one
    rv += 1 << 23;
  }
  int e = (int)exp - 127 - 23; // - 23 to cover the factor
  if (e < 0) {

    // underflow
    if (e <= -32) {
      rv = 0;
    } else {

      rv = rv >> (-e);
    }
  } else {
    // overflow can't occur as we've checked for infinity above
    rv = rv << e;
  }
  // printf("rv=%d\n", rv);
  if (!is_positive) {
    rv = ~(int)rv + 1;
  }
  return (int)rv;
}

void test_float_f2i() {
  unsigned print_every = 0xFFFFFF;

  // full-search (takes somewhere in order of 10 minutes)
  unsigned end = 0xFFFFFFFF;

  // quick-search
  end = 0xFFFFF;

  munit_assert_uint(float_f2i(end), ==, (int)u2f(end));
  for (unsigned i = 0; i < end; i++) {
    if (i % print_every == 0) {
      printf("float_half: [%d/%d]\n", i / print_every, UINT_MAX / print_every);
    }
    if (is_nan(i)) {
      munit_assert_uint(float_f2i(i), ==, 0x80000000);
    } else {
      if (float_f2i(i) != (int)u2f(i)) {
        printf("%u\n", i);
        printf("%.8X\n", i);
        printf("%f\n", u2f(i));
        munit_assert_uint(float_f2i(i), ==, (int)u2f(i));
      }
    }
  }
}

float_bits float_i2f(int i) {
  // always be normalized
  unsigned factor = i;
  unsigned is_positive = 1 - ((unsigned)i >> 31);
  unsigned bias = 127;
  if (i == INT_MIN) {
    // Haha: luckily it works without this change.
    // TODO:
  }
  if (!is_positive) {
    factor = ~i + 1;
  }
  if (i == 0) {
    return 0;
  }

  int msb_one_at = 0;
  for (int i =31; i >= 0; i-- ) { if (factor >> i & 1) {
      msb_one_at = i; 
      break;
    }
  }
  // printf("before msb fix factor=%0.8X\n", factor);
  factor &= ~(1 << msb_one_at); // remove the normalized one
  int shift_by = 23 -msb_one_at;
  unsigned exp = bias + 23 - shift_by;  
  // printf("before shift factor=%0.8X\n", factor);
  if (shift_by < 0) {
    unsigned prev = factor >> (-1-shift_by) &1; 
    unsigned old_factor = factor;
    factor >>= (-shift_by);
    unsigned curr = factor&1; 

    // rounding:

    // round-to-eve
    // when the removed part is like: 1000000
    if (prev == 1 && (((factor << -shift_by) + (1 << (-1-shift_by)))) == old_factor) {
      if (curr != 0) {
        factor += 1;
        // can only happen if all factor is 111111..
        if ((factor >> 23) & 1) {
          exp += 1;
          factor &= ~(1 << 23);
        }        
      }
    }
    // round to top
    // when the removed part is 1**1**
    else if (prev == 1) {
        factor += 1;
        // can only happen if all factor is 111111..
        if ((factor >> 23) & 1) {
          exp += 1;
          factor &= ~(1 << 23);
        }        
    }

    // round to bottom (do nothing)
    // when the removed part is 0*****

  } else {
    factor <<= (shift_by);
  }

  return ((1-is_positive) << 31) | exp << 23 | factor;
}

void test_float_i2f() {
  unsigned print_every = 0xFFFFFF;

  // full-search (takes somewhere in order of 10 minutes)
  unsigned end = 0xFFFFFFFF;

  // quick-search
  end = 0xFFFFF;

  munit_assert_uint(float_i2f(end), ==, f2u((float)(int)end));
  for (unsigned i = 0; i < end; i++) {
    if (i % print_every == 0) {
      printf("float_i2f: [%d/%d]\n", i / print_every, UINT_MAX / print_every);
    }
    if (is_nan(i)) {
      munit_assert_uint(float_f2i(i), ==, 0x80000000);
    } else {
      if (float_i2f(i) != f2u((float)(int)i)) {
        printf("%d\n", i);
        printf("%.8X\n", i);
        printf("%f\n", u2f(i));
        munit_assert_uint(float_i2f(i), ==, f2u((float)(int)i));
      } }
  }
}

void run_tests() {
  int test_count = 0;

  test_full_mask_of_size();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

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

  test_rotate_left();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_fits_bits();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_xbyte();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_saturating_add();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_tsub_ok();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_divide_power2();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_mul3div4();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_threefourths();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_generate_patterns_a();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_generate_patterns_b();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_is_nan();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_float_le();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_float_negate();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_float_absval();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_float_twice();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_float_half();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_float_f2i();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);

  test_float_i2f();
  test_count += 1;
  printf("%2d Tests passed!\n", test_count);
    
}

int get_one() { return 1; }
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
  // printf("%f\n", u2f(4286578689));
  // printf("%f\n", u2f(2139095041));
  // return 1;

  // unsigned i = 2147483648;

  // if (float_f2i(i) != (int)u2f(i)) {
  // 	printf("%u\n", i);
  // 	printf("%.8X\n", i);
  // 	printf("%f\n", u2f(i));
  // }
  // int res = float_f2i(i);
  // munit_assert_int(float_f2i(i), ==, (int)u2f(i));
  // return 1;

  run_tests();

  repl_2_89();
  // show_float(-0.0);
  // float a= 1.0;
  // printf("%X", *(unsigned int *)&a);

  // int b = 0x7F7FFFFF;
  // int b = 0x007FFFFF;
  // int b = 0x7F7FFFFF; // 23 + bias => 22 + 128 => 150
  // float a= INTO(float, b);
  // printf("%X\n", *(unsigned int *)&a);
  // printf("%.30f\n",a);
}
