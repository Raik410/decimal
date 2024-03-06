#include "../s21_decimal.h"
#include "helpers.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;

  int sign = get_sign(result);
  set_sign(result, !sign);

  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  s21_decimal zero = {0};
  *result = zero;
  int exp = get_exponent(&value);
  clear_exp(&value);
  division_decimal_by_ten(&value, exp);
  copy_decimal(value, result);
  return 0;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int sign = get_sign(&value);
  s21_truncate(value, result);
  s21_decimal temp = {{sign, 0, 0, 0}};
  add_decimal_without_exponent_sign(*result, temp, result);
  set_sign(result, sign);
  return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int exp = get_exponent(&value);
  s21_truncate(value, result);
  s21_decimal decimal_without_fraction = *result;
  multiple_decimal_with_shift_by_ten(&decimal_without_fraction, exp);
  sub_decimal_without_exponent_sign(value, decimal_without_fraction, &value);
  division_decimal_by_ten(&value, exp - 1);
  s21_decimal temp_2 = {{value.bits[0] > 4, 0, 0, 0}};
  add_decimal_without_exponent_sign(*result, temp_2, result);
  return 0;
}