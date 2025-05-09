# This is for C but can easily be modified to work for other languages as well
CC = clang

# Object files go into $(OBJDIR), executables go into $DISTDIR
OBJDIR = build
DISTDIR = dist
INSTALL_DIR ?= install

# Uncomment next line to overwrite standard malloc library
# WRAP = -Wl,--wrap=malloc -Wl,--wrap=free -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=malloc_usable_size

# Compiler and linker flags
CFLAGS := $(SCM) -Weverything -Werror -Wno-unsafe-buffer-usage -Wno-padded -Wno-declaration-after-statement -Wno-used-but-marked-unused -Wall -fPIC -g
LFLAGS := $(CFLAGS) -lpthread

# Gathers all header and C files located in the root directory
# in $(HFILES) and $(CFILES), respectively
HFILES := $(wildcard *.h)
CFILES := $(wildcard *.c)

# Produces in $(OFILES) the names of .o object files for all C files
OFILES := $(patsubst %.c,$(OBJDIR)/%.o,$(CFILES))

# This a rule that defines how to compile all C files that have changed
# (or their header files) since they were compiled last
$(OBJDIR)/%.o : %.c $(HFILES) $(CFILES)
	$(CC) $(CFLAGS) -c $< -o $@

# Consider these targets as targets, not files
.PHONY : all format clean test install

# Build everything: compile all C (changed) files and
# link the object files into an executable (app)
all: dist/libauthorized_keys.a dist/libauthorized_keys.so dist/test

# $(OBJDIR) is an order-only prerequisite:
# only compile if $(OBJDIR) does not exist yet and
# not just because $(OBJDIR) has a new time stamp
$(OFILES): | $(OBJDIR)

# Create directory for $(OBJDIR)
$(OBJDIR):
	mkdir $(OBJDIR)

# Build shared library, copy library header file to $(DISTDIR)
$(DISTDIR)/libauthorized_keys.so: $(OFILES)
	mkdir -p $(DISTDIR)
	$(CC) $(LFLAGS) $(WRAP) $(OFILES) -shared -o $(DISTDIR)/libauthorized_keys.so

# Build shared library, copy library header file to $(DISTDIR)
$(DISTDIR)/libauthorized_keys.a: $(OFILES)
	mkdir -p $(DISTDIR)
	$(AR) rcs $@ $(OFILES) -o $(DISTDIR)/libauthorized_keys.a

# Build test executable to validate library
$(DISTDIR)/test: $(OFILES)
	mkdir -p $(DISTDIR)
	$(CC) $(LFLAGS) $(OFILES) -o $(DISTDIR)/test

format:
	clang-format -style=google -i *.[ch]

# Clean up by removing the $(OBJDIR) and $(DISTDIR) directories
clean:
	rm -rf $(OBJDIR)
	rm -rf $(DISTDIR)
	rm -rf vgcore.*

test: $(DISTDIR)/test
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         $(DISTDIR)/test example_authorized_keys

install: all
	mkdir -p $(INSTALL_DIR)/bin $(INSTALL_DIR)/lib $(INSTALL_DIR)/include
	mv dist/test $(INSTALL_DIR)/bin
	mv dist/libauthorized_keys.so $(INSTALL_DIR)/lib
	mv dist/libauthorized_keys.a $(INSTALL_DIR)/lib
	mv $(HFILES) $(INSTALL_DIR)/include

