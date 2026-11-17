# Parallel File Processor - Phase 1
CC     = gcc
CFLAGS = -Wall -Wextra -Iinclude
TARGET = app
SRC    = $(wildcard src/*.c)
HDR    = include/processor.h

all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean