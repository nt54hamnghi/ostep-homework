#include <stdlib.h>
int main() {
    int* x = malloc(100 * sizeof *x);
    freeVec(&x[1]);
    return 0;
}
