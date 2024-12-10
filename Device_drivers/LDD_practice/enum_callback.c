#include <stdio.h>
#include <stdlib.h>

//creat enum templet
enum abc{Add,Sub,Mul,Div};

//creat structure templet
struct operation 
{
    int (*func_ptr[4])(int, int);//declaration,array of function pointers
};
//function declarations
int add(int, int);
int sub(int, int);
int mul(int, int);
int divi(int, int);

int main() 
{
	char ch;
	int a,b;//declare three variables
    struct operation op={add,sub,mul,divi};//assign the enum members to index's of array of function pointers
	int index;//variable declare
		printf("Enter operator & values:");
		scanf("%c%d%d",&ch,&a,&b);
    switch (ch) 
	{
        case '+':
            index= Add;//enum ADD value is 0
            break;
        case '-':
            index= Sub;//enum Sub value is 1
            break;
        case '*':
            index= Mul;//enum Mul value is 2
            break;
        case '/':
            index= Div;//enum Div value is 3
            break;
        default:
            printf("invalid");
            return 1;
    }
    int result = op.func_ptr[index](a, b);//here function pointer invoke the function based on index number
    printf("result:%d\n", result);
	return 0;
}
int add(int a,int b) 
{
    return a+b;
}
int sub(int a,int b) 
{
    return a-b;
}
int mul(int a,int b) 
{
    return a*b;
}
int divi(int a,int b) 
{
    return a/b;
}
