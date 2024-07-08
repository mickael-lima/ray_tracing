##
# ray-tracing
#
# @file
# @version 0.1
CC = g++
CFLAGS = -ggdb -pedantic-errors -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -std=c++23
TARGET = main
INCLUDES = lib/*
SRCS = src/*

all: ${TARGET}

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o ray-tracing ${TARGET}.cpp ${SRCS}

clean:
	rm ray-tracing

# end
