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
	int ch,a,b;//declare three variables
    struct operation op={add,sub,mul,divi};//assign the enum members to index's of array of function pointers
	int index;//variable declare
	while(1)
	{
		printf("\nMenu\n");
		printf("1:Addition:\n");
		printf("2:subtraction\n");
		printf("3:multiplication\n");
		printf("4:division\n");
		printf("5:exit\n");
		printf("enter the choice");
		scanf("%d",&ch);
		if(ch==5)
		{
			printf("exit the program");
			break;
		}
		printf("Enter the values");
		scanf("%d%d",&a,&b);
    switch (ch) 
	{
        case 1:
            index= Add;//enum ADD value is 0
            break;
        case 2:
            index= Sub;//enum Sub value is 1
            break;
        case 3:
            index= Mul;//enum Mul value is 2
            break;
        case 4:
            index= Div;//enum Div value is 3
            break;
        default:
            printf("invalid");
            continue;
    }
    int result = op.func_ptr[index](a, b);//here function pointer invoke the function based on index number
    printf("result:%d\n", result);
	}
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
