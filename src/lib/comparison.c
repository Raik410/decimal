#include "../s21_decimal.h"
#include "helpers.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 1;

  bool sign = get_sign(&value_1);
  bool sign_second = get_sign(&value_2);
  int exponent = get_exponent(&value_1);
  int exponent_second = get_exponent(&value_2);

  if (sign != sign_second) result = 0;
  if (exponent != exponent_second) result = 0;

  for (int i = 95; i >= 0 && result; i--) {
    result = equal_bit(value_1.bits[i / 32], value_2.bits[i / 32], i % 32);
  }

  return result;
}

int s21_is_not_equal(const s21_decimal decimal_one,
                     const s21_decimal decimal_second) {
  return !s21_is_equal(decimal_one, decimal_second);
}

int s21_is_greater(s21_decimal decimal_one, s21_decimal decimal_second) {
  bool result = true;
  bool check = false;

  if (s21_is_equal(decimal_one, decimal_second)) result = false;
  int sign_decimal_one = get_sign(&decimal_one);
  int sign_decimal_second = get_sign(&decimal_second);

  if (sign_decimal_one < sign_decimal_second)
    result = true;
  else if (sign_decimal_one > sign_decimal_second)
    result = false, check = true;

  unsigned int exp_decimal_one = get_exponent(&decimal_one);
  unsigned int exp_decimal_second = get_exponent(&decimal_second);

  if (exp_decimal_one > 28)
    less_decimal(&decimal_one, exp_decimal_one, 28), exp_decimal_one = 28;
  if (exp_decimal_second > 28)
    less_decimal(&decimal_second, exp_decimal_second, 28),
        exp_decimal_second = 28;

  s21_decimal copy_decimal_one = decimal_one;
  s21_decimal copy_decimal_second = decimal_second;

  casting_equal_exponent(&copy_decimal_one, &copy_decimal_second,
                         exp_decimal_one, exp_decimal_second);
  copy_decimal_one.bits[3] = 0, copy_decimal_second.bits[3] = 0;

  if (result && !check) {
    check = sign_decimal_one && sign_decimal_second
                ? !greater_decimal(copy_decimal_one, copy_decimal_second)
                : greater_decimal(copy_decimal_one, copy_decimal_second);
  }

  return check && result;
}

int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return s21_is_greater(dec1, dec2) || s21_is_equal(dec1, dec2);
}

int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_greater(dec1, dec2);
}

int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_greater(dec1, dec2) || s21_is_equal(dec1, dec2);
}