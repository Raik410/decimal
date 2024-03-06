#include "helpers.h"

#include "../s21_decimal.h"

int check_bit(const int value, const int index) {
  return (value & (1 << index)) != 0;
}

void set_bit(unsigned int* value, const int index, const int bit) {
  *value = bit ? (*value | (1 << index)) : (*value & ~(1 << index));
}

void print_decimal(s21_decimal value) {
  for (int i = 127; i >= 0; i--) {
    if (i == 127 || i == 126 || i == 119 || i == 111 || i == 95 || i == 63 ||
        i == 31) {
      printf(" ");
    }
    printf("%u", check_bit(value.bits[i / 32], i % 32));
  }
  printf("\n");
}

void print_decimal_long(s21_long_decimal value) {
  for (int i = 223; i >= 0; i--) {
    if (i == 127 || i == 126 || i == 119 || i == 111 || i == 95 || i == 63 ||
        i == 31) {
      printf(" ");
    }
    printf("%u", check_bit(value.bits[i / 32], i % 32));
  }
  printf("\n");
}

int get_sign(const s21_decimal* value) { return check_bit(value->bits[3], 31); }

int get_sign_long(const s21_long_decimal* value) {
  return check_bit(value->bits[6], 31);
}

void set_sign(s21_decimal* value, int sign) {
  set_bit(&(value->bits[3]), 31, sign);
}

void set_sign_long(s21_long_decimal* value, int sign) {
  set_bit(&(value->bits[6]), 31, sign);
}

unsigned int get_exponent(const s21_decimal* value) {
  return (value->bits[3] >> 16) & 0xFF;
}

unsigned int get_exponent_long(const s21_long_decimal* value) {
  return (value->bits[6] >> 16) & 0xFF;
}

void set_exponent(s21_decimal* value, unsigned int exponent) {
  value->bits[3] &= ~(0xFF << 16);
  value->bits[3] |= (exponent & 0xFF) << 16;
}

void set_exponent_long(s21_long_decimal* value, unsigned int exponent) {
  value->bits[6] &= ~(0xFF << 16);
  value->bits[6] |= (exponent & 0xFF) << 16;
}

void set_mantissa(s21_decimal* dst, unsigned int src) {
  dst->bits[0] = src;  // Взять младшие 32 бита числа src
  dst->bits[1] = 0;
  dst->bits[2] = 0;
}

unsigned int get_mantissa_decimal_for_int(s21_decimal src) {
  unsigned int result = src.bits[0];

  return result;
}

unsigned int get_exponent_from_float(float src) {
  unsigned int binary_float = *((unsigned int*)&src);
  unsigned int exponent = (binary_float & 0x7F800000) >> 23;

  return exponent;
}

void float_exp(unsigned int* scale_amount, s21_decimal* dst, float src) {
  set_bit(&dst->bits[0], 0, 1);

  for (*scale_amount = 0; *scale_amount <= 28 && (int)src != src;) {
    if ((int)src != src) {  // 1.25 scale = 2 src = 125
      *scale_amount += 1;
      src *= 10;
    }
  }

  unsigned int get_int = (int)src;  // 125
  for (int i = 0; i < 31; i++) {
    set_bit(&dst->bits[0], i, check_bit(get_int, i));
  }
}

void shift_left(s21_decimal* value, int shift) {
  if (shift <= 0) return;
  unsigned int overflow = 0;

  for (int i = 0; i < 3; i++) {
    unsigned int current_overflow = value->bits[i] >> (32 - shift);

    value->bits[i] <<= shift;
    value->bits[i] |= overflow;

    overflow = current_overflow;
  }
}

s21_decimal shift_left_output(s21_decimal value, int shift) {
  if (shift <= 0) return value;
  unsigned int overflow = 0;

  for (int i = 0; i < 3; i++) {
    unsigned int current_overflow = value.bits[i] >> (32 - shift);

    value.bits[i] <<= shift;
    value.bits[i] |= overflow;

    overflow = current_overflow;
  }

  return value;
}

void shift_right(s21_decimal* value, int shift) {
  if (shift <= 0) return;
  unsigned int overflow = 0;

  for (int i = 2; i >= 0; i--) {
    unsigned int current_overflow = value->bits[i] & ((1u << shift) - 1);

    value->bits[i] >>= shift;
    value->bits[i] |= (overflow << (32 - shift));

    overflow = current_overflow;
  }
}

