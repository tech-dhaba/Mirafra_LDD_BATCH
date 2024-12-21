#include<stdio.h>
#include<string.h>

void reverse(char *p,char *q)
{
	char temp;
	if(p<q)
	{
		temp=*p;
		*p=*q;
		*q=temp;
		p++;q--;
		reverse(p,q);
	}
}
int main()
{
	char str[100];
	printf("Enter the string :");
	scanf("%s",str);
	char *p,*q,temp;
	p=str;
	q=str+strlen(str)-1;
	reverse(p,q);
	printf("\nnew str :%s\n",str);
}
