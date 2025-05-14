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
void mostrar_descripcion_aldea(Aldea* aldea) {
    printf("\n=== Aldea %s ===\n", aldea->nombre);
    printf("Mazmorra asociada: %s\n", aldea->mazmorra_asociada->nombre);
    if (aldea->item_oculto) {
        printf("Rumores: Hay un item oculto en esta aldea!\n");
    }
    printf("Comandos disponibles: busq, maz, compr, trans, ant, sig\n");
}
// Prototipos de funciones
void inicializar_juego(int num_aldeas);
void ciclo_principal();
void liberar_memoria();
int esta_en_aldea();

// Función para generar nombres combinados
char* generar_nombre(const char* base_nombres[], int num_base, int index) {
    // Primero manejamos los casos base (primeros nombres individuales)
    if (index < num_base) {
        char* nombre = malloc(strlen(base_nombres[index]) + 1);
        strcpy(nombre, base_nombres[index]);
        return nombre;
    }
    
    // Para índices mayores, calculamos las combinaciones
    int max_combinations = (1 << num_base) - 1; // 2^n - 1 combinaciones posibles
    index = (index - num_base) % max_combinations + 1; // Ajustamos el índice
    
    // Buffer para el nombre resultante
    char* resultado = malloc(100); // Tamaño suficiente para las combinaciones
    resultado[0] = '\0';
    
    int first = 1; // Flag para saber si es el primer elemento
    
    // Generar combinación basada en el índice
    for (int i = 0; i < num_base; i++) {
        if (index & (1 << i)) { // Si el bit i está activo en el índice
            if (!first) {
                strcat(resultado, "-");
            }
            strcat(resultado, base_nombres[i]);
            first = 0;
        }
    }
    
    return resultado;
}

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

// Modifica inicializar_juego() así:
void inicializar_juego(int num_aldeas) {
    srand(time(NULL));
    
    const char* nombres_aldeas[] = {"Kakariko", "Hateno", "Zora", "Goron", "Gerudo"};
    const char* nombres_mazmorras[] = {"Agua", "Tierra", "Fuego", "Aire"};
    const char* nombres_items[] = {"Espada", "Escudo", "Arco", "Boomerang", "Bomba"};
    
    int num_nombres_aldeas = sizeof(nombres_aldeas)/sizeof(nombres_aldeas[0]);
    int num_nombres_mazmorras = sizeof(nombres_mazmorras)/sizeof(nombres_mazmorras[0]);
    int num_nombres_items = sizeof(nombres_items)/sizeof(nombres_items[0]);

    // 1. Crear items
    items_superior = malloc(num_aldeas * sizeof(Item));
    for (int i = 0; i < num_aldeas; i++) {
        char* nombre_item = generar_nombre(nombres_items, num_nombres_items, i);
        strncpy(items_superior[i].nombre, nombre_item, 50);
        free(nombre_item);
        items_superior[i].encontrado = 0;
        items_superior[i].mazmorra_asociada = NULL;
    }

    // 2. Crear mundo superior
    Aldea* anterior = NULL;
    for (int i = 0; i < num_aldeas; i++) {
        Aldea* nueva_aldea = malloc(sizeof(Aldea));
        Mazmorra* nueva_mazmorra = malloc(sizeof(Mazmorra));

        // Configurar aldea
        char* nombre_aldea = generar_nombre(nombres_aldeas, num_nombres_aldeas, i);
        strncpy(nueva_aldea->nombre, nombre_aldea, 50);
        free(nombre_aldea);
        
        // Configurar mazmorra
        char* nombre_mazmorra = generar_nombre(nombres_mazmorras, num_nombres_mazmorras, i);
        strncpy(nueva_mazmorra->nombre, nombre_mazmorra, 50);
        free(nombre_mazmorra);
        
        nueva_mazmorra->derrotada = 0;
        nueva_aldea->mazmorra_asociada = nueva_mazmorra;
        
        // Asignar item requerido
        if (i == 0) {
            nueva_mazmorra->item_requerido = &items_superior[0];
        } else {
            int item_index;
            do {
                item_index = rand() % num_aldeas;
            } while (items_superior[item_index].mazmorra_asociada != NULL);
            
            nueva_mazmorra->item_requerido = &items_superior[item_index];
            items_superior[item_index].mazmorra_asociada = nueva_mazmorra;
        }
        
        // Asignar item oculto (50% probabilidad)
        nueva_aldea->item_oculto = NULL;
        if (rand() % 2 == 0) {
            nueva_aldea->item_oculto = &items_superior[rand() % num_aldeas];
        }

        nueva_aldea->anterior = anterior;
        nueva_aldea->siguiente = NULL;
        
        if (anterior != NULL) {
            anterior->siguiente = nueva_aldea;
        } else {
            mundo_superior = nueva_aldea;
        }
        
        anterior = nueva_aldea;
    }
    
    aldea_actual = mundo_superior;
}
int esta_en_aldea() {
    // Implementación simple - asumimos que si no estamos en una mazmorra, estamos en una aldea
    // Esto debería refinarse según tu lógica de juego
    return 1;
}

