#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estructuras.h"
#include <stdbool.h>

 bool itemAccesible(Item *item, Aldea *mundo, Aldea *mundo_paralelo);
Aldea *crearMundo(int n_aldeas) {
   
    const char *bases[] = {"Agua", "Tierra", "Fuego", "Aire"};
    int base_count = sizeof(bases) / sizeof(bases[0]);

    Aldea *inicio = NULL, *prev = NULL;
    Item *lista_items = NULL;
    Item *item_actual = NULL;

    
    // === CREACIÓN DE ÍTEMS ===
    // 1. Crear ítem de tienda principal
    Item *tienda_principal = malloc(sizeof(Item));
    strcpy(tienda_principal->nombre, "Ítem de tienda");
    tienda_principal->conseguido = 0;
    tienda_principal->usada_en = NULL;
    tienda_principal->sig = NULL;
    lista_items = tienda_principal;

    // 2. Crear ítems normales (1 a n_aldeas)
    Item **items_normales = malloc(n_aldeas * sizeof(Item*));
    for (int i = 0; i < n_aldeas; i++) {
        items_normales[i] = malloc(sizeof(Item));
        snprintf(items_normales[i]->nombre, MAX_NOMBRE, "Ítem %d", i+1);
        items_normales[i]->conseguido = 0;
        items_normales[i]->usada_en = NULL;
        items_normales[i]->sig = lista_items;
        lista_items = items_normales[i];
    }
    
    // 3. Crear ítems paralelos
    Item **items_paralelos = malloc(n_aldeas * sizeof(Item*));
    for (int i = 0; i < n_aldeas; i++) {
        items_paralelos[i] = malloc(sizeof(Item));
        snprintf(items_paralelos[i]->nombre, MAX_NOMBRE, "Ítem %d (P)", i+1);
        items_paralelos[i]->conseguido = 0;
        items_paralelos[i]->usada_en = NULL;
        items_paralelos[i]->sig = lista_items;
        lista_items = items_paralelos[i];
    }

    // === CREAR MUNDO SUPERIOR ===
    int *asignados_requiere = calloc(n_aldeas, sizeof(int));
    int *asignados_ocultos = calloc(n_aldeas, sizeof(int));
    
    // Lista de índices disponibles para asignación
    int *disponibles_requiere = malloc(n_aldeas * sizeof(int));
    int *disponibles_ocultos = malloc(n_aldeas * sizeof(int));
    for (int i = 0; i < n_aldeas; i++) {
        disponibles_requiere[i] = i;
        disponibles_ocultos[i] = i;
    }
    int disponibles_requiere_count = n_aldeas;
    int disponibles_ocultos_count = n_aldeas;
    
    for (int i = 0; i < n_aldeas; i++) {
        Aldea *aldea = malloc(sizeof(Aldea));
        generarNombreAldea(i + 1, aldea->nombre, MAX_NOMBRE);
        aldea->oculto = NULL;
        aldea->sig = NULL;
        aldea->ant = prev;
        aldea->vinculada = NULL;
        aldea->mundo = 0;

        if (prev) prev->sig = aldea;
        if (!inicio) inicio = aldea;
        prev = aldea;

        Mazmorra *maz = malloc(sizeof(Mazmorra));
        snprintf(maz->nombre, MAX_NOMBRE, "%s", aldea->nombre);  
        maz->origen = aldea;
        maz->oculto = NULL;
        maz->requiere = NULL;
        maz->sig = NULL;
        maz->derrotada = 0;

        aldea->mazmorra = maz;

        // Asignar ítem requerido
        if (i == 0) {
            maz->requiere = tienda_principal;
            tienda_principal->usada_en = maz;
        } else if (disponibles_requiere_count > 0) {
            int idx = rand() % disponibles_requiere_count;
            int item_idx = disponibles_requiere[idx];
            
            maz->requiere = items_normales[item_idx];
            items_normales[item_idx]->usada_en = maz;
            
            // Eliminar este índice de los disponibles
            disponibles_requiere[idx] = disponibles_requiere[disponibles_requiere_count-1];
            disponibles_requiere_count--;
        }

        // Asignar ítem oculto a aldea (50% de probabilidad)
        if (rand() % 2 == 0 && disponibles_ocultos_count > 0) {
            int idx = rand() % disponibles_ocultos_count;
            int item_idx = disponibles_ocultos[idx];
            
            aldea->oculto = items_normales[item_idx];
            items_normales[item_idx]->usada_en = aldea;
            
            // Eliminar este índice de los disponibles
            disponibles_ocultos[idx] = disponibles_ocultos[disponibles_ocultos_count-1];
            disponibles_ocultos_count--;
        }

        // Asignar ítem oculto a mazmorra (50% de probabilidad)
        if (rand() % 2 == 0 && disponibles_ocultos_count > 0) {
            int idx = rand() % disponibles_ocultos_count;
            int item_idx = disponibles_ocultos[idx];
            
            maz->oculto = items_normales[item_idx];
            items_normales[item_idx]->usada_en = maz;
            
            // Eliminar este índice de los disponibles
            disponibles_ocultos[idx] = disponibles_ocultos[disponibles_ocultos_count-1];
            disponibles_ocultos_count--;
        }
    }

    // === CREAR MUNDO PARALELO ===
    Aldea *inicio_paralelo = NULL;
    Aldea *prev_paralelo = NULL;
    Aldea *tmp = inicio;
    int paralelo_index = 0;

    // Reiniciar listas de disponibles para el mundo paralelo
    for (int i = 0; i < n_aldeas; i++) {
        disponibles_requiere[i] = i;
        disponibles_ocultos[i] = i;
    }
    disponibles_requiere_count = n_aldeas;
    disponibles_ocultos_count = n_aldeas;
    
    while (tmp) {
        Aldea *aldea_p = malloc(sizeof(Aldea));
        snprintf(aldea_p->nombre, MAX_NOMBRE, "%s (P)", tmp->nombre);
        aldea_p->oculto = NULL;
        aldea_p->sig = NULL;
        aldea_p->ant = prev_paralelo;
        aldea_p->vinculada = tmp;
        aldea_p->mundo = 1;

        tmp->vinculada = aldea_p;

        if (prev_paralelo) prev_paralelo->sig = aldea_p;
        if (!inicio_paralelo) inicio_paralelo = aldea_p;
        prev_paralelo = aldea_p;

        Mazmorra *maz_p = malloc(sizeof(Mazmorra));
        snprintf(maz_p->nombre, MAX_NOMBRE, "%s", aldea_p->nombre); 
        maz_p->origen = aldea_p;
        maz_p->oculto = NULL;
        maz_p->requiere = NULL;
        maz_p->sig = NULL;
        maz_p->derrotada = 0;

        aldea_p->mazmorra = maz_p;

        // Asignar ítem requerido
        if (disponibles_requiere_count > 0) {
            int idx = rand() % disponibles_requiere_count;
            int item_idx = disponibles_requiere[idx];
            
            maz_p->requiere = items_paralelos[item_idx];
            items_paralelos[item_idx]->usada_en = maz_p;
            
            // Eliminar este índice de los disponibles
            disponibles_requiere[idx] = disponibles_requiere[disponibles_requiere_count-1];
            disponibles_requiere_count--;
        }

        // Asignar ítem oculto a aldea paralela (50% de probabilidad)
        if (rand() % 2 == 0 && disponibles_ocultos_count > 0) {
            int idx = rand() % disponibles_ocultos_count;
            int item_idx = disponibles_ocultos[idx];
            
            aldea_p->oculto = items_paralelos[item_idx];
            items_paralelos[item_idx]->usada_en = aldea_p;
            
            // Eliminar este índice de los disponibles
            disponibles_ocultos[idx] = disponibles_ocultos[disponibles_ocultos_count-1];
            disponibles_ocultos_count--;
        }

        // Asignar ítem oculto a mazmorra paralela (50% de probabilidad)
        if (rand() % 2 == 0 && disponibles_ocultos_count > 0) {
            int idx = rand() % disponibles_ocultos_count;
            int item_idx = disponibles_ocultos[idx];
            
            maz_p->oculto = items_paralelos[item_idx];
            items_paralelos[item_idx]->usada_en = maz_p;
            
            // Eliminar este índice de los disponibles
            disponibles_ocultos[idx] = disponibles_ocultos[disponibles_ocultos_count-1];
            disponibles_ocultos_count--;
        }

        tmp = tmp->sig;
        paralelo_index++;
    }

    // Liberar memoria
    free(asignados_requiere);
    free(asignados_ocultos);
    free(disponibles_requiere);
    free(disponibles_ocultos);


    // === VERIFICACIÓN DE ÍTEMS REQUERIDOS ===
    // Verificar que todos los ítems paralelos estén asignados
    for (int i = 0; i < n_aldeas; i++) {
        if (items_paralelos[i]->usada_en == NULL) {
            // Buscar una mazmorra paralela sin ítem oculto
            Aldea *tmp_aldea = inicio_paralelo;
            while (tmp_aldea) {
                if (tmp_aldea->mazmorra->oculto == NULL) {
                    tmp_aldea->mazmorra->oculto = items_paralelos[i];
                    items_paralelos[i]->usada_en = tmp_aldea->mazmorra;
                    break;
                }
                tmp_aldea = tmp_aldea->sig;
            }
            
            // Si no se encontró mazmorra, buscar aldea sin ítem oculto
            if (items_paralelos[i]->usada_en == NULL) {
                tmp_aldea = inicio_paralelo;
                while (tmp_aldea) {
                    if (tmp_aldea->oculto == NULL) {
                        tmp_aldea->oculto = items_paralelos[i];
                        items_paralelos[i]->usada_en = tmp_aldea;
                        break;
                    }
                    tmp_aldea = tmp_aldea->sig;
                }
            }
        }
    }

    // Verificar que todos los ítems normales estén asignados (excepto el de tienda)
    for (int i = 0; i < n_aldeas; i++) {
        if (items_normales[i]->usada_en == NULL && items_normales[i] != tienda_principal) {
            // Buscar una mazmorra normal sin ítem oculto
            Aldea *tmp_aldea = inicio;
            while (tmp_aldea) {
                if (tmp_aldea->mazmorra->oculto == NULL) {
                    tmp_aldea->mazmorra->oculto = items_normales[i];
                    items_normales[i]->usada_en = tmp_aldea->mazmorra;
                    break;
                }
                tmp_aldea = tmp_aldea->sig;
            }
            
            // Si no se encontró mazmorra, buscar aldea sin ítem oculto
            if (items_normales[i]->usada_en == NULL) {
                tmp_aldea = inicio;
                while (tmp_aldea) {
                    if (tmp_aldea->oculto == NULL) {
                        tmp_aldea->oculto = items_normales[i];
                        items_normales[i]->usada_en = tmp_aldea;
                        break;
                    }
                    tmp_aldea = tmp_aldea->sig;
                }
            }
        }
    }

    // === VERIFICACIÓN FINAL DE ACCESIBILIDAD ===
    // Asegurar que todos los ítems requeridos sean accesibles
    verificarAccesibilidad(inicio, inicio_paralelo, n_aldeas, items_normales, items_paralelos, tienda_principal);

    // === Visualización básica ===
    printf("\n🌍 Mundo superior:\n");
    tmp = inicio;
    while (tmp) {
        printf("🔸 %s - Mazmorra: %s | Requiere: %s \n", 
               tmp->nombre, 
               tmp->mazmorra->nombre,
               tmp->mazmorra->requiere ? tmp->mazmorra->requiere->nombre : "Nada",
               tmp->oculto ? tmp->oculto->nombre : "Nada",
               tmp->mazmorra->oculto ? tmp->mazmorra->oculto->nombre : "Nada");
        tmp = tmp->sig;
    }

    printf("\n🌌 Mundo paralelo:\n");
    tmp = inicio_paralelo;
    while (tmp) {
        printf("🔹 %s - Mazmorra: %s | Requiere: %s \n", 
               tmp->nombre, 
               tmp->mazmorra->nombre,
               tmp->mazmorra->requiere ? tmp->mazmorra->requiere->nombre : "Nada",
               tmp->oculto ? tmp->oculto->nombre : "Nada",
               tmp->mazmorra->oculto ? tmp->mazmorra->oculto->nombre : "Nada");
        tmp = tmp->sig;
    }

    return inicio;
}
// Función auxiliar para verificar si todos los ítems están asignados
int todosAsignados(int *asignados, int n) {
    for (int i = 0; i < n; i++) {
        if (!asignados[i]) return 0;
    }
    return 1;
}

