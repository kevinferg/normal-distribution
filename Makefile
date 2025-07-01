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
    TOUCH = touch
else
    RM = cmd.exe /C del /Q
    TOUCH = cmd.exe /C echo.>
    MKDIR = mkdir
    EXE = .exe
    SLASH = \\
endif

CC = gcc

# Directory and file handling
VPATH = $(SRCDIR):$(OBJDIR)
INC_LOCATIONS = $(shell find $(INCDIR) $(SRCDIR) $(RAYLIB_INC) -type d)
INC_FLAGS = $(addprefix -I,$(INC_LOCATIONS))
CFLAGS = $(INC_FLAGS) -std=c99 -O2 -MMD -MP

# List of all .c files in src/, recursively
srcs = $(shell find $(SRCDIR) -name "*.c")
src_objs = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(srcs))
deps     = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.d, $(srcs))


all: $(BINDIR)/main$(EXE)

$(BINDIR)/main$(EXE): $(src_objs)
	@echo Linking $@ from obj/*
	@$(CC) $^ -o $@ -s

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo Compiling $< to $@
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

-include $(deps)

.PHONY: clean all

silent:
	@:

clean:
ifeq ($(UNAME_S),Linux)
	$(RM) -f $(src_objs) $(deps) $(BINDIR)/*
	$(TOUCH) $(OBJDIR)/.gitkeep $(BINDIR)/.gitkeep
else
	$(RM) $(subst /,$(SLASH),$(src_objs)) $(subst /,$(SLASH),$(deps)) $(BINDIR)$(SLASH)* 2>nul || exit 0
	$(TOUCH) $(OBJDIR)$(SLASH).gitkeep
	$(TOUCH) $(BINDIR)$(SLASH).gitkeep
endif