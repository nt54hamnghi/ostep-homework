/*
 * Write a program that creates a child process, and then in the child
 * closes standard output (STDOUT_FILENO). What happens if the child
 * calls printf() to print some output after closing the descriptor?
 * */

/*
 * close() closes the file descriptor, so that it no longer refers to
 * any file and may be reused. If the fd is the last reference to an
 * open file description, the resources for that description are freed.
 * If it's the last fd to a file that has been unlinked, then the file
 * is removed.
 *
 * https://stackoverflow.com/a/59096214
 * In general, a file attached to the terminal is line-buffered, while
 * other files are block-buffered, and stderr is unbuffered. Closing
 * stdout detaches it from the terminal, so it becomes block-buffered.
 * printf will happily copy the data into this buffer without failing,
 * even if the fd is closed. Once the buffer fills or when the content
 * is flushed with fflush, printf flushes it with write(), which fails
 * and returns an error to printf. In general, you don't want to close
 * the default streams, because any new file you open will take the
 * lowest available fd. This can have unintended results if another part
 * of the program writes to stdout, not knowing that it has been closed,
 * potentially corrupting the content.
 */

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
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
    printf("parent pid: %d\n", getpid());
    if (fork_or_die() == 0) {
        close(STDOUT_FILENO);
        int res = printf("child pid: %d\n", getpid());
        fflush(stdout);
        fprintf(stderr, "%d %s\n", res, strerror(errno));
    } else {
        wait_or_die();
    }
    return 0;
}
