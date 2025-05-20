#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "estructuras.h"
#include "mundo.h"
#include "juego.h"

int main() {
    int n;
    printf("🌍 ¿Cuántos mundos deseas que Yggdrasil te revele?\n> ");
    scanf("%d", &n);

    srand(time(NULL));

    Aldea *inicio = crearMundo(n);
    jugar(inicio);

    return 0;
}
