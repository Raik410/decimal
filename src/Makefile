CC = gcc
FLAGS = -Wall -Werror -Wextra
FUNC_FILES_C = lib/*.c
FUNC_FILES_O = *.o
LIBNAME = s21_decimal.a
FUNC_FILES_C_TESTS = tests/*.c
LIBS = -lcheck -lsubunit -lm
GCOV_FLAGS = -fprofile-exclude-files=tests/tests.c -fprofile-arcs -ftest-coverage

all: s21_decimal.a

s21_decimal.a:
	@$(CC) $(FLAGS) -c $(FUNC_FILES_C)
	@ar rcs $(LIBNAME) $(FUNC_FILES_O)
	@rm -rf *.o

main: s21_decimal.a
	@$(CC) $(FLAGS) main.c -L. s21_decimal.a -lm -o main
	@./main
	@rm -f s21_decimal.a
	@rm -f main

test: s21_decimal.a
	@$(CC) $(FLAGS) $(FUNC_FILES_C_TESTS) -L. s21_decimal.a $(LIBS) -o test
	@./test || $(MAKE) clean

clean:
	@rm -rf *.o
	@rm -f s21_decimal.a
	@rm -f main
	@rm -f test
	@rm -f ./a.out *.gcda *.gcno
	@rm -f *.css *.html

clang-check:
	@clang-format -n *.c *.h lib/*.c lib/*.h tests/*.c samples/*.c

clang:
	@clang-format -i *.c *.h lib/*.c lib/*.h tests/*.c samples/*.c

valgrind: test
	valgrind --tool=memcheck --leak-check=yes ./test

cppcheck: 
	cppcheck *.c *.h lib/*.c lib/*.h tests/*.c samples/*.c

gcov_report: test
	gcov lib/*.c
	~/.local/bin/gcovr -r . --html --html-details -o s21_decimal.html

open:
	@echo "OPEN REPORT..."
	xdg-open s21_decimal.html
