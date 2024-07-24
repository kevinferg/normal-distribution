# Makefile

# Usage:
# Use `mingw32-make` instead of `make` on Windows
# ----------------------------------------------------------------
# mingw32-make         --> Builds executable in the bin/ directory
# mingw32-make clean   --> Removes executable and object files

# Directory names
OBJDIR = obj
INCDIR = inc
SRCDIR = src
BINDIR = bin

# Final executable names (& corresponding sources)
MAIN = main

# Detect OS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    RM = rm -f
    MKDIR = mkdir -p
    EXE = 
    SLASH = /
else
    RM = cmd.exe /C del /Q
    MKDIR = mkdir
    EXE = .exe
    SLASH = \\
endif

CC = gcc

# Directory and file handling
VPATH = $(SRCDIR):$(OBJDIR)
INC_LOCATIONS = $(INCDIR)
INC_FLAGS = $(addprefix -I,$(INC_LOCATIONS))
CFLAGS = $(INC_FLAGS) -MMD -MP

srcs = $(notdir $(wildcard $(SRCDIR)/*.c))
deps = $(srcs:.c=.d)
src_objs = $(addprefix $(OBJDIR)/,$(srcs:.c=.o))

-include $(deps)

all: $(BINDIR)/$(MAIN)$(EXE)

$(BINDIR)/$(MAIN)$(EXE): $(src_objs) | silent
	$(CC) $^ -o $@ -s

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean all

silent:
	@:

clean:
ifeq ($(UNAME_S),Linux)
	$(RM) $(OBJDIR)/* $(BINDIR)/*
else
	$(RM) $(OBJDIR)$(SLASH)* $(BINDIR)$(SLASH)*
endif
