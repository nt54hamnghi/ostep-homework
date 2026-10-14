#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#define N 1000000

static long long elapsed(struct timeval end, struct timeval start) {
    return (end.tv_sec - start.tv_sec) * 1000000LL + (end.tv_usec - start.tv_usec);
}

static void check_precision(void) {
    struct timeval start, end;

    for (int i = 0; i < N; i++) {
        gettimeofday(&start, NULL);
        gettimeofday(&end, NULL);
        long diff = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);
        printf("%ld us\n", diff);
    }
}

int main(void) {
    struct timeval start, end;

    gettimeofday(&start, NULL);

    for (int i = 0; i < N; i++) {
        int nbytes = read(0, (char[]){}, 0);
        // printf("read: %d bytes", nbytes);
        // if (nbytes == -1)
        //     printf("%s", strerror(errno));
    }

    gettimeofday(&end, NULL);

    long long taken = elapsed(end, start);
    printf("total time taken: %.6f s\n", (double)taken / 1000000);
    printf("time taken per syscall: %.6f µs\n", (double)taken / N);

    return 0;
}
