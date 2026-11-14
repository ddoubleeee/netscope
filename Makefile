CC = gcc
CFLAGS = -std=c23 -Wall -Wextra -Iinclude -O2

TARGET = build/netscope
SRC = $(wildcard src/*.c)

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean