.PHONY: gcov_report

GCOVFLAGS = --coverage

gcov_report: clean test.o gcov_lib.a
	mkdir -p gcov_obj
ifeq ("$(OS)", "Linux")
	$(CC) $(CFLAGS) test.o -o gcov_obj/test_gcov -L. gcov_obj/gcov_lib.a $(GCOVFLAGS) -lm $(PKG)
	gcov_obj/test_gcov
	mkdir -p report
	$(GCOV) --html-details report/index.html --html-medium-threshold 80 --html-high-threshold 100 --html-title "Coverage s21_tetris report"
	$(FREE_TEST)
else
	$(CC) $(CFLAGS) test.o -o gcov_obj/test_gcov -L. gcov_obj/gcov_lib.a $(GCOVFLAGS) -lm $(PKG)
	gcov_obj/test_gcov
	mkdir -p report
	lcov -t "Report" -c -d ./gcov_obj --output-file ./report/coverage.info --gcov-tool $(GCOV) --rc branch_coverage=1
	genhtml ./report/coverage.info --output-directory ./report/ --rc branch_coverage=1
endif

gcov_obj/%.o: %.c
	mkdir -p gcov_obj
	$(CC) $(CFLAGS) $(GCOVFLAGS) -c $< -o gcov_obj/$(@F)

gcov_lib.a: $(OBJECTS_GCOV)
	ar rc gcov_obj/$(*F).a $(addprefix gcov_obj/,$(^F))
	ranlib gcov_obj/$(*F).a