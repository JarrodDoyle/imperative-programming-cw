#include <stdio.h>
#include <limits.h>

int main() {
    long high = 100000000000;
    long low = 0;
    long mid;
    int i;
    while (low < high - 1) {
        mid = low + (high - low) / 2;
        i = mid;
        if (i == mid) {
            low = mid;
        } else {
            high = mid;
        }
    }
    printf("Highest int using halving method %ld\n", mid);
    printf("Actual highest int %d\n", INT_MAX);
    printf("Lowest int %d\n", INT_MIN);
    return 0;
}
