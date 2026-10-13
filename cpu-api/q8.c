/*
 * Write a program that creates two children, and connects the standard
 * output of one to the standard input of the other, using the
 * */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void wait_or_die(int pid) {
    int rc = waitpid(pid, NULL, 0);
    assert(rc > 0);
}

int fork_or_die() {
    int rc = fork();
    assert(rc >= 0);
    return rc;
}

int main(int argc, char* argv[]) {
    int pd[2];
    pipe(pd);

    printf("parent pid: %d\n", getpid());
    int cpid1 = fork_or_die();
    if (cpid1 == 0) {
        printf("first child pid: %d\n", getpid());

        // first child will write, so close read end
        close(pd[0]);
        // close stdout fd, and replace it with the write end
        dup2(pd[1], STDOUT_FILENO);
        // should write to write end of pipe
        printf("hello, sibbling\n");
        // done writing, close write end
        close(pd[1]);
    } else {
        int cpid2 = fork_or_die();
        if (cpid2 == 0) {
            printf("second child pid: %d\n", getpid());

            // second child will read, so close write end
            close(pd[1]);
            // close stdin fd, and replace it with the read end
            dup2(pd[0], STDIN_FILENO);
            // should read from end end of pipe
            char buffer[100];
            fgets(buffer, sizeof(buffer), stdin);

            printf("received from: %d, message: %s", getpid(), buffer);
            close(pd[0]); // done reading, close read end
        } else {
            wait_or_die(cpid1);
            wait_or_die(cpid2);
            close(pd[0]);
            close(pd[1]);
        }
    }

    return 0;
}
