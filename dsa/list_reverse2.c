#include<stdio.h>
#include<stdlib.h>
typedef struct student
{
	int data;
	struct student *next;
}node;
// if we pass double pointer as parameter we need to call the function by using the pointer refernce
int *insert(node **head)
{
	int data;
	node *new=(node *)malloc(sizeof(node));
	if(new==NULL)
	{
		printf("cant allocate the memory\n");
		//return *head;
	}
	printf("Enter the data\n");
	scanf("%d",&new->data);
	new->next=NULL;
	if((*head)==NULL)
	{
		*head=new;
	}
	else
	{
		if((*head)->data > new->data)
		{
			new->next=*head;
			*head=new;
		}
		else
		{
			node *temp=*head;
			while(temp->next!=NULL && temp->next->data < new->data)
			{
				temp=temp->next;
			}
			new->next=temp->next;
			temp->next=new;
		}
	
	}
	
}
void display(node **head)
{
	node *temp=*head;
	if((*head)==NULL)
	{
		printf("list is empty\n");
		return;
	}
	while(temp!=NULL)
	{
		printf("the data is :%d\n",temp->data);
		temp=temp->next;
	}
}
// passing double pointer as a argument
// as formal parameter
int *rotate(node **head)
{
	if((*head)==NULL || (*head)->next==NULL)
	{
		return *head;
	}
	node *slow=*head,*fast=*head;
	node *buf=NULL;
	while(fast!=NULL && fast->next!=NULL)
	{
		buf=slow;
		slow=slow->next;
		fast=fast->next->next;
	}
	if(fast)
		fast->next=*head;
	if(buf)
		buf->next=NULL;
	*head=slow;
}
int main()
{
//assing single pointer to the head
node *head=NULL;
int n;
	while(1)
	{
		printf("Enter 1 for insert\n");
		printf("Enter 2 for dispaly \n");
		printf("Enter 3 for reverse \n");
		scanf("%d",&n);
		switch(n)
		{
			// we pass double pointer we need to pass address as parameter
			case 1:insert(&head);
			       break;
			case 2 :display(&head);
				break;
			case 3:rotate(&head);
			       break;
			default:printf("invalid test case \n");
				break;
		}
	}
}


