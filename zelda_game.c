#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Definición de estructuras
typedef struct Item {
    char nombre[50];
    int encontrado;
    struct Mazmorra* mazmorra_asociada;
} Item;

typedef struct Mazmorra {
    char nombre[50];
    int derrotada;
    Item* item_requerido;
    Item* item_oculto;
} Mazmorra;

typedef struct Aldea {
    char nombre[50];
    Mazmorra* mazmorra_asociada;
    Item* item_oculto;
    struct Aldea* anterior;
    struct Aldea* siguiente;
} Aldea;

// Variables globales
Aldea* mundo_superior = NULL;
Aldea* mundo_paralelo = NULL;
Item* items_superior = NULL;
Item* items_paralelo = NULL;
Aldea* aldea_actual = NULL;
int vidas = 3;
int dinero = 0;
int mundo_paralelo_desbloqueado = 0;

// Prototipos de funciones
void inicializar_juego(int num_aldeas);
void ciclo_principal();
void liberar_memoria();

int main() {
    int num_aldeas;
    
    printf("Bienvenido a Zelda!\n");
    printf("Ingrese el número de aldeas a generar: ");
    scanf("%d", &num_aldeas);
    
    inicializar_juego(num_aldeas);
    ciclo_principal();
    liberar_memoria();
    
    return 0;
}

char* generar_nombre(const char** base_nombres, int num_base, int index) {
    // Implementa la lógica para combinar nombres según el índice
}

void inicializar_juego(int num_aldeas) {
    srand(time(NULL));
    
    // 1. Crear listas de nombres base
    const char* nombres_aldeas[] = {"Kakariko", "Hateno", "Zora", "Goron", "Gerudo"};
    const char* nombres_mazmorras[] = {"Agua", "Tierra", "Fuego", "Aire"};
    const char* nombres_items[] = {"Espada", "Escudo", "Arco", "Boomerang", "Bomba"};
    
    // 2. Crear mundo superior
    for (int i = 0; i < num_aldeas; i++) {
        Aldea* nueva_aldea = (Aldea*)malloc(sizeof(Aldea));
        // Configurar nombres, mazmorras asociadas, etc.
    }
    
    // 3. Crear mundo paralelo (similar al superior)
    
    // 4. Crear listas de items
    
    // 5. Asignar items a aldeas y mazmorras
    
    // 6. Configurar aldea inicial
    aldea_actual = mundo_superior;
}


void comando_aldea(char* comando) {
    if (strcmp(comando, "busq") == 0) {
        // Lógica para buscar ítem
    } else if (strcmp(comando, "maz") == 0) {
        // Ir a mazmorra asociada
    } else if (strcmp(comando, "compr") == 0) {
        // Mostrar menú de compra
    } else if (strcmp(comando, "trans") == 0) {
        // Transportar a mundo paralelo si está desbloqueado
    } else if (strcmp(comando, "ant") == 0) {
        // Ir a aldea anterior
    } else if (strcmp(comando, "sig") == 0) {
        // Ir a aldea siguiente
    } else {
        printf("Comando no reconocido.\n");
    }
}

void comando_mazmorra(char* comando) {
    // Similar a comando_aldea pero con acciones específicas de mazmorra
}

void ciclo_principal() {
    char comando[10];
    
    while (vidas > 0) {
        printf("\nEstás en: %s\n", aldea_actual->nombre);
        printf("Vidas: %d | Dinero: $%d\n", vidas, dinero);
        printf("> ");
        scanf("%s", comando);
        
        if (esta_en_aldea()) {
            comando_aldea(comando);
        } else {
            comando_mazmorra(comando);
        }
        
        // Verificar condición de victoria
    }
    
    if (vidas <= 0) {
        printf("Game Over!\n");
    }
}