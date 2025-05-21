// Autores: Gabriel Seijas 19-00036 y Angel Valero 18-10436
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "estructuras.h"
#include "mundo.h"
#include "juego.h"

// Función principal
// Esta función inicializa el juego, crea el mundo y comienza la aventura
int main() {
    int n;
    printf("🌍 ¿Cuántos mundos deseas que Yggdrasil te revele?\n> ");
    scanf("%d", &n);

    srand(time(NULL));
    // Inicializar el generador de números aleatorios recibe el número de aldeas como argumento
    Aldea *inicio = crearMundo(n);
    // Crear el mundo
    jugar(inicio);
    // Jugar el juego 
    return 0;
    // Liberar memoria
}
