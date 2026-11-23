#include <stdlib.h>

int main() {
    int* ptr = NULL;
    int val = *ptr;
    return 0;
}

// cc -o null: omits the deref, so the program exits normally
// cc -O0 -o null: preserves the deref, causing a segmentation fault
