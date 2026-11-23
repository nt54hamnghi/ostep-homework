#include <stdio.h>
#include <stdlib.h>
int main() {
    int* x = malloc(100 * sizeof *x);
    freeVec(x);
    printf("%d\n", x[0]);
    return 0;
}
