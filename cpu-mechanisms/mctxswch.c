/*
 * Measuring the cost of a context switch is a little trickier.
 * The lmbench benchmark does so by running two processes on a
 * single CPU, and setting up two UNIX pipes between them:
 *
 * 1. The first process issues a write to the first pipe, and
 *    waits for a read on the second.
 *
 * 2. The OS puts the first process in the blocked state, and
 *    switches to the other process.
 *
 * 3. This process reads from the first pipe and then writes
 *    to the second. When it tries to read from the first pipe
 *    again, it blocks, and thus the back-and-forth cycle of
 *    communication continues.
 *
 * By measuring the cost of communicating like this repeatedly,
 * lmbench can make a good estimate of the cost of a context
 * switch. You can try to re-create something similar here,
 * using pipes, or perhaps some
 *
 * One difficulty in measuring context-switch cost arises in
 * systems with more than one CPU; you need to ensure that your
 * context-switching processes are located on the same processor.
 * Fortunately, most operating systems have calls to bind a process
 * to a particular processor.
 *
 * On Linux, the sched_setaffinity() call is what you're looking for.
 * By ensuring both processes are on the same processor, you are making
 * sure to measure the cost of the OS stopping one process and restoring
 * another on the same CPU.
 */

#define _GNU_SOURCE
#include <assert.h>
#include <sched.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 100000

static int fork_or_die() {
    int rc = fork();
    assert(rc >= 0);
    return rc;
}

static long long elapsed_us(struct timeval end, struct timeval start) {
    return (end.tv_sec - start.tv_sec) * 1000000LL + (end.tv_usec - start.tv_usec);
}

int main(void) {
    // Pin this process (and the child, which inherits it) to CPU 0.
    cpu_set_t set;    // CPU affinity(linking) mask
    CPU_ZERO(&set);   // empty cpu set
    CPU_SET(0, &set); // add cpu 0
    // pid = 0 means the calling thread.
    assert(sched_setaffinity(0, sizeof(set), &set) == 0);

    int pd0[2], pd1[2];
    assert(pipe(pd0) == 0);
    assert(pipe(pd1) == 0);
    char byte = 'x';

    if (fork_or_die() == 0) {
        // child: writes to pd0, reads from pd1
        close(pd0[0]); // unused: read end of pd0
        close(pd1[1]); // unused: write end of pd1

        struct timeval start, end;
        gettimeofday(&start, NULL);
        for (int i = 0; i < N; i++) {
            assert(write(pd0[1], &byte, 1) == 1);
            assert(read(pd1[0], &byte, 1) == 1);
        }
        gettimeofday(&end, NULL);

        close(pd0[1]);
        close(pd1[0]);

        double taken = (double)elapsed_us(end, start);
        printf("total time: %.6f s\n", taken / 1e6);
        printf("per round trip: %.3f µs\n", taken / N);
        printf("per context switch: %.3f µs\n", taken / (2.0 * N));
    } else {
        // parent: reads from pd0, writes to pd1
        close(pd0[1]); // unused: write end of pd0
        close(pd1[0]); // unused: read end of pd1

        for (int i = 0; i < N; i++) {
            assert(read(pd0[0], &byte, 1) == 1);
            assert(write(pd1[1], &byte, 1) == 1);
        }

        close(pd0[0]);
        close(pd1[1]);
        wait(NULL);
    }
    return 0;
}
