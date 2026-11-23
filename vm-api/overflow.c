#include <stdlib.h>
int main() {
    int* x = malloc(100 * sizeof *x);
    x[100] = 0;
    freeVec(x);
    return 0;
}
