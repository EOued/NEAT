CFILES = \
	neural_network.c\
	utils.c\
	testing/unit_nn.c

SRCDIR = src
SOURCES = $(foreach file, $(CFILES), $(SRCDIR)/$(file))
OBJDIR = build/obj
INCLUDE = -I$(SRCDIR)/headers
OFILES = $(CFILES:.c=.o)
SUBDIRS = testing
TARGETS = $(foreach file, $(OFILES), $(OBJDIR)/$(file))
EXECDIR = build/target
EXECNAME = nn
EXEC = $(EXECDIR)/$(EXECNAME)
GXX = gcc

FLAGS=-g -Wall -Wextra -Wvla -pedantic -fsanitize=address,undefined -pthread
POSTFLAGS=-lm

debug=0
ifeq ($(debug),1)
FLAGS += -DDEBUG
else
FLAGS += -Werror
endif


$(shell mkdir -p $(OBJDIR))
$(shell mkdir -p $(EXECDIR))
$(foreach dir, $(SUBDIRS), $(shell mkdir -p $(OBJDIR)/$(dir)))

# Rule to generate object files from source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(GXX) $(FLAGS) $(INCLUDE) -c $< -o $@ $(POSTFLAGS)

# The final target depends on all object files
$(EXEC): $(TARGETS)
	$(GXX) $(FLAGS) $(INCLUDE) -o $(EXEC) $(TARGETS) $(POSTFLAGS)

# Clean rule
clean:
	rm -rf $(EXEC) $(OBJDIR);


