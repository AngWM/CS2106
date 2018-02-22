# We define some symbols that let us set which compiler to
# use, and what compilation options we would like. This is
# equivalent to using #define in C/C++. Symbols are accessed
# using the $() operator, as you will soon see.

# First our compiler. We will use gcc
CC = gcc

# Compilation options. -O3 means optimize the code
# as much as possible. -Wall means warn us of all
# potential problems in the code.
CCOPTS = -O3 -Wall

# The DEPS symbol tells us of the header files and other
# files that are needed in this project. Files listed
# in DEPS will trigger a recompilation of all modules
# if they are modified.
DEPS = db.h

# Now we come to rules. The left hand of a rule
# before the ":" tells us what we want to generate.
# This is called a "target".
# The right side of the ":" tells us what is needed
# to generate our targets. The right side is called
# a dependency.
# The second line of a rule tells us what we must
# do to produce the target from the dependency.
# In this case, to generate a .o (object) file,
# We depend on .c (source) file and other files like
# headers, etc.
# The % symbol is a "wildcard" symbol, which means
# to generate anything that is of type .o, look
# for files with type .c.
# The next line (which MUST be indented with a TAB)
# tells us that to generate the .o file, we use
# gcc. The $@ represents the target while the $<
# represents the dependency.
# Note that the second line fo the rule MUST
# be indented with a TAB, not with spaces.
# Note how we access the CC, DEPS and CCOPTS
# sumbols using $()

%.o: %.c $(DEPS)
	$(CC) $(CCOPTS) -c -o $@ $<

# More symbols
BINARIES = db.o
ALL = phonebook

# Another rule, which tells us that we should compile
# phonebook. There is a rule further down on how
# to compile phonebook. Because this is the first
# "non wildcare" rule in the Makefile, it is the
# default rule to be executed when you type "make".
all: $(ALL)

# A rule to clean the directory of all object
# files and binaries. Invoke by typing "make clean"
clean:
	rm -f $(ALL) $(BINARIES)

# Finally the rule on how to make phonebook.
# The $^ symbol just refers to the right side
# of the rule.

phonebook: phonebook.o $(BINARIES)
	$(CC) $(CCOPTS) $^ -o $@
