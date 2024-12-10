#include<stdio.h>
#include<stdlib.h>
int (*fp)(int,int);
int add(int,int);
int sub(int,int);
int mul(int,int);
int divi(int,int);
int func(int (*fp)(int,int),int,int);
int main(int argc,char *argv[])
{
	char ch;
    int a,b;
	/*if(argc !=4)
	{
		printf("%s",argv[0]);
				return 1;
	}*/
	ch=*argv[1];
	a=atoi(argv[2]);
	b=atoi(argv[3]);
	switch(ch)
	{
		case '+':
			printf("add:%d\n",func(add,a,b));
			break;
		case '-':
			printf("sub:%d\n",func(sub,a,b));
			break;
		case '*':
			printf("mul:%d",func(mul,a,b));
			break;
		case '/':
		/* if(b==0)
		 {
		   printf("error");
			   return 1;
		 }*/
			printf("div:%d",func(divi,a,b));
		break;
		default:
		 printf("giving valid input");
		break;
	}
	return 0;
}

int func(int (*fp)(int,int),int a,int b)
{
	return fp(a,b);
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

