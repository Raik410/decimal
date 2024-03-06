#include "lib/helpers.h"
#include "s21_decimal.h"

int main() {
  s21_decimal a = {{INT32_MAX - 1, 0, 0, 0}};
  s21_decimal b = {{1000000, 0, 0, 0}};

  s21_decimal res = {{0, 0, 0, 0}};

  int bits_in_mantis = 96;
  int bits_in_place_number = 32;

  //    printf("Decimal a: ");
  //    print_decimal(a);
  //    printf("Decimal b: ");
  //    print_decimal(b);
  //    printf("Decimal res: ");
  //    print_decimal(res);
  //    printf("\n\n");

  for (int i = 0; i < bits_in_mantis; i++) {
    int value = a.bits[i / bits_in_place_number];

    if (!check_bit(value, i % bits_in_place_number)) {
      continue;
    }

    s21_decimal copy_b = {{0, 0, 0, 0}};
    copy_decimal(b, &copy_b);
    shift_left(&copy_b, i);
    add_decimal_without_exponent_sign(res, copy_b, &res);

    //        printf("Decimal b: ");
    //        print_decimal(b);
    //        printf("Decimal res: ");
    //        print_decimal(res);
  }

  print_decimal(res);
  return 0;
}

int main() {
  // s21_long_decimal decimal1 = {{10, 0, 0, 0, 0, 0, 0}};
  // s21_long_decimal decimal2 = {{7, 0, 0, 0, 0, 0, 0}};
  s21_decimal decimal1 = {{2121, 0, 0, 0}};
  s21_decimal decimal2 = {{10, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  div_decimal_without_exponent_sign(decimal1, decimal2, &res);
  print_decimal(res);
  return 0;
}

void delim(int a, int b) {
  int S = 0;
  int i = 0;
  int exp = 0;
  int tmp = 0;
  while (a >= b) {
    int copy_b = b;
    i = 0;
    while ((copy_b << 1) <= a) {
      copy_b <<= 1;
      i++;
    }

    S += (1 << i);

    a -= copy_b;

    printf("i is %d\n", i);
    printf("S is %d\n", S);
    printf("a is %d\n", a);
  }

  while (a) {
    exp++;
    a *= 10;            // 10
    S *= 10;            // 20
    tmp = a / b;        // 5
    S += tmp;           // 25
    a = a - (tmp * b);  //
  }

  printf("S is %d\n", S);
  printf("a is %d\n", a);
  printf("exp is %d\n", exp);
}

int main() {
  // int a = 101;
  // int b = 50;
  s21_decimal decimal = {{100, 0, 0, 0}};
  s21_decimal decimal_second = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  // delim(a, b);
  div_decimal_without_exponent_sign(decimal, decimal_second, &result);
  print_decimal(result);
  return 0;
}

int s21_div(s21_decimal decimal_one, s21_decimal decimal_second,
            s21_decimal* result) {
  s21_decimal zero = {{0, 0, 0, 0}};
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_decimal entire = zero;
  s21_decimal rest = div_decimal_without_exponent_sign_fraction(
      decimal_one, decimal_second, &entire);
  rest.bits[3] = 0;
  s21_decimal fraction_expression = zero;
  s21_decimal tmp = zero;

  *result = zero;
  int exponent_one = get_exponent(&decimal_one);
  int exponent_second = get_exponent(&decimal_second);
  int exp_sum = exponent_one >= exponent_second
                    ? exponent_one - exponent_second
                    : exponent_second - exponent_one;

  int sign_one = get_sign(&decimal_one);
  int sign_second = get_sign(&decimal_second);

  int result_sign = sign_one && sign_second ? 0 : sign_one || sign_second;

  int i = 0;

  while (!decimal_are_zero(rest) && i <= 5) {
    exp_sum++;
    i++;
    s21_mul(rest, ten, &rest);
    s21_mul(entire, ten, &entire);
    div_decimal_without_exponent_sign_fraction(rest, decimal_second,
                                               &fraction_expression);
    s21_add(entire, fraction_expression, &entire);
    s21_mul(fraction_expression, decimal_second, &tmp);
    s21_sub(rest, tmp, &rest);
  }
  copy_decimal(entire, result);
  set_sign(result, result_sign);
  set_exponent(result, exp_sum);

  return EXIT_SUCCESS;
}

s21_decimal div_decimal_without_exponent_sign_fraction(
    s21_decimal decimal_one, s21_decimal decimal_second, s21_decimal* result) {
  s21_decimal zero = {{0, 0, 0, 0}};
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal S = zero;
  s21_decimal i = zero;
  *result = zero;

  // bool two_decimal_are_zero =
  //     decimal_are_zero(decimal_one) || decimal_are_zero(decimal_second);
  // if (two_decimal_are_zero) return EXIT_SUCCESS;

  // int result_return = 0;

  // if (decimal_are_zero(decimal_second)) return EXIT_SUCCESS;

  while (s21_is_greater_or_equal(decimal_one, decimal_second)) {
    s21_decimal copy_decimal_second = decimal_second;
    i = zero;
    while (s21_is_less_or_equal(shift_left_output(copy_decimal_second, 1),
                                decimal_one)) {
      shift_left(&copy_decimal_second, 1);
      s21_add(i, one, &i);
    }
    int i_shift = 0;
    s21_from_decimal_to_int(i, &i_shift);
    i_shift = 1 << i_shift;
    s21_from_int_to_decimal(i_shift, &i);
    s21_add(S, i, &S);

    s21_sub(decimal_one, copy_decimal_second, &decimal_one);
  }
  copy_decimal(S, result);

  return decimal_one;
}