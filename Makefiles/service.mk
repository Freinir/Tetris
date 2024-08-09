.PHONY: clang_correct clang_check check clean rebuild

CLANG_FORMAT = ../materials/linters/.clang-format

clang_correct:
	clang-format --style=file:$(CLANG_FORMAT) -i $(LIB_PATH)*.c $(LIB_PATH)*.h $(GUI_PATH)*.c $(GUI_PATH)*.h

clang_check:
	clang-format --style=file:$(CLANG_FORMAT) -n $(LIB_PATH)*.c $(LIB_PATH)*.h $(GUI_PATH)*.c $(GUI_PATH)*.h

check:
	cppcheck -q --enable=warning,portability --inconclusive $(LIB_PATH) $(GUI_PATH)

clean:
	rm -rf $(LIBS) $(TEST_LIB) *.o *.so *.a *.out *.gcda *.gcno *.info test test.c test_gcov report obj gcov_obj tetris score.data ./dist

rebuild: clean all

