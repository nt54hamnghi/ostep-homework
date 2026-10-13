/*
 * Write a program that opens a file (with the open() system call)
 * and then calls fork() to create a new process. Can both the child
 * and parent access the file descriptor returned by open()? What
 * happens when they are writing to the file concurrently, i.e., at the
 * same time?
 */

/*
 * 1. The child receives a copy of the parent's file descriptor table
 *    after fork(). Both processes' descriptors refer to the same
 *    open file description in the kernel.
 *
 * 2. After fork(), the parent and child execute concurrently. The OS
 *    scheduler determines which runs first. Therefore, writes from both
 *    processes can interleave. O_NONBLOCK does not control which process
 *    writes first.
 *
 *    For regular files:
 *      - write() may block, regardless of O_NONBLOCK.
 *      - O_NONBLOCK generally has no effect on regular file I/O.
 */

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void wait_or_die() {
    int rc = wait(NULL);
    assert(rc > 0);
}

int fork_or_die() {
    int rc = fork();
    assert(rc >= 0);
    return rc;
}

int main() {
    int fd = creat("./test.txt", 0644);
    // int fd = open("./test.txt", O_CREAT | O_WRONLY | O_TRUNC | O_NONBLOCK,
    // 0064);

    if (fork_or_die() == 0) {
        printf("child (pid:%d)\n", (int)getpid());
        const char* msg = "Hello, world!\n";
        for (int i = 0; i < 10; i++) {
            write(fd, msg, 14);
        }
    } else {
        printf("parent (pid:%d)\n", (int)getpid());
        const char* msg = "Ola, world!\n";
        for (int i = 0; i < 10; i++) {
            write(fd, msg, 12);
        }
    }

    return 0;
}
