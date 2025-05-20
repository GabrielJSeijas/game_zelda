# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall

# Archivos objeto
OBJ = main.o mundo.o juego.o

# Nombre del ejecutable
TARGET = Rundr

# Regla principal
all: $(TARGET)

# Cómo construir el ejecutable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Cómo construir cada archivo .o a partir del .c correspondiente
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Limpieza: elimina archivos objeto y el ejecutable
clean:
	rm -f *.o $(TARGET)
