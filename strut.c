#include<stdio.h>

int sum(int m, int n) {
    return m + n;
}

int mul(int m, int n) {
    return m * n;
}

struct st {
    int (*sum)(int, int);
    int (*mul)(int, int);
};

void main() {
    struct st var;
    var.sum = sum;
    printf("%d\n", var.sum(3, 5));
    var.mul = mul;
    printf("%d\n", var.mul(3, 5));
}

