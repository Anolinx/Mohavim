CC = gcc
CFLAGS = -Wall -O2 -fPIC
TARGET_DIR = .

# Plugin targets
PLUGIN_TARGETS = quick_terminal.so

all: $(PLUGIN_TARGETS)

# Compile quick_terminal plugin
quick_terminal.so: quick_terminal.c
	$(CC) $(CFLAGS) -shared -o $@ $<

# Install plugins to the plugins directory
install: $(PLUGIN_TARGETS)
	cp $(PLUGIN_TARGETS) $(TARGET_DIR)/

# Clean compiled files
clean:
	rm -f *.so

.PHONY: all install clean