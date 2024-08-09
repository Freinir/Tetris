.PHONY: test leak_test

FREE_TEST = rm -rf test.c test.o
CFLAGS_TEST = -std=c11 -Wall -Werror -Wextra -pedantic 
CHECK_FILE = ./tests/*.check

test.c:
	checkmk clean_mode=1 $(CHECK_FILE) > test.c	

test.o: test.c
	$(CC) $(CFLAGS_TEST) -g -c test.c	

test: clean test.o $(TEST_LIB_A)
	$(CC) $(CFLAGS) -g test.o -o test -L. obj/$(LIB_NAME).a -lm $(PKG)
	./test
	$(FREE_TEST)

leak_test: clean test.o $(TEST_LIB_A)
	$(CC) $(CFLAGS) -g test.o -o test -L. obj/$(LIB_NAME).a -lm $(PKG)
	$(LEAKS_RUN_TEST)
	$(FREE_TEST)