void shift_left_long(s21_long_decimal* value, int shift) {
  if (shift <= 0) return;
  unsigned int overflow = 0;

  for (int i = 0; i < 7; i++) {
    unsigned int current_overflow = value->bits[i] >> (32 - shift);

    value->bits[i] <<= shift;
    value->bits[i] |= overflow;

    overflow = current_overflow;
  }
}

int equal_bit(unsigned int value_1, unsigned int value_2, int index) {
  return check_bit(value_1, index) == check_bit(value_2, index) ? 1 : 0;
}

void casting_equal_exponent(s21_decimal* decimal_one,
                            s21_decimal* decimal_second, unsigned int exp_one,
                            unsigned int exp_second) {
  if (exp_one > exp_second)
    multiple_decimal_with_shift_by_ten(decimal_second, exp_one - exp_second);
  else
    multiple_decimal_with_shift_by_ten(decimal_one, exp_second - exp_one);
}

void casting_equal_exponent_long_decimal(s21_long_decimal* decimal_one,
                                         s21_long_decimal* decimal_second,
                                         unsigned int exp_one,
                                         unsigned int exp_second) {
  if (exp_one > exp_second)
    multiple_decimal_with_shift_by_ten_long(decimal_second,
                                            exp_one - exp_second);
  else if (exp_one < exp_second)
    multiple_decimal_with_shift_by_ten_long(decimal_one, exp_second - exp_one);
}

void multiple_decimal_with_shift_by_ten(s21_decimal* decimal,
                                        int shift_amount) {
  for (int i = 0; i < shift_amount; i++) {
    s21_decimal copy_decimal_second_multi = *decimal;
    shift_left(decimal, 3);  // 0010 10000 + 00100 = 10100
    shift_left(&copy_decimal_second_multi, 1);
    add_decimal_without_exponent_sign(*decimal, copy_decimal_second_multi,
                                      decimal);
  }
}

void multiple_decimal_with_shift_by_ten_long(s21_long_decimal* decimal,
                                             int shift_amount) {
  for (int i = 0; i < shift_amount; i++) {
    s21_long_decimal copy_decimal_second_multi = *decimal;
    shift_left_long(decimal, 3);  // 0010 10000 + 00100 = 10100
    shift_left_long(&copy_decimal_second_multi, 1);
    add_decimal_without_exponent_sign_long(*decimal, copy_decimal_second_multi,
                                           decimal);
  }
}

void copy_decimal(s21_decimal value_1, s21_decimal* value_2) {
  value_2->bits[0] = value_1.bits[0];
  value_2->bits[1] = value_1.bits[1];
  value_2->bits[2] = value_1.bits[2];
  value_2->bits[3] = value_1.bits[3];
}

void copy_decimal_long(s21_long_decimal value_1, s21_long_decimal* value_2) {
  value_2->bits[0] = value_1.bits[0];
  value_2->bits[1] = value_1.bits[1];
  value_2->bits[2] = value_1.bits[2];
  value_2->bits[3] = value_1.bits[3];
  value_2->bits[4] = value_1.bits[4];
  value_2->bits[5] = value_1.bits[5];
  value_2->bits[6] = value_1.bits[6];
}

int add_decimal_without_exponent_sign(s21_decimal decimal_one,
                                      s21_decimal decimal_second,
                                      s21_decimal* result) {
  s21_decimal zero = {{0, 0, 0, 0}};
  *result = zero;
  int sum_bits = 0;
  int return_value = 0;
  bool transposition = false;

  for (int i = 0; i < 96 && !return_value; i++) {
    sum_bits = check_bit(decimal_one.bits[i / 32], i % 32) +
               check_bit(decimal_second.bits[i / 32], i % 32);

    switch (sum_bits) {
      case 2:
        if (transposition == true) {
          set_bit(&result->bits[i / 32], i % 32, 1);
          transposition = true;
        } else {
          set_bit(&result->bits[i / 32], i % 32, 0);
          transposition = true;
        }
        break;
      case 1:
        if (transposition == true) {
          set_bit(&result->bits[i / 32], i % 32, 0);
          transposition = true;
        } else {
          set_bit(&result->bits[i / 32], i % 32, 1);
          transposition = false;
        }
        break;
      case 0:
        if (transposition == true) {
          set_bit(&result->bits[i / 32], i % 32, 1);
          transposition = false;
        } else {
          set_bit(&result->bits[i / 32], i % 32, 0);
        }
        break;
    }
    return_value = (i == 96 - 1) && transposition;
  }

  *result = return_value ? zero : *result;

  return return_value;
}