void comando_aldea(char* comando) {
    if (strcmp(comando, "busq") == 0) {
        // Lógica para buscar ítem
        if (aldea_actual->item_oculto != NULL && !aldea_actual->item_oculto->encontrado) {
            aldea_actual->item_oculto->encontrado = 1;
            printf("¡Encontraste el %s!\n", aldea_actual->item_oculto->nombre);
        } else {
            printf("No hay items ocultos aquí. El ítem para la mazmorra %s está ", 
                   aldea_actual->mazmorra_asociada->nombre);
            
            if (aldea_actual->mazmorra_asociada->item_requerido->mazmorra_asociada != NULL) {
                printf("en la mazmorra %s.\n", 
                       aldea_actual->mazmorra_asociada->item_requerido->mazmorra_asociada->nombre);
            } else {
                printf("disponible para compra en esta aldea.\n");
            }
        }
        
    } else if (strcmp(comando, "maz") == 0) {
        // Ir a mazmorra asociada con posibilidad de perder vida
        printf("Viajando a la mazmorra %s...\n", aldea_actual->mazmorra_asociada->nombre);
        
        // Ganar $10 por el viaje
        dinero += 10;
        printf("Encontraste $10 en el camino. Dinero total: $%d\n", dinero);
        
        // 20% de probabilidad de perder vida
        if (rand() % 5 == 0) {
            vidas--;
            printf("¡Oh no! Perdiste una vida en el camino. Vidas restantes: %d\n", vidas);
        }
        
    } else if (strcmp(comando, "compr") == 0) {
        // Menú de compra mejorado
        int opcion;
        printf("\n--- Tienda de %s ---\n", aldea_actual->nombre);
        printf("1. Recuperar vida ($5)\n");
        printf("2. Ítem primera mazmorra ($25)\n");
        printf("3. Vida adicional ($100)\n");
        printf("Tu dinero: $%d\n", dinero);
        printf("Seleccione una opción (1-3): ");
        
        scanf("%d", &opcion);
        
        switch(opcion) {
            case 1: // Recuperar vida
                if (dinero >= 5) {
                    dinero -= 5;
                    if (vidas < 127) {
                        vidas++;
                        printf("Vida recuperada. Vidas ahora: %d\n", vidas);
                    } else {
                        printf("Ya tienes el máximo de vidas.\n");
                    }
                } else {
                    printf("Dinero insuficiente.\n");
                }
                break;
                
            case 2: // Ítem primera mazmorra
                if (aldea_actual == mundo_superior || aldea_actual == mundo_paralelo) {
                    if (dinero >= 25) {
                        dinero -= 25;
                        // Asignar el ítem requerido para la primera mazmorra
                        mundo_superior->mazmorra_asociada->item_requerido->encontrado = 1;
                        printf("¡Has comprado el %s!\n", 
                               mundo_superior->mazmorra_asociada->item_requerido->nombre);
                    } else {
                        printf("Dinero insuficiente.\n");
                    }
                } else {
                    printf("Este ítem solo se puede comprar en la primera aldea.\n");
                }
                break;
                
            case 3: // Vida adicional
                if (dinero >= 100) {
                    if (vidas < 127) {
                        dinero -= 100;
                        vidas++;
                        printf("¡Vida adicional adquirida! Máximo de vidas ahora: %d\n", vidas);
                    } else {
                        printf("Ya has alcanzado el máximo de 127 vidas.\n");
                    }
                } else {
                    printf("Dinero insuficiente. Necesitas $100.\n");
                }
                break;
                
            default:
                printf("Opción inválida.\n");
        }
        
    } else if (strcmp(comando, "trans") == 0) {
        // Transportar a mundo paralelo
        if (mundo_paralelo_desbloqueado) {
            Aldea* aldea_paralela = NULL;
            Aldea* temp = mundo_paralelo;
            int index = 0;
            
            // Encontrar la aldea correspondiente en el mundo paralelo
            Aldea* actual = mundo_superior;
            while (actual != aldea_actual && actual != NULL) {
                index++;
                actual = actual->siguiente;
            }
            
            actual = mundo_paralelo;
            for (int i = 0; i < index && actual != NULL; i++) {
                actual = actual->siguiente;
            }
            
            if (actual != NULL) {
                aldea_actual = actual;
                printf("Te has transportado a %s en el mundo paralelo.\n", aldea_actual->nombre);
            }
        } else {
            printf("El mundo paralelo no está desbloqueado aún.\n");
        }
        
    } else if (strcmp(comando, "ant") == 0) {
        // Ir a aldea anterior con consecuencias
        if (aldea_actual->anterior != NULL) {
            aldea_actual = aldea_actual->anterior;
            dinero += 10;
            printf("Has llegado a %s. Encontraste $10 en el camino. Dinero: $%d\n", 
                   aldea_actual->nombre, dinero);
            
            if (rand() % 5 == 0) { // 20% chance de perder vida
                vidas--;
                printf("¡Oh no! Perdiste una vida en el camino. Vidas restantes: %d\n", vidas);
            }
        } else {
            printf("No hay aldea anterior. ¡Aquí naciste!\n");
        }
        
    } else if (strcmp(comando, "sig") == 0) {
        // Ir a aldea siguiente con consecuencias
        if (aldea_actual->siguiente != NULL) {
            aldea_actual = aldea_actual->siguiente;
            dinero += 10;
            printf("Has llegado a %s. Encontraste $10 en el camino. Dinero: $%d\n", 
                   aldea_actual->nombre, dinero);
            
            if (rand() % 5 == 0) { // 20% chance de perder vida
                vidas--;
                printf("¡Oh no! Perdiste una vida en el camino. Vidas restantes: %d\n", vidas);
            }
        } else {
            printf("No hay aldea siguiente. ¡Este es el fin del camino!\n");
        }
        
    } else {
        printf("Comando no reconocido.\n");
    }
}

