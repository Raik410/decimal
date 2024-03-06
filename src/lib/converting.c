#include "../s21_decimal.h"
#include "helpers.h"

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  s21_decimal zero = {{0, 0, 0, 0}};

  *dst = zero;

  if (src == 0) return EXIT_SUCCESS;
  if (src > INT32_MAX || src < INT32_MIN) return EXIT_FAILURE;

  int sign = 0;

  if (src > 0)
    sign = 0;
  else if (src < 0)
    sign = 1, src *= -1;

  unsigned int temp_src = (unsigned int)src;

  set_exponent(dst, 0);
  set_sign(dst, sign);
  set_mantissa(dst, temp_src);

  return EXIT_SUCCESS;
}

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  *dst = 0;

  unsigned int sign = get_sign(&src);
  unsigned int exponent = get_exponent(&src);
  if (exponent) s21_truncate(src, &src);
  // if (src.bits[1] || src.bits[2]) return EXIT_FAILURE;
  unsigned int mantissa = get_mantissa_decimal_for_int(src);

  unsigned int mantissa_without_fraction = mantissa * pow(10, -exponent);

  if (sign) {
    mantissa_without_fraction = -mantissa_without_fraction;
  }

  *dst = mantissa_without_fraction;

  return EXIT_SUCCESS;
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
  s21_decimal zero = {{0, 0, 0, 0}};
  *dst = zero;
  int sign = 0;
  src < 0 ? sign = 1, src *= -1 : 0;
  unsigned int exponent = get_exponent_from_float(src);
  unsigned int exp_set = 0;
  int check_float_overflow =
      src < 1e-28f || src > powf(2, 96) || exponent == 0xFF;

  if (check_float_overflow) return EXIT_FAILURE;

  float_exp(&exp_set, dst, src);

  set_exponent(dst, exp_set);
  set_sign(dst, sign);

  return check_float_overflow;
}

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  int sign = get_sign(&src);
  sign = (sign == 0) ? 1 : -1;
  long double exp = get_exponent(&src);
  long double mantiss = 0;
  for (int i = 0; i < 96; i++) {
    mantiss += (long double)check_bit(src.bits[i / 32], i % 32) * pow(2, i);
  }
  *dst = (float)(mantiss * sign * powl(10.f, -exp));
  return 0;
}