int add_decimal_without_exponent_sign_long(s21_long_decimal decimal_one,
                                           s21_long_decimal decimal_second,
                                           s21_long_decimal* result) {
  s21_long_decimal zero = {{0, 0, 0, 0, 0, 0, 0}};
  *result = zero;
  int sum_bits = 0;
  bool transposition = false;
  int return_value = 0;

  for (int i = 0; i <= 223 && !return_value; i++) {
    sum_bits = check_bit(decimal_one.bits[i / 32], i % 32) +
               check_bit(decimal_second.bits[i / 32], i % 32);

    switch (sum_bits) {
      case 2:
        if (transposition == true) {
          set_bit(&result->bits[i / 32], i % 32, 1);
          transposition = true;
        } else {
          set_bit(&result->bits[i / 32], i % 32, 0);
          transposition = true;
        }
        break;
      case 1:
        if (transposition == true) {
          set_bit(&result->bits[i / 32], i % 32, 0);
          transposition = true;
        } else {
          set_bit(&result->bits[i / 32], i % 32, 1);
          transposition = false;
        }
        break;
      case 0:
        if (transposition == true) {
          set_bit(&result->bits[i / 32], i % 32, 1);
          transposition = false;
        } else {
          set_bit(&result->bits[i / 32], i % 32, 0);
        }
        break;
    }
    return_value = (i == 223) && transposition;
  }
  *result = return_value ? zero : *result;

  return return_value;
}

int greater_decimal(s21_decimal decimal_one, s21_decimal decimal_second) {
  int result = 0, bit_one = 0, bit_second = 0;
  for (int i = 95; i >= 0 && !result; i--) {
    bit_one = check_bit(decimal_one.bits[i / 32], i % 32);
    bit_second = check_bit(decimal_second.bits[i / 32], i % 32);
    if (bit_one > bit_second) {
      result = 1;
    } else if (bit_one < bit_second) {
      result = -1;
    }
  }
  return result > 0;
}

int greater_decimal_long(s21_long_decimal decimal_one,
                         s21_long_decimal decimal_second) {
  int result = 0, bit_one = 0, bit_second = 0;
  for (int i = 223; i >= 0 && !result; i--) {
    bit_one = check_bit(decimal_one.bits[i / 32], i % 32);
    bit_second = check_bit(decimal_second.bits[i / 32], i % 32);
    if (bit_one > bit_second) {
      result = 1;
    } else if (bit_one < bit_second) {
      result = -1;
    }
  }
  return result > 0;
}

void sub_decimal_without_exponent_sign(s21_decimal decimal_one,
                                       s21_decimal decimal_second,
                                       s21_decimal* result) {
  int borrow = 0;
  int bit_sub_result = 0;

  for (int i = 0; i < 96; i++) {
    int bit_value_one = check_bit(decimal_one.bits[i / 32], i % 32);
    int bit_value_second = check_bit(decimal_second.bits[i / 32], i % 32);
    bit_sub_result = bit_value_one - bit_value_second - borrow;

    switch (bit_sub_result) {
      case 0:
        set_bit(&result->bits[i / 32], i % 32, 0);
        borrow = 0;
        break;
      case -1:
        set_bit(&result->bits[i / 32], i % 32, 1);
        borrow = 1;
        break;
      case 1:
        set_bit(&result->bits[i / 32], i % 32, 1);
        borrow = 0;
        break;
      case -2:
        set_bit(&result->bits[i / 32], i % 32, 0);
        borrow = 1;
        break;
    }
  }
}

void sub_decimal_without_exponent_sign_long(s21_long_decimal decimal_one,
                                            s21_long_decimal decimal_second,
                                            s21_long_decimal* result) {
  int borrow = 0;
  int bit_sub_result = 0;

  for (int i = 0; i <= 223; i++) {
    int bit_value_one = check_bit(decimal_one.bits[i / 32], i % 32);
    int bit_value_second = check_bit(decimal_second.bits[i / 32], i % 32);
    bit_sub_result = bit_value_one - bit_value_second - borrow;

    switch (bit_sub_result) {
      case 0:
        set_bit(&result->bits[i / 32], i % 32, 0);
        borrow = 0;
        break;
      case -1:
        set_bit(&result->bits[i / 32], i % 32, 1);
        borrow = 1;
        break;
      case 1:
        set_bit(&result->bits[i / 32], i % 32, 1);
        borrow = 0;
        break;
      case -2:
        set_bit(&result->bits[i / 32], i % 32, 0);
        borrow = 1;
        break;
    }
  }
}

