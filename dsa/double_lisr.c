#include<stdio.h>
#include<stdlib.h>
typedef struct student 
{
	int data;
	struct student *prev,*next;
}node;
int insert(node **head)
{
	int data;
	node *new=(node*) malloc(sizeof(node));
	if(new==NULL)
	{
		printf("can't allocate the memory\n");
		return;
	}
	printf("enter the data \n");
	scanf("%d",&new->data);
	new->next=NULL;
	if(*head==NULL)
	{
		*head=new;
		new->prev=NULL;
	}
	else
	{
		if((*head)->data > new->data)
		{
			new->next=(*head);
			(*head)->prev=new;
			new->prev=NULL;
			(*head)=new;
		}
		else
		{
			node *temp=*head;
			while(temp->next!=NULL && temp->next->data <new->data)
			{
				temp=temp->next;
			}
			new->next=temp->next;
			temp->next=new;
			new->prev=temp;
			if(temp->next!=NULL)
			{
				temp->next->prev=new;
			}
		}
	}
}
void display(node *head)
{
	node *temp=head;
	while(temp!=NULL)
	{
		printf("%d\t ",temp->data);
		temp=temp->next;
	}
}
int delete(node **head)
{
	int data;
	printf("Enter the data \n");
	scanf("%d",&data);
	if(*head==NULL)
	{
		printf("list is empty\n");
		return;
	}
	else
	{
		node *temp=*head;
		if((*head)->data==data)
		{
			*head=(*head)->next;
			(*head)->prev=NULL;
			free(temp);
		}
		else
		{
			node *prev=NULL;
			while(temp->next!=NULL && temp->data !=data)
			{
				prev=temp;
				temp=temp->next;
			}
			prev->next=temp->next;
			if(temp->next!=NULL)
			{
				temp->next->prev=prev;
			}
			free(temp);
		}
	}
}

int main()
{
	node *head=NULL;
	int n;
	int data;
	while(1)
	{
		printf("Enter for insert\n");
		printf("enter 2 for display\n");
		printf("Enter 3 for delete\n");
		scanf("%d",&n);
		switch(n)
		{
			case 1:insert(&head);
			       break;
			case 2:display(head);
			       break;
			case 3:delete(&head);
			       break;
			default:printf("invlaid test case\n");
				break;
		}
	}
}

