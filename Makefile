CXX=g++
#/home/maxim/workspace/elvis/3.Implementation/Software/FirmwareBuilder/_build/buildroot-2019.02.3/achilles2-dev/host/bin/arm-none-linux-gnueabi-g++
CXXFLAGS=-I./eventpp/include -I./ -std=c++11 -g -DDEBUG=1 -D__cplusplus=201103L

# set non-optional preprocessor flags here
# # eg. project specific include directories
CPPFLAGS +=


#DEPS = ./eventpp/include/eventpp/callbacklist.h ./EvtQueue.hpp
#OBJ = main.o EvtQueue.o

# find cpp files in subdirectories
#SOURCES := $(shell find . -name '*.cpp')
SOURCES := ./main.cpp ./EvtQueue.cpp
LIBS=-lpthread


# find headers
HEADERS := ./EvtQueue.hpp ./eventpp/include/eventpp/callbacklist.h

OUTPUT := ttt

all: $(OUTPUT)

$(OUTPUT): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(OUTPUT) $(SOURCES) $(LIBS)
clean:
	$(RM) $(OUTPUT)
