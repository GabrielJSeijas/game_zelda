#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "estructuras.h"
#include "mundo.h"
#include "juego.h"

int main() {
    int n;
    printf("¿Cuántas aldeas quieres generar? ");
    scanf("%d", &n);

    srand(time(NULL));

    Aldea *inicio = crearMundo(n);
    jugar(inicio);

    return 0;
}
