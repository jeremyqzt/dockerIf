CC = gcc
CXX = g++
AR = ar
CFLAGS = -Wall -O2 -g
LDFLAGS = -shared

OBJFILE = dockerIf.o
TARGET = libdockerIf.so

HEADER = dockerIf/include/dockerIf.h
TEST = test
TEST_OBJ = test.o
LDLIBS = -ldockerIf

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

all: $(TARGET)

$(TARGET): $(OBJFILE)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJFILE)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET) test *.so

install: $(TARGET) $(TARGETCPP)
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(TARGET) $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(TARGETCPP) $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -m 644 $(HEADER) $(DESTDIR)$(PREFIX)/include/


