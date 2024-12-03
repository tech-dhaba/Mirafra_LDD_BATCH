#include <stdio.h>
int fib( int n);
int power( int n);
int fact( int n);
struct fun_ptr
{
	int (*fp) (int);
};
int main()
{
	int n,i,res;
	struct fun_ptr fp[]={fib,power,fact};
	printf("1-fibnocci\n2-power\n3-factorial\n");
	scanf("%d",&i);
	printf("enter a number:");
	scanf("%d",&n);
	switch(i)
	{
		case 1:res=fp[0].fp(n);
		printf("fibnocci of %dth number =%d\n",n,res);
		break;
		case 2:res=fp[1].fp(n);
		break;
		case 3:res=fp[2].fp(n);
		break;
	}
}
int fib( int n)
{
	if (n==1||n==0)
		return 0;
	else if(n==2)
		return 1;
	else 
		return fib(n-1)+fib(n-2);
}
int power(int n)
{
}
int fact(int n)
{
}
