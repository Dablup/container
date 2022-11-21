#include <iostream>
#include <sched.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fstream>
#include <sys/mman.h>

char* stack_memory() {
    const int stackSize = 65536;
    auto *stack = new (std::nothrow) char[stackSize];

    if (stack == nullptr) {
        printf("Cannot allocate memory \n");
        exit(EXIT_FAILURE);
    }

    return stack+stackSize;  //move the pointer to the end of the array because the stack grows backward.
}

template <typename... P>
int run(P... params) {
    //basically generating the arguments array at compile time.
    char *args[] = {(char *)params..., (char *)0};
    return execvp(args[0], args);
}

template <typename Function>
void clone_process(Function&& function, int flags){
auto pid = clone(function, stack_memory(), flags, 0);

wait(nullptr);
}


void setup_variables() {
    clearenv();
    setenv("TERM", "xterm-256color", 0);
    setenv("PATH", "/bin/:/sbin/:usr/bin:/usr/sbin", 0);
}

void setup_root(const char* folder){
    chroot(folder);
    chdir("/");
}

int jail(void *args) {

    printf("child pid: %d\n", getpid());
    sethostname("new_container", 20);
    setup_variables();

    setup_root("./root");

    mount("proc", "/proc", "proc", 0, 0);

    auto runThis = [](void *args) ->int { run("/bin/sh"); };

    clone_process(runThis, SIGCHLD);

    umount("/proc");
    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {

    printf("parent pid: %d\n", getpid());
    clone_process(jail, CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD | CLONE_NEWNET);

    return EXIT_SUCCESS;
}

