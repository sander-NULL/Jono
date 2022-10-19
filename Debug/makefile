all: Jono perft searchtest compare

Jono: alphabeta.o bitboards.o debug.o main.o movgen.o timemgmt.o
	@echo "*** Building" $@ " ***"
	gcc -pthread -o Jono alphabeta.o bitboards.o debug.o main.o movgen.o timemgmt.o

perft: bitboards.o perft.o movgen.o
	@echo "*** Building" $@ " ***"
	gcc -o perft bitboards.o perft.o movgen.o
	
searchtest:	alphabeta-test.o bitboards.o movgen.o searchtest.o
	@echo "*** Building" $@ " ***"
	gcc -pthread -o searchtest alphabeta-test.o bitboards.o movgen.o searchtest.o
	
compare: compare.o
	@echo "*** Building" $@ " ***"
	gcc -o compare compare.o
	
alphabeta.o: ../alphabeta.c
	@echo "*** Compiling to" $@ " ***"
	gcc -O0 -g3 -Wall -pedantic -pedantic-errors -c ../alphabeta.c -o alphabeta.o

bitboards.o: ../bitboards.c
	@echo "*** Compiling to" $@ " ***"
	gcc -O0 -g3 -Wall -pedantic -pedantic-errors -c ../bitboards.c -o bitboards.o
	
debug.o: ../debug.c
	@echo "*** Compiling to" $@ " ***"
	gcc -O0 -g3 -Wall -pedantic -pedantic-errors -c ../debug.c -o debug.o
	
main.o: ../main.c
	@echo "*** Compiling to" $@ " ***"
	gcc -O0 -g3 -Wall -pedantic -pedantic-errors -c ../main.c -o main.o

movgen.o: ../movgen.c
	@echo "*** Compiling to" $@ " ***"
	gcc -O0 -g3 -Wall -pedantic -pedantic-errors -c ../movgen.c -o movgen.o
	
timemgmt.o: ../timemgmt.c
	@echo "*** Compiling to" $@ " ***"
	gcc -O0 -g3 -Wall -pedantic -pedantic-errors -c ../timemgmt.c -o timemgmt.o
	
perft.o: ../Perft/perft.c
	@echo "*** Compiling to" $@ " ***"
	gcc -O0 -g3 -Wall -pedantic -pedantic-errors -c ../Perft/perft.c -o perft.o

searchtest.o: ../Search\ test/searchtest.c
	@echo "*** Compiling to" $@ " ***"
	gcc -O0 -g3 -Wall -pedantic -pedantic-errors -c ../Search\ test/searchtest.c -o searchtest.o
	
compare.o: ../Search\ test/compare.c
	@echo "*** Compiling to" $@ " ***"
	gcc -O0 -g3 -Wall -pedantic -pedantic-errors -c ../Search\ test/compare.c -o compare.o
	
alphabeta-test.o: ../alphabeta.c		#Compiling the search module in test mode
	@echo "*** Compiling to" $@ " ***"
	gcc -O0 -g3 -Wall -DSEARCHTEST -pedantic -pedantic-errors -c ../alphabeta.c -o alphabeta-test.o