#include<stdio.h>

int print(int i) {
    return printf("%d", i);
}

int get() {
    int x = 0;
    scanf("%d", &x);
    return x;
}

