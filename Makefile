CC = gcc
CXX = g++
AR = ar
CFLAGS = -Wall -O2 -g
LDFLAGS = -shared

OBJFILE = dockerIf.o dockerIfHeaderParser.o
TARGET = libdockerIf.so

HEADER = include/dockerIf.h
TEST = test
TEST_OBJ = test.o
TEST_SRC = test.c
LDLIBS = -ldockerIf

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

all: $(TARGET)

$(TARGET): $(OBJFILE)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJFILE)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST): $(TEST_OBJ)
	$(CC) $(CFLAGS) $(LDLIBS) $< -o $@

clean:
	rm -rf *.o $(TARGET) test *.so

install: $(TARGET)
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(TARGET) $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -m 644 $(HEADER) $(DESTDIR)$(PREFIX)/include/


