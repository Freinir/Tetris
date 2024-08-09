.PHONY: all install play dvi dist uninstall

OS = $(shell uname)
ifeq ("$(OS)","Linux")
	CC = gcc
	GCOV = gcovr 
	PKG =`pkg-config --cflags --libs check tinfo`
	LEAKS_RUN_TEST = valgrind --tool=memcheck --leak-check=yes ./test 
else
	CC = gcc-14 
	GCOV = gcov-14
	PKG =`pkg-config --cflags --libs check`
	LEAKS_RUN_TEST = leaks -atExit-- ./test
endif

CFLAGS = -std=c11 -Wall -Werror -Wextra -pedantic #-D_POSIX_C_SOURCE=199309L

EXTRA_LIBS = -lm -lncursesw $(PKG)
LIB_PATH = brick_game/tetris/
LIB_NAME = backend
GUI_PATH = gui/cli/
GUI_NAME = frontend

SRC_LIBS= $(LIB_PATH)$(LIB_NAME).c
LIBS = $(SRC_LIBS:.c=.a)
TEST_LIB = $(LIB_PATH)$(LIB_NAME).o
TEST_LIB_A = $(TEST_LIB:.o=.a)
OBJECTS_GCOV = $(addprefix gcov_obj/,$(TEST_LIB))

all: install play

install: $(LIBS)
	mkdir -p tetris/
	$(CC) $(CFLAGS) $(GUI_PATH)$(GUI_NAME).c $(addprefix obj/,$(^F)) -o tetris/tetris $(EXTRA_LIBS)

play: ./tetris/tetris
	./tetris/tetris

%.a: %.o
	mkdir -p obj	
	ar rc obj/$(@F) obj/$(<F)
	ranlib obj/$(@F)

%.o: %.c
	mkdir -p obj
	$(CC) $(CFLAGS) -g -c $< -o obj/$(@F) -lm -lncursesw

dvi:
	doxygen

dist:	
	tar -czvf brick_game.tar ./
	mkdir -p dist
	mv brick_game.tar dist/

uninstall: clean
	rm -rf dvi
	rm -f score.data
	rm -rf ./dist

include ./Makefiles/gcov_report.mk
include ./Makefiles/test.mk
include ./Makefiles/service.mk