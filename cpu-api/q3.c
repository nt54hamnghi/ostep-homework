/*
 * Write another program using fork(). The child process should print “hello”;
 * the parent process should print “goodbye”. You should try to ensure that the
 * child process always prints first; can you do this without calling wait() in
 * the parent?
 */

/*
 * We can use a pipe for synchronization, created before fork():
 *
 * - parent calls read() on the pipe, which blocks until data arrives.
 * - child prints "hello\n", then writes one byte to the pipe.
 * - parent’s read() returns, and it prints "goodbye\n".
 *
 * This guarantees the child prints first without using wait(), assuming the
 * calls succeed.
 *
 * Using sleep() in the parent only delays it; it does not guarantee the order.
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
    printf("----\n");

    int pipefd[2];
    int rc = pipe(pipefd);
    assert(rc == 0);

    if (fork_or_die() == 0) {
        // child will run till completion and write
        close(pipefd[0]); // close read end

        printf("hello\n");

        int n = write(pipefd[1], "done", 5);
        assert(n == 5);

        close(pipefd[1]); // done, close write end
    } else {
        // parent will wait for data to read
        close(pipefd[1]); // close write end

        char buf[5];
        int n = read(pipefd[0], buf, 5);
        assert(n == 5);

        printf("goodbye\n");

        close(pipefd[0]); // done, close read end
    }
}
