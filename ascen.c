#include<stdio.h>
typedef void(*p)(int arr[],int size);
void callback(int arr[],int size,p t);
void ascen(int arr[],int size);
void decen(int arr[],int size);

void main() {
    int i, ele, a[5];
    ele = sizeof(a) / sizeof(a[0]);
    for(i = 0; i < ele; i++)
        scanf(" %d", &a[i]);

    callback(a, ele, ascen);
    printf("ascending\n");
    for(i = 0; i < ele; i++)
        printf("%d\n", a[i]);

    callback(a, ele, decen);
    printf("descending\n");
    for(i = 0; i < ele; i++)
        printf("%d\n", a[i]);
}

void ascen(int a[], int ele) {
    int i, j, t;
    for(i = 0; i < ele - 1; i++) {
        for(j = 0; j < ele - 1 - i; j++) {
            if(a[j] > a[j + 1]) {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
        }
    }
}

void decen(int a[], int ele) {
    int i, j, t;
    for(i = 0; i < ele - 1; i++) {
        for(j = 0; j < ele - 1 - i; j++) {
            if(a[j] < a[j + 1]) {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
        }
    }
}

void callback(int arr[], int size, p t) {
    t(arr, size);
}

