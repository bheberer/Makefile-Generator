# Makefile-Generator

IMPORTANT NOTE: For those of you who see this code, just note that I will
be rewriting the entire thing in Ruby at some point in the near future. Besides
using Ruby, the main change will be the data structure I use. Instead of the
weird linked-list structure, I'll be using a hashmap with the keys being the
names of the C sources files and the values being sets of file dependencies.

Program that takes C source files in the command line and prints out a Makefile
for them. Example program call: "generator hello.c goodbye.c"

The user also has the option to use their own cflags.

Once all of the input has been received from the user, the program will looks
line by line through the source files for dependencies, and will then do the
same for the dependencies themselves.
