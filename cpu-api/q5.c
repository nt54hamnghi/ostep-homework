/*
 * Now write a program that uses wait() to wait for the child process
 * to finish in the parent. What does wait() return? What happens if
 * you use wait() in the child?
 * */

/*
 * The wait syscall waits for a state change of a child process of the
 * calling process. State changes include termination. A successful call
 * to wait returns the terminated child's pid. Otherwise, it returns -1.
 * Therefore, assuming the child process has not called forked again,
 * invoking wait in the child process after fork will retunr -1 and does
 * nothing.
 * */

#include <assert.h>
#include <stddef.h>
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

int main(int argc, char* argv[]) {
    if (fork_or_die() == 0) {
        printf("hello \n");
        wait_or_die();
    } else {
        printf("goodbye \n");
    }
    return 0;
}
