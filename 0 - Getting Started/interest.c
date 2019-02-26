#include <stdio.h>

double add(double n, double p) {
    return n * (1 + p / 100.0);
}

int main() {
    double money = 1000.0;
    double interest = 5;
    printf("%f\n", add(money, interest));
    return 0;
}
