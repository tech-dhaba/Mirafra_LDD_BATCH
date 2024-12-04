#include<stdio.h>
typedef int(*q)(int);
void callback(int *p, int size, q t);
int dble(int x);
int squre(int x);

void main() {
    int i, ele, a[5];
    ele = sizeof(a) / sizeof(a[0]);
    for(i = 0; i < ele; i++)
        scanf("%d", &a[i]);
    printf("double of a given number\n");
    callback(a, ele, dble);
    for(i = 0; i < ele; i++)
        printf("%d\n", a[i]);
    printf("square of a given number\n");
    callback(a, ele, squre);
    for(i = 0; i < ele; i++)
        printf("%d\n", a[i]);
}

int dble(int x) {
    return x * 2;
}

int squre(int x) {
    return x * x;
}

void callback(int *p, int size, q t) {
    for(int i = 0; i < size; i++) {
        p[i] = t(p[i]);
    }
}

