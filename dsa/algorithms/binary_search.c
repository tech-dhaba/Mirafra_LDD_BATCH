#include<stdio.h>
#include<stdlib.h>
int binary(int arr[],int n,int data)
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(arr[j] > arr[j+1])
			{
				int temp=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=temp;
			}
		}
	}
	int low=0;
	int high=n-1;
	while(low<=high)
	{
	for(int i=0;i<n;i++)
	{
		int mid=low+(high-low)/2;
		if(arr[mid]==data)
		{
			return mid;
		}
		else if(arr[mid] > data)
		{
			low=mid+1;
		}
		else if(arr[mid] < data)
		{
			high=mid-1;
		}
		else
		{
			printf("data is not found\n");
			exit(0);
		}
		return mid;
	}
	}
}

int main()
{
	int n;
	printf("Enter the n value \n");
	scanf("%d",&n);
	int arr[n];
	printf("Enter the array elements\n");
	for(int i=0;i<n;i++)
	{
		scanf("%d",&arr[i]);
	}
	int k=binary(arr,n,10);
	printf("the element is found at: %d\n",k);
}
