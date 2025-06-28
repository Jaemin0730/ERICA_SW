CC = gcc
CFLAGS = -Wall -pthread
TARGET = sleeping_ta
SRC = sleeping_ta.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
