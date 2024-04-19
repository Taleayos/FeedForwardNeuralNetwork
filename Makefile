FLAGS = -Wall -Werror -Wextra -std=c++17

all: install tests


install:
	cd View && qmake && make && make clean
	cp -r View/MLP.app  ~/Desktop

uninstall:
	rm -rf View/MLP.app 
	rm -rf ~/Desktop/MLP.app
	rm -rf View/Makefile

clean:
	rm -rf *.o *.a *.gcno  *.gcda *.gcov *.css report html latex *.dSYM report *.info *.gz
	rm -rf test

dvi:
	
	open dvi.html

dist: install
	mkdir archiv
	cp -r dvi.html Emnist/ letters/ Model/ View/ Controller/ Tests/ Research.md weights82 Makefile  archiv
	tar -cvzf MLP.tar.gz archiv
	rm -rf archiv

tests: clean
	g++ $(FLAGS) Tests/test.cpp Model/model.cpp Model/matrix.cpp Model/graph.cc -lgtest -o test -lstdc++
	./test

	
gcov_report: clean
	g++ $(FLAGS) --coverage Tests/test.cpp Model/model.cpp Model/matrix.cpp Model/graph.cc -o test -lgtest 
	./test
	lcov -t "test" -o test.info -c -d . --no-external
	genhtml -o report test.info
	open report/index.html

style:
	clang-format -n Model/*.cpp Model/*.h Model/*.cc Controller/*.h View/*.cpp View/*.h

leaks:
	leaks -atExit -- ./test