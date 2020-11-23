all: baboons
	# ../cppcheck-2.2/cppcheck --enable=warning,performance,portability --error-exitcode=1 -v .

baboons: baboons.o
	# gcc -Werror -Wall -O0 baboons.o -o baboons -lpthread -lrt
	# gcc -Werror -Wall -O3 baboons.o -o baboons -lpthread -lrt
	# gcc -Werror -Wall baboons.o -o baboons -lpthread -lrt
	gcc -Wall baboons.o -o baboons -lpthread -lrt
	# gcc -O0 -Wall -Wextra -Werror -pedantic baboons.o -o baboons -lpthread -lrt
	# gcc -O0 -Wall -Wextra -pedantic baboons.o -o baboons -lpthread -lrt
	

baboons.o: baboons.c
	# gcc -Werror -Wall -O0 -c baboons.c -lpthread -lrt
	# gcc -Werror -Wall -O3 -c baboons.c -lpthread -lrt
	# gcc -Werror -Wall -c baboons.c -lpthread -lrt
	gcc -Wall -c baboons.c -lpthread -lrt
	# gcc -Wall -Wextra -Werror -pedantic -c baboons.c -lpthread -lrt
	# gcc -Wall -Wextra -pedantic -c baboons.c -lpthread -lrt


clean:
	rm -rf *.o

memcheck:
	valgrind ./baboons input.dat 3 -s
