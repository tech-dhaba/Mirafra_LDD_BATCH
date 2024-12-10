#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// factorial function
int factorial(int n, int b) {
    if (n == 0) {
        return 1;
    }
    return n * factorial(n - 1, 0);
}

// fibonacci function
int fibonacci(int n, int b) {
    if (n == 0 || n == 1) {
        return n;
    }
    return fibonacci(n-1,0) + fibonacci(n-2,0);
}

// power function
int power(int base, int po) {
    if (po == 0) {
        return 1;
    }
    return base * power(base,po - 1);
}

// Callback mechanism
int fun(int (*fp)(int, int), int a, int b) {
    return fp(a, b);
}

int main() {
    // Array of function pointers
    int (*fp[3])(int, int) = {factorial, fibonacci, power};

    int ch, a, b;
    printf("Choose an operation:\n");
    printf("1: Factorial\n");
    printf("2: Fibonacci\n");
    printf("3: Power\n");
    printf("Enter the choice: ");
    scanf("%d", &ch);
 switch (ch) 
	{

        case 1: 
            printf("Enter a number: ");
            scanf("%d", &a);
            printf("%d\n",fun(fp[0], a, 0));//factorial
            break;

        case 2: 
            printf("Enter a number: ");
            scanf("%d", &a);
            printf("%d\n",fun(fp[1], a, 0));//fibonacci
            break;

        case 3: 
            printf("Enter the base and exponent: ");
            scanf("%d %d", &a, &b);
            printf(" %d\n",fun(fp[2], a, b));//power
            break;

        default:
            printf("Invalid\n");
            break;
    }
    return 0;
}
