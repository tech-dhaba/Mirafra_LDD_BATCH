#include <stdio.h>
#include <stdlib.h>
#include<string.h>
struct operation 
{
    int (*func_ptr)(int, int);
};
int add(int, int);
int sub(int, int);
int mul(int, int);
int divi(int, int);
int main(int argc, char *argv[]) 
{
    if (argc != 4) 
	{
        printf("%s", argv[0]);
        return 1;
    }
    char ch = argv[1][0]; 
    int a = atoi(argv[2]);
    int b = atoi(argv[3]);
    struct operation op;
	  if((strcmp(argv[1],"add"))==0)
	  {
            op.func_ptr = add;
	  }
	else if((strcmp(argv[1],"sub"))==0)
	{

            op.func_ptr = sub;
	}
	  else if((strcmp(argv[1],"mul"))==0)
	  {
            op.func_ptr = mul;
	  }
       //     break;
        case '/':
            if (b == 0) {
                printf("erroe\n");
                return 1;
            }
            op.func_ptr = divi;
            break;
        default:
            printf("invalid");
            return 1;
 //}

    int result = op.func_ptr(a, b);
    printf("result:%d\n", result);
	return 0;
}	
int add(int a, int b) 
{
    return a + b;
}
int sub(int a, int b) 
{
    return a - b;
}
int mul(int a, int b) 
{
    return a * b;
}
int divi(int a, int b) 
{
    return a/b;
}
