CC=gcc
FLAGS =-std=c11
LDFLAGS=-Wall -Wextra -Werror 
LIBFLAGS=`pkg-config --cflags --libs check` -lm
CFILES=test.c parser.c s21_matrix.c
QDIR= 3D_Viewer
DDIR= distr_3D_Viewer
.PHONY: install clean all

all: tests
# clean install
	
# ./$(QDIR)/s21_calc.app/Contents/MacOS/s21_calc
	
tests:
	$(CC) $(LDFLAGS) $(CFLAGS) $(CFILES) -o viewer.o $(LIBFLAGS)
	./viewer.o

clean:
	rm -rf *.o *.out *.cfg *.tar report manual

check:
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --extension=c *.c *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	rm -rf *.o *.out *.cfg

leaks:
	$(CC) $(LDFLAGS) $(CFLAGS) $(CFILES) -o calc.o $(LIBFLAGS)
	./calc.o
	CK_FORK=no leaks --atExit -- ./calc.o
	
valgrind_check:  
	$(CC) -O0 -g $(CFILES) -o $(LIBFLAGS)
	valgrind --leak-check=full --track-origins=yes ./viewer.o -n file
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./viewer.o -n file

install:
	mkdir $(QDIR)
	cd $(QDIR); qmake ../qt/qt/qt.pro
	cd $(QDIR); make
	cd $(QDIR); make clean

uninstall: 
	rm -r $(QDIR)

dist:
	mkdir $(DDIR)
	mkdir $(DDIR)/qt
	cp *.c *.h Makefile $(DDIR)	
	cp -R qt $(DDIR)
	tar --totals -cvf $(DDIR).tar $(DDIR)
	rm -rf $(DDIR)

gcov_report: 
	$(CC) --coverage $(LDFLAGS) $(CFILES) -o viewer.o $(LIBFLAGS)
	./viewer.o
	lcov -o viewer.info -c -d ./ --no-external
	genhtml -o report viewer.info
	open report/index.html
	rm -rf *.gcno *.gcda *.info

dvi:
	makeinfo manual.texi --html
	open ./manual/index.html