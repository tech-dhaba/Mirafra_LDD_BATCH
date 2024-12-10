#include<stdio.h>
#include<stdlib.h>
typedef struct student
{
	int data;
	struct student *next;
}node;
void insert(node **head,int data)
{
	node *new=(node *)malloc(sizeof(node));
	if(new==NULL)
	{
		printf("can't allocate the memory\n");
		return;
	}
	new->data=data;
	new->next=NULL;
	if((*head)==NULL)
	{
		*head=new;
	}
	else
	{
		if((*head)->data > data)
		{
			new->next= *head;
			*head=new;
		}
		else
		{
			node *temp=*head;
			while(temp->next!=NULL && temp->data < data)
			{
				temp=temp->next;
			}new->next=temp->next;
			temp->next=new;
		}
	}
}
void display(node *head)
{
	node *temp=head;
	while(temp!=NULL)
	{
		printf("%d\t",temp->data);
		temp=temp->next;
	}
}
void reverse(node **head)
{
	node *slow=*head,*fast=*head;
	while(fast!=NULL && fast->next!=NULL)
	{
		slow=slow->next;
		fast=fast->next->next;
	}
	node *second=slow->next;
	slow->next=NULL;
	node *l1=NULL;
	node *l2=*head;
	node *l3=NULL;
	while(l2!=NULL)
	{
		l3=l2->next;
		l2->next=l1;
		l1=l2;
		l2=l3;
	}
	*head=l1;
	node *temp=*head;
	while(temp->next!=NULL)
	{
		temp=temp->next;
	}
	temp->next=second;

}

int main()
{
	node *head=NULL;
	int n;
	while(1)
	{
		printf("enter 1 for insert \n");
		printf("enter 2 for display \n");
		printf("enter 3 for reverse \n");
		scanf("%d",&n);
		switch(n)
		{
			case 1:int data;
			       printf("Enter the data\n");
			       scanf("%d",&data);
			       insert(&head,data);
			       break;
			case 2:display(head);
			       break;
			case 3:reverse(&head);
			       break;
			default:printf("invalid test case \n");
				break;
		}
	}
}
			       
