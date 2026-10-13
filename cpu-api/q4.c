/*
 * Write a program that calls fork() and then calls some form of
 * exec() to run the program /bin/ls. See if you can try all of the
 * variants of exec(), including (on Linux) execl(), execle(),
 * execlp(), execv(), execvp(), and execvpe(). Why do
 * you think there are so many variants of the same basic call?
 */

#include <assert.h>
#include <fcntl.h>
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
    const char* PATH = "/etc/profiles/per-user/hamnghi/bin/exa";
    if (fork_or_die() == 0) {
        execl(PATH, "exa", "-l", "-a", NULL);
        execlp("exa", "exa", "-l", "-a", NULL);
        execle(PATH, "exa", "-l", "-a", NULL, (char*[]){"HELLO=world", NULL});

        execv(PATH, (char*[]){"exa", "-l", "-a", NULL});
        execvp("exa", (char*[]){"exa", "-l", "-a", NULL});
        execve(PATH, (char*[]){"exa", "-l", "-a", NULL}, (char*[]){"HELLO=world", NULL});
    } else {
        wait_or_die();
    }
}
