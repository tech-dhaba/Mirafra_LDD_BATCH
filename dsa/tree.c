#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct student
{
	int data;
	struct student *left,*right;
}node;
void insert(node **root)
{
	int data;
	node *new=(node *)malloc(sizeof(node));
	if(new==NULL)
	{
		printf("can't allocate the memory \n");
		exit(1);
	}
	printf("enter the data \n");
	scanf("%d",&new->data);
	new->left=NULL;
	new->right=NULL;
	if((*root)==NULL)
	{
		*root =new;
	}
	else
	{
		node *parent=NULL;
		node *temp=(*root);
		while(temp!=NULL)
		{
			parent=temp;
			if(temp->data > new->data)
			{
				temp=temp->left;
			}
			else
			{
				temp=temp->right;
			}
		}
		if(parent->data > new->data)
		{
			parent->left=new;
		}
		else
		{
			parent->right=new;
		}
	}
}
void inorder(node *root)
{
	if(root == NULL)
	{
		return;
	}
	inorder(root->left);
		printf("%d\t",root->data);
		inorder(root->right);
	
}
node *findnode(node *root, int data, char *position) {
    node *temp = root;
    int level = 1; // Start from level 1
    char path[100] = "Root"; // Initialize path from the root

    while (temp != NULL) {
        if (temp->data == data) {
            sprintf(position, "Level %d (%s)", level, path); // Format position info
            return temp;
        } else if (temp->data > data) {
            strcat(path, " -> Left"); // Append "Left" to the path
            temp = temp->left;
        } else {
            strcat(path, " -> Right"); // Append "Right" to the path
            temp = temp->right;
        }
        level++;
    }
}
node* findMin(node *root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

node* deleteNode(node *root, int data) {
    if (root == NULL) {
        printf("Node not found\n");
        return root;
    }

    // Search for the node to delete
    if (data < root->data) {
        root->left = deleteNode(root->left, data);
    } else if (data > root->data) {
        root->right = deleteNode(root->right, data);
    } else {
        // Node to delete is found
        if (root->left == NULL) {  // Node with only right child or no child
            node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {  // Node with only left child
            node *temp = root->left;
            free(root);
            return temp;
        }

        // Node with two children
        node *temp = findMin(root->right);  // Find in-order successor
        root->data = temp->data;  // Replace data
        root->right = deleteNode(root->right, temp->data);  // Delete the successor
    }
    return root;
}

int main()
{
	node *root=NULL;
	int n;
	while(1)
	{
		printf("enter 1 for insert \n");
		printf("Enter 2 for inoeder display\n");
		printf("enter 3 for finding the data\n");
		printf("enter 4 for delete of a node \n");
		scanf("%d",&n);
		switch(n)
		{
			case 1:insert(&root);
			       break;
			case 2:inorder(root);
			       break;
			case 3:int data;
			       char position[100]="";// to store character positions
			       printf("enter the data\n");
			       scanf("%d",&data);
			       node *result=findnode(root,data,position);
			       if(result)
			       {
				       printf("the data is found %d:\n",result->data);
			       }
			       else
			       {
				       printf("the data is not found \n");
			       }
			       printf("the node is found at the position\n%s:",position);
			       break;
			case 4:printf("Enter the node to delete: ");
			       scanf("%d", &data);
			       root = deleteNode(root, data);
			       break;
			default:printf("invlaid input\n");
				break;
		}
	}
}
	
