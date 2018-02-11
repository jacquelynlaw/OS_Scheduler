# typing "make" in terminal will compile and link main.c, sjf.c
# run by typing "./a.out" after compiling

# Note: The "-I" command has makefile look in current directory for .h file, can be ignored

program: main.c fcfs.c sjf.c rr.c
	gcc -o a.out main.c fcfs.c sjf.c rr.c -I.