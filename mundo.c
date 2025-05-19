#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estructuras.h"
#include <stdbool.h>

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
    for (int i = 1; i <= n_aldeas; i++) {
        Item *nuevo = malloc(sizeof(Item));
        snprintf(nuevo->nombre, MAX_NOMBRE, "Ítem %d", i);
        nuevo->conseguido = 0;
        nuevo->usada_en = NULL;
        nuevo->sig = lista_items;
        lista_items = nuevo;
    }
    
    // 3. Crear ítems paralelos (1 (P) a n_aldeas (P))
    for (int i = 1; i <= n_aldeas; i++) {
        Item *nuevo = malloc(sizeof(Item));
        snprintf(nuevo->nombre, MAX_NOMBRE, "Ítem %d (P)", i);
        nuevo->conseguido = 0;
        nuevo->usada_en = NULL;
        nuevo->sig = lista_items;
        lista_items = nuevo;
    }
    
    item_actual = lista_items;

    // === CREAR MUNDO SUPERIOR ===
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
        snprintf(maz->nombre, MAX_NOMBRE, "Mazmorra de %s", bases[i % base_count]);
        maz->origen = aldea;
        maz->oculto = NULL;
        maz->requiere = NULL;
        maz->sig = NULL;

        aldea->mazmorra = maz;

        // Asignar ítem oculto a aldea (usar ítems normales)
        if (rand() % 2 == 0) {
            // Buscar un ítem normal no asignado (nombre sin "(P)")
            Item *iter = lista_items;
            while (iter) {
                if (iter->usada_en == NULL && strstr(iter->nombre, "(P)") == NULL && iter != tienda_principal) {
                    aldea->oculto = iter;
                    iter->usada_en = aldea; // Marcamos que está usado
                    break;
                }
                iter = iter->sig;
            }
        }

        // Asignar ítem oculto a mazmorra (usar ítems normales)
        if (rand() % 2 == 0) {
            Item *iter = lista_items;
            while (iter) {
                if (iter->usada_en == NULL && strstr(iter->nombre, "(P)") == NULL && iter != tienda_principal) {
                    maz->oculto = iter;
                    iter->usada_en = maz; // Marcamos que está usado
                    break;
                }
                iter = iter->sig;
            }
        }

        // Asignar ítem requerido para derrotar la mazmorra
        if (i == 0) {
            // Primera mazmorra requiere el ítem de tienda
            maz->requiere = tienda_principal;
            tienda_principal->usada_en = maz;
        } else {
            // Buscar un ítem normal no asignado
            Item *iter = lista_items;
            while (iter) {
                if (iter->usada_en == NULL && strstr(iter->nombre, "(P)") == NULL && iter != tienda_principal) {
                    maz->requiere = iter;
                    iter->usada_en = maz;
                    break;
                }
                iter = iter->sig;
            }
        }
    }

    // === CREAR MUNDO PARALELO ===
    Aldea *inicio_paralelo = NULL;
    Aldea *prev_paralelo = NULL;
    Aldea *tmp = inicio;

    while (tmp) {
        // Crear aldea paralela
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

        // Crear mazmorra paralela
        Mazmorra *maz_p = malloc(sizeof(Mazmorra));
        snprintf(maz_p->nombre, MAX_NOMBRE, "%s (P)", tmp->mazmorra->nombre);
        maz_p->origen = aldea_p;
        maz_p->oculto = NULL;
        maz_p->requiere = NULL;
        maz_p->sig = NULL;

        aldea_p->mazmorra = maz_p;

        // Asignar ítem requerido a la mazmorra paralela (usar ítems paralelos)
        Item *iter_p = lista_items;
        while (iter_p) {
            if (iter_p->usada_en == NULL && strstr(iter_p->nombre, "(P)") != NULL) {
                maz_p->requiere = iter_p;
                iter_p->usada_en = maz_p;
                break;
            }
            iter_p = iter_p->sig;
        }

        // Ítems ocultos en aldea o mazmorra paralela (usar ítems paralelos)
        if (rand() % 2 == 0) {
            Item *iter = lista_items;
            while (iter) {
                if (iter->usada_en == NULL && strstr(iter->nombre, "(P)") != NULL) {
                    aldea_p->oculto = iter;
                    iter->usada_en = aldea_p;
                    break;
                }
                iter = iter->sig;
            }
        }

        if (rand() % 2 == 0) {
            Item *iter = lista_items;
            while (iter) {
                if (iter->usada_en == NULL && strstr(iter->nombre, "(P)") != NULL) {
                    maz_p->oculto = iter;
                    iter->usada_en = maz_p;
                    break;
                }
                iter = iter->sig;
            }
        }

        tmp = tmp->sig;
    }

    // === Visualización básica ===
    printf("\n🌍 Mundo superior:\n");
    tmp = inicio;
    while (tmp) {
        printf("🔸 %s - Mazmorra: %s | Requiere: %s\n", 
               tmp->nombre, 
               tmp->mazmorra->nombre,
               tmp->mazmorra->requiere ? tmp->mazmorra->requiere->nombre : "Nada");
        tmp = tmp->sig;
    }

    printf("\n🌌 Mundo paralelo:\n");
    tmp = inicio_paralelo;
    while (tmp) {
        printf("🔹 %s - Mazmorra: %s | Requiere: %s\n", 
               tmp->nombre, 
               tmp->mazmorra->nombre,
               tmp->mazmorra->requiere ? tmp->mazmorra->requiere->nombre : "Nada");
        tmp = tmp->sig;
    }

    return inicio;
}

#define MAX_ELEMENTS 4
#define MAX_COMBINATION 10 // Máximo de elementos a combinar (para evitar desbordamientos)

const char *elements[MAX_ELEMENTS] = {"Agua", "Tierra", "Fuego", "Aire"};

// Función para generar combinaciones
void generarNombreAldea(int mundo_num, char *nombre, size_t buffer_size) {
    // Casos base (mundos 1-4)
    if (mundo_num <= 4) {
        snprintf(nombre, buffer_size, "%s", elements[mundo_num - 1]);
        return;
    }

    int combinacion = 2; // Empezamos con combinaciones de 2
    int mundo_base = 5;  // El mundo 5 es donde empiezan las combinaciones
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