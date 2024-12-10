/*Implement a sorting function that takes an array of integers and a comparison function pointer to allow sorting in ascending or descending order.Implement a sorting function that takes an array of integers and a comparison function pointer to allow sorting in ascending or descending order*/

#include<stdlib.h>
#include<stdio.h>
int assending(int a,int b);
int decending(int a,int b);
void sorting(int arr[],int n,int (*fp)(int,int));
int main()
{
	int i,n;
	printf("enter the size:");
	scanf("%d",&n);
	int arr[n];
	printf("enter the array elements:");
	for(i=0;i<n;i++)
	{
		scanf("%d",&arr[i]);
	}
	int (*fp)(int,int);
	switch(ch)
	{

	int *a=sorting(arr,n,assending);
	for(i=0;i<n;i++)
	{
		printf("%d",arr[i]);
	}
	printf("\n");
	sorting(arr,n,decending);
    for(i=0;i<n;i++)
 {
	 printf("%d",arr[i]);
 }
	return 0;
}

int *sorting(int arr[],int n,int (*fp)(int,int))
{
	int i,j,temp;
	for(i=0;i<n-1;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(fp(arr[i],arr[j])>0)
			{
				temp=arr[i];
				arr[i]=arr[j];
				arr[j]=temp;
			}
		}
	}
	return arr;
}

int assending(int a,int b)
{
	return a-b;
}

int decending(int a,int b)
{
	return b-a;
}
