all:
	make compile
#	make run
	make debug
compile:
	g++ main.cpp scanner.cpp -o out.out
run:
	clear
	./out.out
debug:
	clear
	./out.out debug
clean:
	rm -rf *.out