void verificarAccesibilidad(Aldea *mundo, Aldea *mundo_paralelo, int n_aldeas, 
                           Item **items_normales, Item **items_paralelos, Item *tienda_principal) {
    // Verificar ítems normales
    for (int i = 0; i < n_aldeas; i++) {
        if (items_normales[i]->usada_en == NULL && items_normales[i] != tienda_principal) {
            // Asignar a una aldea aleatoria del mundo normal
            int aldea_idx = rand() % n_aldeas;
            Aldea *tmp = mundo;
            for (int j = 0; j < aldea_idx; j++) tmp = tmp->sig;
            
            if (tmp->oculto == NULL) {
                tmp->oculto = items_normales[i];
                items_normales[i]->usada_en = tmp;
            } else if (tmp->mazmorra->oculto == NULL) {
                tmp->mazmorra->oculto = items_normales[i];
                items_normales[i]->usada_en = tmp->mazmorra;
            }
        }
    }

    // Verificar ítems paralelos
    for (int i = 0; i < n_aldeas; i++) {
        if (items_paralelos[i]->usada_en == NULL) {
            // Asignar a una aldea aleatoria del mundo paralelo
            int aldea_idx = rand() % n_aldeas;
            Aldea *tmp = mundo_paralelo;
            for (int j = 0; j < aldea_idx; j++) tmp = tmp->sig;
            
            if (tmp->oculto == NULL) {
                tmp->oculto = items_paralelos[i];
                items_paralelos[i]->usada_en = tmp;
            } else if (tmp->mazmorra->oculto == NULL) {
                tmp->mazmorra->oculto = items_paralelos[i];
                items_paralelos[i]->usada_en = tmp->mazmorra;
            }
        }
    }

    // Verificación adicional para ítems requeridos
    verificarRequeridos(mundo, mundo_paralelo, n_aldeas);
}

