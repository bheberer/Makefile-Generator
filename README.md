# Makefile-Generator

Program that takes C source files in the command line and prints out a Makefile
for them. Example program call: "generator hello.c goodbye.c"

The user also has the option to use their own cflags.

Once all of the input has been received from the user, the program will looks
line by line through the source files for dependencies, and will then do the
same for the dependencies themselves.