void division_decimal_by_ten(s21_decimal* value, int shift_amount) {
  s21_decimal zero = {{0, 0, 0, 0}};
  s21_decimal ones = {{1, 0, 0, 0}};
  s21_decimal tens = {{10, 0, 0, 0}};
  unsigned int bit = value->bits[3];
  for (int i = 0; i < shift_amount; i++) {
    s21_decimal temp = zero, copy_value = *value;  // value = 70
    while (greater_decimal(copy_value, tens) ||
           s21_is_equal(copy_value, tens)) {
      s21_decimal copy_tens1 = tens, copy_tens2 = tens, copy_ones = ones;
      int count_sub = 0;  // счетчик для подсчета колличества вычитаний
      while (greater_decimal(copy_value, copy_tens2)) {  // пока 70 больше 10
        multiple_decimal_with_shift_by_ten(&copy_tens2, 1);  //
        if (greater_decimal(copy_value, copy_tens2)) {
          copy_decimal(copy_tens2, &copy_tens1);
          count_sub++;
        }
      }
      multiple_decimal_with_shift_by_ten(&copy_ones, count_sub);  // 0
      sub_decimal_without_exponent_sign(copy_value, copy_tens1,
                                        &copy_value);  //  60
      add_decimal_without_exponent_sign(temp, copy_ones, &temp);
    }
    copy_decimal(temp, value);
  }
  value->bits[3] = bit;
}

void division_decimal_by_ten_long(s21_long_decimal* value, int shift_amount) {
  s21_long_decimal zero = {{0, 0, 0, 0, 0, 0, 0}};
  s21_long_decimal ones = {{1, 0, 0, 0, 0, 0, 0}};
  s21_long_decimal tens = {{10, 0, 0, 0, 0, 0, 0}};
  unsigned int bit = value->bits[6];
  for (int i = 0; i < shift_amount; i++) {
    s21_long_decimal temp = zero, copy_value = *value;  // value = 70
    while (greater_decimal_long(copy_value, tens) ||
           s21_is_equal_long(copy_value, tens)) {
      s21_long_decimal copy_tens1 = tens, copy_tens2 = tens, copy_ones = ones;
      int count_sub = 0;  // счетчик для подсчета колличества вычитаний
      while (
          greater_decimal_long(copy_value, copy_tens2)) {  // пока 70 больше 10
        multiple_decimal_with_shift_by_ten_long(&copy_tens2, 1);  //
        if (greater_decimal_long(copy_value, copy_tens2)) {
          copy_decimal_long(copy_tens2, &copy_tens1);
          count_sub++;
        }
      }
      multiple_decimal_with_shift_by_ten_long(&copy_ones, count_sub);  // 0
      sub_decimal_without_exponent_sign_long(copy_value, copy_tens1,
                                             &copy_value);  //  60
      add_decimal_without_exponent_sign_long(temp, copy_ones, &temp);
    }
    copy_decimal_long(temp, value);
  }
  value->bits[6] = bit;
}

