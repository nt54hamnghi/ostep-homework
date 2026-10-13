/*
 * Write a program that calls fork(). Before calling fork(),
 * have the main process access a variable (e.g., x) and set
 * its value to something (e.g., 100). What value is the variable
 * in the child process? What happens to the variable when both
 * the child and parent change the value of x?
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
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
    int x = 100;
    printf("x before fork: %d\n", x);
    if (fork_or_die() == 0) {
        x = 101;
        printf("x in child: %d\n", x);
        exit(0);
    }
    wait_or_die();
    printf("x after child exit: %d\n", x);
    return 0;
}
