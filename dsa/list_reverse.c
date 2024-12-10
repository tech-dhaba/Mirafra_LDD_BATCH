#include<stdio.h>
#include<stdlib.h>
typedef struct student
{
	int data;
	struct student *next;
}node;
node *head=NULL;
void insert()
{
	int data;
	node *new=(node *)malloc(sizeof(node));
	if(new==NULL)
	{
		printf("cant allocate the memory\n");
		return;
	}
	printf("Enter the data\n");
	scanf("%d",&new->data);
	if(head==NULL)
	{
		head=new;
		new->next=NULL;
	}
	else
	{
		if(head->data > new->data)
		{
			new->next=head;
			head=new;
		}
		else
		{
			node *temp=head;
			while(temp->next!=NULL && temp->data < new->data)
			{
				temp=temp->next;
			}
			new->next=temp->next;
			temp->next=new;
		}
	}
}
void display()
{
	node *temp=head;
	while(temp!=NULL)
	{
		printf("the data is :%d\n",temp->data);
		temp=temp->next;
	}
}
void rotate()
{
	node *slow=head,*fast=head;
	node *buf;
	while(fast!=NULL && fast->next!=NULL)
	{
		buf=slow;
		slow=slow->next;
		fast=fast->next->next;
	}
	fast->next=head;
	buf->next=NULL;
	head=slow;
}
int main()
{
	int n;
	while(1)
	{
		printf("Enter 1 for insert\n");
		printf("Enter 2 for dispaly \n");
		printf("Enter 3 for reverse \n");
		scanf("%d",&n);
		switch(n)
		{
			case 1:insert();
			       break;
			case 2 :display();
				break;
			case 3:rotate();
			       break;
			default:printf("invalid test case \n");
				break;
		}
	}
}


