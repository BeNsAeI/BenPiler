all:
	make compile
#	make run
	make debug
compile:
	g++ -std=c++0x main.cpp scanner.cpp -o out.out
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
