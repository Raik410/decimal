#include "../s21_decimal.h"
#include "helpers.h"

int s21_add(s21_decimal decimal_one, s21_decimal decimal_second,
            s21_decimal* result) {
  s21_decimal zero = {{0, 0, 0, 0}};
  s21_long_decimal zero_long = {{0, 0, 0, 0, 0, 0, 0}};
  *result = zero;
  int result_bool = 0;
  int exponent_one = get_exponent(&decimal_one);
  int exponent_second = get_exponent(&decimal_second);

  int sign_one = get_sign(&decimal_one);
  int sign_second = get_sign(&decimal_second);

  if (exponent_one > 28)
    less_decimal(&decimal_one, exponent_one, 28), exponent_one = 28;
  if (exponent_second > 28)
    less_decimal(&decimal_second, exponent_second, 28), exponent_second = 28;

  int largest_exponent =
      exponent_one > exponent_second ? exponent_one : exponent_second;

  decimal_one.bits[3] = 0, decimal_second.bits[3] = 0;

  s21_long_decimal decimal_long_one = zero_long,
                   decimal_long_second = zero_long, tmp = zero_long;

  copy_decimal_to_long_decimal(&decimal_one, &decimal_long_one, 0);
  copy_decimal_to_long_decimal(&decimal_second, &decimal_long_second, 0);
  casting_equal_exponent_long_decimal(&decimal_long_one, &decimal_long_second,
                                      exponent_one, exponent_second);

  if (sign_one != sign_second) {
    if (greater_decimal_long(decimal_long_one, decimal_long_second)) {
      sub_decimal_without_exponent_sign_long(decimal_long_one,
                                             decimal_long_second, &tmp);
    } else {
      sub_decimal_without_exponent_sign_long(decimal_long_second,
                                             decimal_long_one, &tmp);
      sign_one = !sign_one;
    }
  } else {
    result_bool = add_decimal_without_exponent_sign_long(
        decimal_long_one, decimal_long_second, &tmp);
  }
  while (!check_long_decimal_equal_not_zero(tmp) && largest_exponent) {
    less_long_decimal(&tmp, largest_exponent, largest_exponent - 1);
    largest_exponent--;
  }

  if (!result_bool && check_long_decimal_equal_not_zero(tmp)) {
    copy_decimal_to_long_decimal(result, &tmp, 1);
    set_exponent(result, largest_exponent);
    set_sign(result, sign_one);
  } else {
    result_bool = 1 + sign_one;
  }
  return result_bool;
}

int s21_sub(s21_decimal decimal_one, s21_decimal decimal_second,
            s21_decimal* result) {
  s21_decimal zero = {{0, 0, 0, 0}};
  s21_long_decimal zero_long = {{0, 0, 0, 0, 0, 0, 0}};
  *result = zero;
  int result_bool = 0;
  int exponent_one = get_exponent(&decimal_one);
  int exponent_second = get_exponent(&decimal_second);

  int sign_one = get_sign(&decimal_one);
  int sign_second = get_sign(&decimal_second);

  if (exponent_one > 28)
    less_decimal(&decimal_one, exponent_one, 28), exponent_one = 28;
  if (exponent_second > 28)
    less_decimal(&decimal_one, exponent_one, 28), exponent_second = 28;

  int largest_exponent =
      exponent_one > exponent_second ? exponent_one : exponent_second;

  decimal_one.bits[3] = 0, decimal_second.bits[3] = 0;

  s21_long_decimal decimal_long_one = zero_long,
                   decimal_long_second = zero_long, tmp = zero_long;

  copy_decimal_to_long_decimal(&decimal_one, &decimal_long_one, 0);
  copy_decimal_to_long_decimal(&decimal_second, &decimal_long_second, 0);
  casting_equal_exponent_long_decimal(&decimal_long_one, &decimal_long_second,
                                      exponent_one, exponent_second);

  if (sign_one != sign_second) {
    add_decimal_without_exponent_sign_long(decimal_long_one,
                                           decimal_long_second, &tmp);
  } else {
    if (greater_decimal_long(decimal_long_one, decimal_long_second)) {
      sub_decimal_without_exponent_sign_long(decimal_long_one,
                                             decimal_long_second, &tmp);
    } else {
      sub_decimal_without_exponent_sign_long(decimal_long_second,
                                             decimal_long_one, &tmp);
      sign_one = !sign_one;
    }
  }
  while (!check_long_decimal_equal_not_zero(tmp) && largest_exponent) {
    less_long_decimal(&tmp, largest_exponent, largest_exponent - 1);
    largest_exponent--;
  }

  if (check_long_decimal_equal_not_zero(tmp)) {
    copy_decimal_to_long_decimal(result, &tmp, 1);
    set_exponent(result, largest_exponent);
    set_sign(result, sign_one);
  } else {
    result_bool = 1 + sign_one;
  }
  return result_bool;
}

int s21_mul(s21_decimal decimal_one, s21_decimal decimal_second,
            s21_decimal* result) {
  s21_decimal zero = {{0, 0, 0, 0}};
  s21_long_decimal zero_long = {{0, 0, 0, 0, 0, 0, 0}};
  *result = zero;
  int exponent_one = get_exponent(&decimal_one);
  int exponent_second = get_exponent(&decimal_second);
  int exp_sum = exponent_one + exponent_second;

  int sign_one = get_sign(&decimal_one);
  int sign_second = get_sign(&decimal_second);

  if (check_overflow_with_multi(decimal_one, decimal_second) &&
      (!sign_one && !sign_second))
    return 1;
  else if (check_overflow_with_multi(decimal_one, decimal_second) &&
           (sign_one || sign_second))
    return 2;

  int result_return = 0;
  int result_sign = sign_one && sign_second ? 0 : sign_one || sign_second;

  decimal_one.bits[3] = 0, decimal_second.bits[3] = 0;

  s21_long_decimal decimal_long_one = zero_long,
                   decimal_long_second = zero_long, tmp = zero_long;

  copy_decimal_to_long_decimal(&decimal_one, &decimal_long_one, 0);
  copy_decimal_to_long_decimal(&decimal_second, &decimal_long_second, 0);

  bool two_decimal_are_zero =
      decimal_are_zero(decimal_one) || decimal_are_zero(decimal_second);
  if (two_decimal_are_zero) return EXIT_SUCCESS;

  mul_decimal_without_exponent_sign_long(decimal_long_one, decimal_long_second,
                                         &tmp);

  if (exp_sum > 28) {
    less_long_decimal(&tmp, exp_sum, 28);
    exp_sum = 28;
  }

  while (!check_long_decimal_equal_not_zero(tmp) && exp_sum) {
    less_long_decimal(&tmp, exp_sum, exp_sum - 1);
    exp_sum--;
  }

  if (check_long_decimal_equal_not_zero(tmp)) {
    copy_decimal_to_long_decimal(result, &tmp, 1);
    set_exponent(result, exp_sum);
    set_sign(result, result_sign);
  } else {
    result_return = 1 + result_sign;
  }

  return result_return;
}

int s21_div(s21_decimal decimal_one, s21_decimal decimal_second,
            s21_decimal* result) {
  s21_decimal zero = {{0, 0, 0, 0}};
  *result = zero;

  if (decimal_are_zero(decimal_one) || decimal_are_zero(decimal_second))
    return 3;

  int res = div_decimal(decimal_one, decimal_second, result);

  return res;
}