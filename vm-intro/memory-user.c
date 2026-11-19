#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main(int argc, char** argv) {

    printf("%d\n", getpid());

    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <memory> [seconds]\n", argv[0]);
        return 1;
    }

    int sec = 5;
    if (argc == 3) {
        sec = (int)strtol(argv[2], NULL, 10);
    }
    alarm(sec);

    size_t mem = (size_t)strtol(argv[1], NULL, 10);
    size_t count = mem * (1 << 20);
    volatile char* arr = (char*)malloc(sizeof(char) * count);
    if (arr == NULL) {
        fprintf(stderr, "Error: failed to allocate memory\n");
        return 1;
    }

    for (;;) {
        for (size_t i = 0; i < count; i++) {
            arr[i] = '!';
        }
    }

    return 0;
}