// Función para verificar que todos los ítems requeridos sean accesibles
void verificarRequeridos(Aldea *mundo, Aldea *mundo_paralelo, int n_aldeas) {
    // Verificar mundo normal
    Aldea *tmp = mundo;
    while (tmp) {
        if (tmp->mazmorra->requiere != NULL) {
            if (!itemAccesible(tmp->mazmorra->requiere, mundo, mundo_paralelo)) {
                // Hacer accesible el ítem requerido
                hacerAccesible(tmp->mazmorra->requiere, mundo, mundo_paralelo);
            }
        }
        tmp = tmp->sig;
    }

    // Verificar mundo paralelo
    tmp = mundo_paralelo;
    while (tmp) {
        if (tmp->mazmorra->requiere != NULL) {
            if (!itemAccesible(tmp->mazmorra->requiere, mundo, mundo_paralelo)) {
                // Hacer accesible el ítem requerido
                hacerAccesible(tmp->mazmorra->requiere, mundo, mundo_paralelo);
            }
        }
        tmp = tmp->sig;
    }
}

// Función para verificar si un ítem es accesible
bool itemAccesible(Item *item, Aldea *mundo, Aldea *mundo_paralelo) {
    Aldea *tmp = mundo;
    while (tmp) {
        if (tmp->oculto == item || tmp->mazmorra->oculto == item) {
            return true;
        }
        tmp = tmp->sig;
    }

    tmp = mundo_paralelo;
    while (tmp) {
        if (tmp->oculto == item || tmp->mazmorra->oculto == item) {
            return true;
        }
        tmp = tmp->sig;
    }

    return false;
}

