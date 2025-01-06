

build: my_lib.o main.o
	g++ main.o my_lib.o -o main.exe

main.o:
	g++ main.cc -c

my_lib.o:
	g++ my_lib.cc -c

run:
	./main.exe


clean:
	rm -f *.o
	rm -f *.exe
