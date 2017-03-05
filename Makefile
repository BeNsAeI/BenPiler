all:
	make compile
#	make run
	make debug
compile:
	g++ -std=c++0x main.cpp scanner.cpp parser.cpp -o out.out
FME:
	g++ -Wall -std=c++0x main.cpp scanner.cpp parser.cpp -o out.out
	make debug
run:
	clear
	./out.out file.cm
debug:
	clear
	./out.out file.cm debug
valG:
	valgrind -v ./out.out test.txt debug
clean:
	rm -rf *.out
