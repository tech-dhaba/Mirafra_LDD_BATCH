#include<stdio.h>
#include<stdlib.h>
int selection(int arr[],int n,int data)
{
	for(int i=0;i<n;i++)
	{
		if(arr[i]==data)
		{
			return i;
		}
	}
}
int main()
{
	int n;
	//int data=10;
	printf("Enter no of array values\n");
	scanf("%d",&n);
	int arr[n];
	printf("Enter the array elements\n");
	for(int i=0;i<n;i++)
	{
		scanf("%d",&arr[i]);
	}
	int k=selection(arr,n,10);
	printf("The elelments is found at %d\n",k);

}