// Función para hacer un ítem accesible
void hacerAccesible(Item *item, Aldea *mundo, Aldea *mundo_paralelo) {
    // Primero intentar en el mundo paralelo
    Aldea *tmp = mundo_paralelo;
    while (tmp) {
        if (tmp->oculto == NULL) {
            tmp->oculto = item;
            item->usada_en = tmp;
            return;
        }
        if (tmp->mazmorra->oculto == NULL) {
            tmp->mazmorra->oculto = item;
            item->usada_en = tmp->mazmorra;
            return;
        }
        tmp = tmp->sig;
    }

    // Si no hay espacio en el mundo paralelo, intentar en el mundo normal
    tmp = mundo;
    while (tmp) {
        if (tmp->oculto == NULL) {
            tmp->oculto = item;
            item->usada_en = tmp;
            return;
        }
        if (tmp->mazmorra->oculto == NULL) {
            tmp->mazmorra->oculto = item;
            item->usada_en = tmp->mazmorra;
            return;
        }
        tmp = tmp->sig;
    }
}
#define MAX_ELEMENTS 9
#define MAX_COMBINATION 10 // Máximo de elementos a combinar (para evitar desbordamientos)

const char *elements[MAX_ELEMENTS] = {"Asgard", "Midgard", "Vanaheimr", "Jotunheim", 
                                      "Alfheim", "Nilfheim", "Svartalfheim", "Muspelheim","helheim"};

