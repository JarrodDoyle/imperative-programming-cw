/* Classify a triangle according to the integer lengths of its sides. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>

// Integer constants representing types of triangle.
enum { Equilateral, Isosceles, Right, Scalene, Flat, Impossible, Illegal };

bool valid(const char str[]) {
    char *ptr;
    int i = strtol(str, &ptr, 10);
    char temp[100];
    sprintf(temp, "%d", i);
    if (strcmp(str, temp) == 0 && str[0] != '0' && str[0] != '-') return true;
    return false;
}

// Convert a string into an integer.  Return -1 if it is not valid.
int convert(const char length[]) {
    if (!valid(length)) return -1;
    char *ptr;
    int i = strtol(length, &ptr, 10);
    return i;
}

// Classify a triangle, given side lengths as strings:
int triangle(int a, int b, int c) {
    long aa = a; long bb = b; long cc = c;
    // Rearrange the sides to be in order (shortest to longest)
    while (!(aa <= bb && bb <= cc)){
        long temp;
        if (aa > bb){
            temp = aa; aa = bb; bb = temp;
        }
        if (bb > cc) {
            temp = bb; bb = cc; cc = temp;
        }
    }
    if (aa <= 0 || bb <= 0 || cc <= 0) return Illegal;
    if (aa == bb && aa == cc) return Equilateral;
    if (aa == bb || aa == cc || bb == cc) return Isosceles;
    if (aa*aa + bb*bb == cc*cc) return Right;
    if (aa + bb > cc) return Scalene;
    if (aa + bb == cc) return Flat;
    return Impossible;
}

// -----------------------------------------------------------------------------
// User interface and testing.

void print(int type) {
    switch (type) {
        case Equilateral: printf("Equilateral"); break;
        case Isosceles: printf("Isosceles"); break;
        case Right: printf("Right"); break;
        case Scalene: printf("Scalene"); break;
        case Flat: printf("Flat"); break;
        case Impossible: printf("Impossible"); break;
        case Illegal: printf("Illegal"); break;
    }
    printf("\n");
}

// A replacement for the library assert function.
void assert(int line, bool b) {
    if (b) return;
    printf("The test on line %d fails.\n", line);
    exit(1);
}

// Check that you haven't changed the triangle type constants.  (If you do, it
// spoils automatic marking, when your program is linked with a test program.)
void checkConstants() {
    assert(__LINE__, Equilateral==0 && Isosceles==1 && Right==2 && Scalene==3);
    assert(__LINE__, Flat==4 && Impossible==5 && Illegal==6);
}

// Tests 1 to 2: check equilateral
void testEquilateral() {
    assert(__LINE__, triangle(8, 8, 8) == Equilateral);
    assert(__LINE__, triangle(1073, 1073, 1073) == Equilateral);
}

// Tests 3 to 5: check isosceles
void testIsosceles() {
    assert(__LINE__, triangle(25, 25, 27) == Isosceles);
    assert(__LINE__, triangle(25, 27, 25) == Isosceles);
    assert(__LINE__, triangle(27, 25, 25) == Isosceles);
}

// Tests 6 to 14: check right angled
void testRight() {
    assert(__LINE__, triangle(3, 4, 5) == Right);
    assert(__LINE__, triangle(3, 5, 4) == Right);
    assert(__LINE__, triangle(5, 3, 4) == Right);
    assert(__LINE__, triangle(5, 12, 13) == Right);
    assert(__LINE__, triangle(5, 13, 12) == Right);
    assert(__LINE__, triangle(12, 5, 13) == Right);
    assert(__LINE__, triangle(12, 13, 5) == Right);
    assert(__LINE__, triangle(13, 5, 12) == Right);
    assert(__LINE__, triangle(13, 12, 5) == Right);
}

// Tests 15 to 20: check scalene
void testScalene() {
    assert(__LINE__, triangle(12, 14, 15) == Scalene);
    assert(__LINE__, triangle(12, 15, 14) == Scalene);
    assert(__LINE__, triangle(14, 12, 15) == Scalene);
    assert(__LINE__, triangle(14, 15, 12) == Scalene);
    assert(__LINE__, triangle(15, 12, 14) == Scalene);
    assert(__LINE__, triangle(15, 14, 12) == Scalene);
}

// Tests 21 to 25: check flat
void testFlat() {
    assert(__LINE__, triangle(7, 9, 16) == Flat);
    assert(__LINE__, triangle(7, 16, 9) == Flat);
    assert(__LINE__, triangle(9, 16, 7) == Flat);
    assert(__LINE__, triangle(16, 7, 9) == Flat);
    assert(__LINE__, triangle(16, 9, 7) == Flat);
}

// Tests 26 to 31: check impossible
void testImpossible() {
    assert(__LINE__, triangle(2, 3, 13) == Impossible);
    assert(__LINE__, triangle(2, 13, 3) == Impossible);
    assert(__LINE__, triangle(3, 2, 13) == Impossible);
    assert(__LINE__, triangle(3, 13, 2) == Impossible);
    assert(__LINE__, triangle(13, 2, 3) == Impossible);
    assert(__LINE__, triangle(13, 3, 2) == Impossible);
}

// Tests 32 to 44: check conversion.
// Leading zeros are disallowed because thy might be mistaken for octal.
void testConvert() {
    assert(__LINE__, convert("1") == 1);
    assert(__LINE__, convert("12345678") == 12345678);
    assert(__LINE__, convert("2147483647") == 2147483647);
    assert(__LINE__, convert("2147483648") == -1);
    assert(__LINE__, convert("2147483649") == -1);
    assert(__LINE__, convert("0") == -1);
    assert(__LINE__, convert("-1") == -1);
    assert(__LINE__, convert("-2") == -1);
    assert(__LINE__, convert("-2147483648") == -1);
    assert(__LINE__, convert("x") == -1);
    assert(__LINE__, convert("4y") == -1);
    assert(__LINE__, convert("13.4") == -1);
    assert(__LINE__, convert("03") == -1);
}

// Tests 45 to 50: check for correct handling of overflow
void testOverflow() {
    assert(__LINE__, triangle(2147483647,2147483647,2147483646) == Isosceles);
    assert(__LINE__, triangle(2147483645,2147483646,2147483647) == Scalene);
    assert(__LINE__, triangle(2147483647,2147483647,2147483647) == Equilateral);
    assert(__LINE__, triangle(1100000000,1705032704,1805032704) == Scalene);
    assert(__LINE__, triangle(2000000001,2000000002,2000000003) == Scalene);
    assert(__LINE__, triangle(150000002,666666671,683333338) == Scalene);
}

// Run tests on the triangle function.
void test() {
    checkConstants();
    testEquilateral();
    testIsosceles();
    testRight();
    testScalene();
    testFlat();
    testImpossible();
    testConvert();
    testOverflow();
    printf("All tests passed\n");
}

// Run the program or, if there are no arguments, test it.
int main(int n, char *args[n]) {
    setbuf(stdout, NULL);
    if (n == 1) {
        test();
    }
    else if (n == 4) {
        int a = convert(args[1]), b = convert(args[2]), c = convert(args[3]);
        int result = triangle(a, b, c);
        print(result);
    }
    else {
        fprintf(stderr, "Use e.g.: ./triangle 3 4 5\n");
        exit(1);
    }
    return 0;
}
