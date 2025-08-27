AUTOMAKE_OPTIONS = gnu no-dependencies

CC=gcc
CFLAGS=-Wall -O2
TARGET=mohavim
SOURCE=mohavim.c

bin_PROGRAMS = mohavim
mohavim_SOURCES = mohavim.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

install-exec-hook:
	cd $(DESTDIR)$(bindir) && rm -f rmohavim && $(LN_S) mohavim rmohavim

uninstall-hook:
	cd $(DESTDIR)$(bindir) && rm -f rmohavim

.PHONY: all clean