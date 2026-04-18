CC = gcc # Compiler Chain
CFLAGS = -Wall -Wextra -std=gnu17 -O2 # Compiler Flags
TARGET = socket
SRC = ./core/socket.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)