# Makefile-Generator

After using C with Emacs during my systems class, a couple of friends and I
thought that creating a simple Makefile generator would be a fun little side
project to get done over the summer.

To use the program, the user must type in the names of the source files they
want to compile in the command line while calling generator.exe. The program
then asks them for the name of the executable and the cflags that the user
wishs to include.

The program then goes through these source files and creates a rule for each.
These rules contain the target for the rule as well as a linked list of
dependencies. After creating the rule the program then goes in and looks through
the lines of the source files looking for these dependencies. It then goes
into the dependencies looking to see if they have any dependencies to account
for. Once the dependencies are all accounted for, the rules are printed out
in a proper Makefile format.

The example Makefile included in this Repo shows an example of two different
source files being used, as well as headers that also rely on other headers
not included in the original source files.

The program will spit out errors if the user doesn't input a .c file into the
command line, or if one of the source files or dependencies doesn't exist in the
current directory.