// Función para generar combinaciones
void generarNombreAldea(int mundo_num, char *nombre, size_t buffer_size) {
    // Casos base (mundos 1-4)
    if (mundo_num <= 9) {
        snprintf(nombre, buffer_size, "%s", elements[mundo_num - 1]);
        return;
    }

    int combinacion = 2; // Empezamos con combinaciones de 2
    int mundo_base = 10;  // El mundo 5 es donde empiezan las combinaciones
    bool nombre_generado = false;

    // Generar combinaciones sin repetición
    while (!nombre_generado && combinacion <= MAX_ELEMENTS) {
        // Calcular cuántas combinaciones posibles hay para este nivel
        int combinaciones_posibles = combinacionesSinRepeticion(MAX_ELEMENTS, combinacion);
        
        if (mundo_num < mundo_base + combinaciones_posibles) {
            generarCombinacionUnica(mundo_num - mundo_base, combinacion, nombre, buffer_size);
            nombre_generado = true;
        } else {
            mundo_base += combinaciones_posibles;
            combinacion++;
        }
    }

    // Si se agotan las combinaciones únicas, usar repeticiones
    if (!nombre_generado) {
        int elemento_base = (mundo_num - 1) % MAX_ELEMENTS;
        int repeticiones = ((mundo_num - 1) / MAX_ELEMENTS) + 1;
        
        // Construir nombre con repeticiones
        char temp[256] = "";
        for (int i = 0; i < repeticiones; i++) {
            if (i > 0) {
                strncat(temp, "-", sizeof(temp) - strlen(temp) - 1);
            }
            strncat(temp, elements[elemento_base], sizeof(temp) - strlen(temp) - 1);
        }
        snprintf(nombre, buffer_size, "%s", temp);
    }
}

// Función auxiliar para generar combinaciones únicas (sin repetición de elementos)
void generarCombinacionUnica(int index, int combinacion, char *nombre, size_t buffer_size) {
    char temp[256] = "";
    int elementos[MAX_ELEMENTS] = {0};
    int pos = 0;
    
    // Convertir el índice a combinación única
    for (int i = 0; i < combinacion; i++) {
        while (1) {
            if (elementos[pos % MAX_ELEMENTS] == 0) {
                if (index >= combinacionesSinRepeticion(MAX_ELEMENTS - pos - 1, combinacion - i - 1)) {
                    index -= combinacionesSinRepeticion(MAX_ELEMENTS - pos - 1, combinacion - i - 1);
                    pos++;
                } else {
                    break;
                }
            } else {
                pos++;
            }
        }
        elementos[pos % MAX_ELEMENTS] = 1;
        
        if (i > 0) {
            strncat(temp, "-", sizeof(temp) - strlen(temp) - 1);
        }
        strncat(temp, elements[pos % MAX_ELEMENTS], sizeof(temp) - strlen(temp) - 1);
        pos++;
    }
    
    snprintf(nombre, buffer_size, "%s", temp);
}

// Función para calcular combinaciones sin repetición (n choose k)
int combinacionesSinRepeticion(int n, int k) {
    if (k == 0 || k == n) return 1;
    if (k > n) return 0;
    return combinacionesSinRepeticion(n - 1, k - 1) + combinacionesSinRepeticion(n - 1, k);
}