#include <stdio.h>

void mul_by_zero() {
    printf("Performed a multiply by a constant 0\n");
}

void mul_by_one(){
    printf("Performed a multiply by a constant 1\n");
}

void div_by_one() {
    printf("Performed a divide by a constant 1\n");
}

void add_zero() {
    printf("Performed an addition by a constant 0\n");
}

void and_zero() {
    printf("Performed a bitwise and by a constant 0\n");
}

void or_neg_one() {
    printf("Performed a bitwise or by a constant -1\n");
}

void xor_zero() {
    printf("Performed a bitwise xor by a constant 0\n");
}

void sub_zero() {
    printf("Performed a subtraction by a constant 0\n");
}

void xor_self() {
    printf("Performed a bitwise xor of a variable on itself\n");
}

void and_self() {
    printf("Performed a bitwise and of a variable on itself\n");
}

void or_self() {
    printf("Performed a bitwise or of a variable on itself\n");
}