int div_decimal(s21_decimal decimal, s21_decimal decimal_sec,
                s21_decimal* result) {
  s21_decimal zero = {{0, 0, 0, 0}};
  s21_long_decimal zerol = {{0, 0, 0, 0, 0, 0, 0}};
  s21_long_decimal onesl = {{1, 0, 0, 0, 0, 0, 0}};

  int exp_one = get_exponent(&decimal), exp_two = get_exponent(&decimal_sec);
  if (exp_one > 28) less_decimal(&decimal, exp_one, 28), exp_one = 28;
  if (exp_two > 28) less_decimal(&decimal_sec, exp_two, 28), exp_two = 28;
  int exp = 0, count = 0;

  int sign_1 = get_sign(&decimal);
  int sign_2 = get_sign(&decimal_sec);

  int flag_over = 0, res = 0;
  decimal.bits[3] = 0;
  decimal_sec.bits[3] = 0;

  s21_long_decimal a = zerol, b = zerol;
  copy_decimal_to_long_decimal(&decimal, &a, 0);
  copy_decimal_to_long_decimal(&decimal_sec, &b, 0);

  casting_equal_exponent_long_decimal(&a, &b, exp_one, exp_two);

  unsigned int shift_count = 0;
  s21_long_decimal temp_1 = zerol, temp_2 = zerol;
  s21_long_decimal on = onesl, copy_a = a;

  division_decimal_by_ten_long(&copy_a, 1);

  while (greater_decimal_long(copy_a, b) || s21_is_equal_long(copy_a, b)) {
    if (greater_decimal_long(a, b) || s21_is_equal_long(copy_a, b)) {
      multiple_decimal_with_shift_by_ten_long(&b, 1);
      count++;
    }
  }
  multiple_decimal_with_shift_by_ten_long(&on, count);
  while (exp < 29 && !flag_over && !res) {
    if (greater_decimal_long(a, b) || s21_is_equal_long(a, b)) {
      if (shift_count) {
        exp += shift_count;
        multiple_decimal_with_shift_by_ten_long(&temp_2, shift_count);
        shift_count = 0;
      }
      sub_decimal_without_exponent_sign_long(a, b, &a);
      add_decimal_without_exponent_sign_long(temp_1, on, &temp_1);
    } else if (!greater_decimal_long(a, b) && !decimal_are_zero_long(a)) {
      while (count && greater_decimal_long(b, a)) {
        division_decimal_by_ten_long(&b, 1);
        division_decimal_by_ten_long(&on, 1);
        count--;
      }
      if (!decimal_are_zero_long(temp_1)) {
        add_decimal_without_exponent_sign_long(temp_2, temp_1, &temp_2);
        temp_1 = zerol;
      } else if (!count) {
        multiple_decimal_with_shift_by_ten_long(&a, 1);
        shift_count++;
      }
    } else if (decimal_are_zero_long(a)) {
      add_decimal_without_exponent_sign_long(temp_2, temp_1, &temp_2);
      flag_over = 1;
    }
    res = !res && temp_2.bits[3] > 0 && !exp;
  }
  int check = 0;
  if (exp > 28) {
    less_long_decimal(&temp_2, exp, 28);
    exp = 28;
    check = decimal_are_zero_long(temp_2);
  }
  while (!check_long_decimal_equal_not_zero(temp_2) && exp && !check && !res) {
    less_long_decimal(&temp_2, exp, exp - 1);
    exp--;
  }
  if (check) {
    *result = zero;
  } else if (!res && check_long_decimal_equal_not_zero(temp_2)) {
    copy_decimal_to_long_decimal(result, &temp_2, 1);
    result->bits[3] = exp << 16;
    set_bit(&result->bits[3], 31, sign_1 && sign_2 ? 0 : sign_1 || sign_2);
  } else {
    res += sign_1;
  }
  return res;
}

void copy_decimal_to_long_decimal(s21_decimal* value_1,
                                  s21_long_decimal* value_2, int where) {
  if (where == 1) {
    value_1->bits[0] = value_2->bits[0];
    value_1->bits[1] = value_2->bits[1];
    value_1->bits[2] = value_2->bits[2];
    value_1->bits[3] = value_2->bits[3];
  } else {
    value_2->bits[0] = value_1->bits[0];
    value_2->bits[1] = value_1->bits[1];
    value_2->bits[2] = value_1->bits[2];
    value_2->bits[3] = value_1->bits[3];
  }
}

void clear_exp(s21_decimal* value) { value->bits[3] &= 0x80000000; }

int check_long_decimal_equal_not_zero(s21_long_decimal value) {
  return !value.bits[3] && !value.bits[4] && !value.bits[5] && !value.bits[6];
}

void less_decimal(s21_decimal* value, int exp, int needed) {
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal copy_value_one = *value;           // copy1_value == 123.55
  division_decimal_by_ten(value, exp - needed);  // value == 12.355
  s21_decimal copy_value_second = *value;        // copy2_value == 12.355
  multiple_decimal_with_shift_by_ten(&copy_value_second,
                                     exp - needed);  // copy2_value == 123.55
  sub_decimal_without_exponent_sign(copy_value_one, copy_value_second,
                                    &copy_value_second);
  division_decimal_by_ten(&copy_value_one, exp - needed);
  copy_value_one.bits[0] > 4
      ? add_decimal_without_exponent_sign(*value, one, value)
      : 0;
}

