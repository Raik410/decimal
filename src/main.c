#include "lib/helpers.h"
#include "s21_decimal.h"

// int main() {
//   s21_decimal a = {{INT32_MAX - 1, 0, 0, 0}};
//   s21_decimal b = {{1000000, 0, 0, 0}};

//   s21_decimal res = {{0, 0, 0, 0}};

//   int bits_in_mantis = 96;
//   int bits_in_place_number = 32;

//   //    printf("Decimal a: ");
//   //    print_decimal(a);
//   //    printf("Decimal b: ");
//   //    print_decimal(b);
//   //    printf("Decimal res: ");
//   //    print_decimal(res);
//   //    printf("\n\n");

//   for (int i = 0; i < bits_in_mantis; i++) {
//     int value = a.bits[i / bits_in_place_number];

//     if (!check_bit(value, i % bits_in_place_number)) {
//       continue;
//     }

//     s21_decimal copy_b = {{0, 0, 0, 0}};
//     copy_decimal(b, &copy_b);
//     shift_left(&copy_b, i);
//     add_decimal_without_exponent_sign(res, copy_b, &res);

//     //        printf("Decimal b: ");
//     //        print_decimal(b);
//     //        printf("Decimal res: ");
//     //        print_decimal(res);
//   }

//   print_decimal(res);
//   return 0;
// }

// // int main() {
// //   // s21_long_decimal decimal1 = {{10, 0, 0, 0, 0, 0, 0}};
// //   // s21_long_decimal decimal2 = {{7, 0, 0, 0, 0, 0, 0}};
// //   s21_decimal decimal1 = {{2121, 0, 0, 0}};
// //   s21_decimal decimal2 = {{10, 0, 0, 0}};
// //   s21_decimal res = {{0, 0, 0, 0}};
// //   div_decimal_without_exponent_sign(decimal1, decimal2, &res);
// //   print_decimal(res);
// //   return 0;
// // }

// void delim(int a, int b) {
//   int S = 0;
//   int i = 0;
//   int exp = 0;
//   int tmp = 0;
//   while (a >= b) {
//     int copy_b = b;
//     i = 0;
//     while ((copy_b << 1) <= a) {
//       copy_b <<= 1;
//       i++;
//     }

//     S += (1 << i);

//     a -= copy_b;

//     printf("i is %d\n", i);
//     printf("S is %d\n", S);
//     printf("a is %d\n", a);
//   }

//   while (a) {
//     exp++;
//     a *= 10;            // 10
//     S *= 10;            // 20
//     tmp = a / b;        // 5
//     S += tmp;           // 25
//     a = a - (tmp * b);  //
//   }

//   printf("S is %d\n", S);
//   printf("a is %d\n", a);
//   printf("exp is %d\n", exp);
// }

int main() {
  s21_decimal decimal = {{150, 0, 0, 0}};
  print_decimal(decimal);
  less_decimal(&decimal, 2, 1);
  print_decimal(decimal);
  return 0;
}
