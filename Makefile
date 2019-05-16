CC = gcc
AR = ar
CFLAGS = -Wall -O2
OBJFILE = dockerIf/dockerIf.o dockerIfParser/dockerIfHeaderParser.o
TARGET = libdockerIf.a
HEADER = dockerIf/include/dockerIf.h dockerIfParser/include/dockerIfParser.h
TEST = test
TEST_OBJ = test.o
LDLIBS = -ldockerIf

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

all: $(TARGET)

$(TARGET): $(OBJFILE)
	$(AR) rcs $(TARGET) $(OBJFILE)

$(TEST): $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -rf *.o $(TARGET) dockerIf/*.o dockerIfParser/*.o test

install: $(TARGET)
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(TARGET) $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -m 644 $(HEADER) $(DESTDIR)$(PREFIX)/include/


