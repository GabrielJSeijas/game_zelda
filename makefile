CC = gcc
CFLAGS = -Wall
OBJ = main.o mundo.o juego.o
TARGET = zelda

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)
