#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int add(int a, int b);
int sub(int a, int b);
int mul(int a, int b);
int divi(int a, int b);


enum calculator
{
	ADD,SUB,MUL,DIVI,MAX						
};
struct fun_ptr
{
	int (*fp) (int ,int);
};
int fun(int (*fp) (int ,int),int a, int b)
{
	return fp(a,b); //calling back
}
int main(int argc,char *argv[])
{
	//structure array to store addresse of functions
	struct fun_ptr fptr[MAX]={add,sub,mul,divi};

	//converting ascii to integers
	int a=atoi(argv[2]);
	int b=atoi(argv[3]);

	int index;

	//comparing the input 
	if (strcmp(argv[1], "add") == 0) 
	{
		index=ADD;
	}
	else if (strcmp(argv[1], "sub") == 0)
	{
		index=SUB;
	}
	else if (strcmp(argv[1], "mul") == 0)
	{
		index=MUL;
	}
	else if (strcmp(argv[1], "divi") == 0)
	{
		index=DIVI;
	}
	//passing function pointer and input integers
	int res =fun(fptr[index].fp,a,b);
	printf("res = %d",res);
	return 0;
}
int add(int a, int b)
{
	return a+b;
}
int sub(int a, int b)
{
	return a-b;
}
int mul(int a, int b)
{
	return a*b;
}
int divi(int a, int b)
{
	return a/b;
}
