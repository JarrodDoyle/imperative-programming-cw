#include <stdio.h>

int square(int n) {
    return n * n;
}

int main() {
    int i;
    i = square(42);
    printf("%d\n", i);
    return 0;
}
