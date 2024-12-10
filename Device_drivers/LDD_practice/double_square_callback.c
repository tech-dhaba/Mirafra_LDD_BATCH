/*Write a program that reads an array of integers and applies a callback function to each element to transform the values (e.g., doubling, squaring).*/

#include<stdlib.h>
#include<stdio.h>
int doubling(int a);
int squaring(int a);
void elements(int arr[],int n,int (*fp)(int));
int main()
{
	int n,i;
	printf("Enter the number");
	scanf("%d",&n);
	int arr[n];
	printf("Enter the elements");
	for(i=0;i<n;i++)
	{
	scanf("%d",&arr[i]);
	}
    elements(arr,n,doubling);
	for(i=0;i<n;i++)
	{
		printf("%d",arr[i]);
	}
	printf("\n");
	elements(arr,n,squaring);
	for(i=0;i<n;i++)
	{
		printf("%d",arr[i]);
	}
	return 0;
}

void elements(int arr[],int n,int (*fp)(int))
{
	int i;
	for(i=0;i<n;i++)
	{
	    arr[i]=(fp)(arr[i]);
	}
}
int doubling(int a)
{
	return a+a;
}

int squaring(int a)
{
	return a*a;
}