void less_long_decimal(s21_long_decimal* value, int exp, int needed) {
  s21_long_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
  s21_long_decimal copy_value_one = *value;           // copy1_value == 123.55
  division_decimal_by_ten_long(value, exp - needed);  // value == 12.355
  s21_long_decimal copy_value_second = *value;        // copy2_value == 12.355
  multiple_decimal_with_shift_by_ten_long(
      &copy_value_second, exp - needed);  // copy2_value == 123.55
  sub_decimal_without_exponent_sign_long(copy_value_one, copy_value_second,
                                         &copy_value_second);
  division_decimal_by_ten_long(&copy_value_one, exp - needed);
  copy_value_one.bits[0] > 4
      ? add_decimal_without_exponent_sign_long(*value, one, value)
      : 0;
}

int s21_is_equal_long(s21_long_decimal value_1, s21_long_decimal value_2) {
  int result = 1;

  bool sign = get_sign_long(&value_1);
  bool sign_second = get_sign_long(&value_2);
  bool exponent = get_exponent_long(&value_1);
  bool exponent_second = get_exponent_long(&value_2);

  if (sign != sign_second) result = 0;
  if (exponent != exponent_second) result = 0;

  for (int i = 191; i >= 0 && result; i--) {
    result = equal_bit(value_1.bits[i / 32], value_2.bits[i / 32], i % 32);
  }

  return result;  // ok
}

bool decimal_are_zero(s21_decimal value) {
  bool result = 0;
  for (int i = 0; i < 3; i++) {
    if (value.bits[i]) result = 1;
  }
  return !result;
}

bool decimal_are_zero_long(s21_long_decimal value) {
  bool result = 0;
  for (int i = 0; i < 6; i++) {
    result += value.bits[i];
  }
  return !result;
}

int check_overflow_with_multi(s21_decimal decimal, s21_decimal decimal_second) {
  int res = 0;

  for (int i = 95; i >= 0; i--) {
    if (check_bit(decimal.bits[i / 32], i % 32)) {
      res += check_bit(decimal.bits[i / 32], i % 32);
    }
    if (check_bit(decimal_second.bits[i / 32], i % 32)) {
      res += check_bit(decimal_second.bits[i / 32], i % 32);
    }
  }

  res = res >= 95 ? 1 : 0;

  return res;
}

int mul_decimal_without_exponent_sign_long(s21_long_decimal decimal_long_one,
                                           s21_long_decimal decimal_long_second,
                                           s21_long_decimal* result) {
  s21_long_decimal zero = {{0, 0, 0, 0, 0, 0, 0}};

  *result = zero;

  bool two_decimal_are_zero = decimal_are_zero_long(decimal_long_one) ||
                              decimal_are_zero_long(decimal_long_second);
  if (two_decimal_are_zero) return EXIT_SUCCESS;

  int result_return = 0;

  int bits_in_mantissa_long = 192;
  int bits_in_place_number = 32;

  s21_long_decimal tmp = zero;

  for (int i = 0; i < bits_in_mantissa_long; i++) {
    int decimal_long_bit = decimal_long_one.bits[i / bits_in_place_number];

    if (!check_bit(decimal_long_bit, i % bits_in_place_number)) {
      continue;
    }

    s21_long_decimal copy_decomal_second_long = zero;
    copy_decimal_long(decimal_long_second, &copy_decomal_second_long);
    shift_left_long(&copy_decomal_second_long, i);
    add_decimal_without_exponent_sign_long(tmp, copy_decomal_second_long, &tmp);
  }

  copy_decimal_long(tmp, result);

  return result_return;
}

int mul_decimal_without_exponent_sign(s21_decimal decimal_one,
                                      s21_decimal decimal_second,
                                      s21_decimal* result) {
  s21_decimal zero = {{0, 0, 0, 0}};

  *result = zero;

  bool two_decimal_are_zero =
      decimal_are_zero(decimal_one) || decimal_are_zero(decimal_second);
  if (two_decimal_are_zero) return EXIT_SUCCESS;

  int result_return = 0;

  int bits_in_mantissa_long = 96;
  int bits_in_place_number = 32;

  s21_decimal tmp = zero;

  for (int i = 0; i < bits_in_mantissa_long; i++) {
    int decimal_long_bit = decimal_one.bits[i / bits_in_place_number];

    if (!check_bit(decimal_long_bit, i % bits_in_place_number)) {
      continue;
    }

    s21_decimal copy_decomal_second_long = {{0, 0, 0, 0}};
    copy_decimal(decimal_second, &copy_decomal_second_long);
    shift_left(&copy_decomal_second_long, i);
    add_decimal_without_exponent_sign(tmp, copy_decomal_second_long, &tmp);
  }

  copy_decimal(tmp, result);

  return result_return;
}