void comando_mazmorra(char* comando) {
    printf("Comando de mazmorra: %s\n", comando);
    // Implementar lógica específica de mazmorra
}

void ciclo_principal() {
    char comando[10];
    int aldeas_derrotadas = 0;
    int total_aldeas = 0;
    
    // Contar aldeas (para la condición de victoria)
    Aldea* temp = mundo_superior;
    while (temp != NULL) {
        total_aldeas++;
        temp = temp->siguiente;
    }

    // Mostrar descripción inicial
    mostrar_descripcion_aldea(aldea_actual);
    
    while (vidas > 0) {
        printf("\nEstás en: %s\n", aldea_actual->nombre);
        printf("Vidas: %d/%d | Dinero: $%d\n", vidas, 127, dinero);
        printf("> ");
        scanf("%9s", comando); // Limita la lectura a 9 caracteres
        
        if (esta_en_aldea()) {
            comando_aldea(comando);
        } else {
            comando_mazmorra(comando);
        }
        
        // Verificar condición de victoria
        temp = mundo_superior;
        aldeas_derrotadas = 0;
        while (temp != NULL) {
            if (temp->mazmorra_asociada->derrotada) {
                aldeas_derrotadas++;
            }
            temp = temp->siguiente;
        }
        
        if (aldeas_derrotadas >= total_aldeas) {
            printf("\n¡FELICITACIONES! Has derrotado todas las mazmorras.\n");
            break;
        }
    }
    
    if (vidas <= 0) {
        printf("\n=== GAME OVER ===\n");
    }
}
void liberar_memoria() {
    // Liberar memoria de las aldeas y mazmorras
    Aldea* actual = mundo_superior;
    while (actual != NULL) {
        Aldea* siguiente = actual->siguiente;
        if (actual->mazmorra_asociada != NULL) {
            free(actual->mazmorra_asociada);
        }
        free(actual);
        actual = siguiente;
    }
    
    // (Falta implementar la liberación del mundo paralelo y los items)
}