/*
 * Write a slight modification of the previous program, this time using waitpid()
 * instead of wait(). When would waitpid() be useful?
 * */

/*
 * waitpid takes a pid integer as input, allowing more control over which child
 * to wait for. If the pid is greater than 0, the calling process will wait for
 * that specific pid. wait(NULL) is equivalent to waitpid(-1, NULL, 0), where -1
 * means any child. Moreover, waitpid's third parameter is for passing a set of
 * options by OR-ing them together; the available options are WNOHANG, WUNTRACED,
 * and WCONTINUED.
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int fork_or_die() {
    int rc = fork();
    assert(rc >= 0);
    return rc;
}

int main(int argc, char* argv[]) {
    if (fork_or_die() == 0) {
        printf("child pid: %d\n", getpid());
    } else {
        int rc = waitpid(-1, NULL, 0);
        assert(rc > 0);

        printf("parent pid: %d\n", getpid());
    }
    return 0;
}
