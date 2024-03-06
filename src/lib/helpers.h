#ifndef HELPERS
#define HELPERS
#include "../s21_decimal.h"

int check_bit(const int value, const int index);

void set_bit(unsigned int* value, const int index, const int bit);

void print_decimal(s21_decimal value);

int get_sign(const s21_decimal* value);

void set_sign(s21_decimal* value, int sign);

unsigned int get_exponent(const s21_decimal* value);

void set_exponent(s21_decimal* value, unsigned int exponent);

int are_bits_equal(const s21_decimal* dec1, const s21_decimal* dec2,
                   int bit_index);

int are_cells_equal(const s21_decimal* dec1, const s21_decimal* dec2,
                    int cell_index);

void set_mantissa(s21_decimal* dst, unsigned int src);

unsigned int get_mantissa_decimal_for_int(s21_decimal src);

unsigned int get_exponent_from_float(float src);

void float_exp(unsigned int* scale_amount, s21_decimal* dst, float src);

void shift_left(s21_decimal* value, const int shift);

void casting_equal_exponent(s21_decimal* decimal_one,
                            s21_decimal* decimal_second, unsigned int exp_one,
                            unsigned int exp_second);

void multiple_decimal_with_shift_by_ten(s21_decimal* decimal, int shift_amount);

int add_decimal_without_exponent_sign(s21_decimal decimal_one,
                                      s21_decimal decimal_second,
                                      s21_decimal* result);

int greater_decimal(s21_decimal decimal_one, s21_decimal decimal_second);
// unsigned int get_overflow_bits(unsigned int value, int shift);

void division_decimal_by_ten(s21_decimal* decimal, int division_count);

void sub_decimal_without_exponent_sign(s21_decimal decimal_one,
                                       s21_decimal decimal_second,
                                       s21_decimal* result);

void copy_decimal(s21_decimal value_1, s21_decimal* value_2);

void clear_exp(s21_decimal* value);

int equal_bit(unsigned int value_1, unsigned int value_2, int index);

void s21_div_without_fraction(s21_decimal decimal_one,
                              s21_decimal decimal_second, s21_decimal* result);

void copy_decimal_to_long_decimal(s21_decimal* value_1,
                                  s21_long_decimal* value_2, int where);

void shift_left_long(s21_long_decimal* value, int shift);

void casting_equal_exponent_long_decimal(s21_long_decimal* decimal_one,
                                         s21_long_decimal* decimal_second,
                                         unsigned int exp_one,
                                         unsigned int exp_second);

void multiple_decimal_with_shift_by_ten_long(s21_long_decimal* decimal,
                                             int shift_amount);

int add_decimal_without_exponent_sign_long(s21_long_decimal decimal_one,
                                           s21_long_decimal decimal_second,
                                           s21_long_decimal* result);

int greater_decimal_long(s21_long_decimal decimal_one,
                         s21_long_decimal decimal_second);

void sub_decimal_without_exponent_sign_long(s21_long_decimal decimal_one,
                                            s21_long_decimal decimal_second,
                                            s21_long_decimal* result);

int check_long_decimal_equal_not_zero(s21_long_decimal value);

void less_long_decimal(s21_long_decimal* value, int exp, int needed);

int get_sign_long(const s21_long_decimal* value);

void set_sign_long(s21_long_decimal* value, int sign);

unsigned int get_exponent_long(const s21_long_decimal* value);

void set_exponent_long(s21_long_decimal* value, unsigned int exponent);

void copy_decimal_long(s21_long_decimal value_1, s21_long_decimal* value_2);

int s21_is_equal_long(s21_long_decimal value_1, s21_long_decimal value_2);

void division_decimal_by_ten_long(s21_long_decimal* value, int shift_amount);

void less_decimal(s21_decimal* value, int exp, int needed);

bool decimal_are_zero(s21_decimal value);

bool decimal_are_zero_long(s21_long_decimal value);

int mul_decimal_without_exponent_sign_long(s21_long_decimal decimal_long_one,
                                           s21_long_decimal decimal_long_second,
                                           s21_long_decimal* result);

void print_decimal_long(s21_long_decimal value);

int s21_mul(s21_decimal decimal_one, s21_decimal decimal_second,
            s21_decimal* result);

int s21_sub(s21_decimal decimal_one, s21_decimal decimal_second,
            s21_decimal* result);

s21_decimal div_decimal_without_exponent_sign_fraction(
    s21_decimal decimal_one, s21_decimal decimal_second, s21_decimal* result);

int mul_decimal_without_exponent_sign(s21_decimal decimal_one,
                                      s21_decimal decimal_second,
                                      s21_decimal* result);

void shift_right(s21_decimal* value, int shift);

s21_decimal shift_left_output(s21_decimal value, int shift);

int div_decimal_without_exponent_sign(s21_decimal decimal_one,
                                      s21_decimal decimal_second,
                                      s21_decimal* result);

int div_decimal(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int check_overflow_with_multi(s21_decimal decimal, s21_decimal decimal_second);

#endif