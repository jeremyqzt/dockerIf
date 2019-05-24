CC = gcc
CXX = g++
AR = ar
CFLAGS = -Wall -O2 -g

OBJFILE = dockerIf/dockerIf.c dockerIfParser/dockerIfHeaderParser.c
CXXFILES = jsoncpp/jsoncpp.cpp dockerIfParser/dockerIfParser.cpp

TARGET = libdockerIf.so
TARGETCPP = libdockerIfParser.so

HEADER = dockerIf/include/dockerIf.h dockerIfParser/include/dockerIfParser.h
TEST = test
TEST_OBJ = test.o
LDLIBS = -ldockerIf -ldockerIfParser

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

all: $(TARGET) $(TARGETCPP)

$(TARGET): $(OBJFILE)
	$(CC) -fPIC -shared  $^ -o $(TARGET)


$(TARGETCPP): $(CXXFILES)
	$(CXX) -fPIC -shared $^ -o $(TARGETCPP)


$(TEST): $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -rf *.o $(TARGET) dockerIf/*.o dockerIfParser/*.o jsoncpp/*.o test *.so

install: $(TARGET) $(TARGETCPP)
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(TARGET) $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(TARGETCPP) $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -m 644 $(HEADER) $(DESTDIR)$(PREFIX)/include/


