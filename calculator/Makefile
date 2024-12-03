all : calculator

calculator:calculator.o
	gcc calculator.o -o calculator

calculator.o:main.c
	gcc -c main.c -o calculator.o

clean :
	rm *.o

