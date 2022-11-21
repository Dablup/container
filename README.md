# My container

## Explanation of functions

Source code of the container is located at file container.c. Here we have several functions:
1) `int main(int argc, char** argv)` - entry point of the container, from here we call function `clone_process` to create a container process. CLONE_NEWPID is used to isolate PID, CLONE_NEWNET is used to isolate net.
2) `void clone_process(Function&& function, int flags)` - clone a process with new allocated memory
3) `char* stack_memory()` - allocates some amount of memory
4) `int jail(void *args) ` - all main logic for container process is here. Here we set up hostname, environmental variables, mount file system, and runs the shell.


## How to run?

1) Navigate to the folder where file `container.c` is located.
2) Run here: `g++ container.c -o container`
3) After that run: `sudo ./container`
4) You will see a running shell. It is your new isolated container.


## How to run a `sysbench` in our container?

1) You need to set up `sysbench` using: ` /# apk add --upgrade sysbench`
2) Then you can run benchmarking testing, for example: `sysbench --test=memory --num-threads=4 run`