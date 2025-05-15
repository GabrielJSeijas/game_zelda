#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estructuras.h"

Aldea *crearMundo(int n_aldeas) {
    const char *bases[] = {"Agua", "Tierra", "Fuego", "Aire"};
    int base_count = sizeof(bases) / sizeof(bases[0]);

    Aldea *inicio = NULL, *prev = NULL;
    Item *lista_items = NULL;
    Item *item_actual = NULL;
    int total_items = n_aldeas * 2; // para ambos mundos

    // Crear lista de ítems
    for (int i = 0; i < total_items; i++) {
        Item *nuevo = malloc(sizeof(Item));
        snprintf(nuevo->nombre, MAX_NOMBRE, "Ítem-%d", i + 1);
        nuevo->conseguido = 0;
        nuevo->usada_en = NULL;
        nuevo->sig = lista_items;
        lista_items = nuevo;
    }

    item_actual = lista_items;

    // === CREAR MUNDO SUPERIOR ===
    for (int i = 0; i < n_aldeas; i++) {
        Aldea *aldea = malloc(sizeof(Aldea));
        snprintf(aldea->nombre, MAX_NOMBRE, "Aldea de %s", bases[i % base_count]);
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

        // Asignar ítem oculto a aldea
        if (rand() % 2 == 0 && item_actual) {
            aldea->oculto = item_actual;
            item_actual = item_actual->sig;
        }

        // Asignar ítem oculto a mazmorra
        if (rand() % 2 == 0 && item_actual) {
            maz->oculto = item_actual;
            item_actual = item_actual->sig;
        }

        // Asignar ítem requerido para derrotarla
        if (i == 0) {
            Item *tienda = malloc(sizeof(Item));
            strcpy(tienda->nombre, "Ítem de tienda");
            tienda->conseguido = 0;
            tienda->usada_en = maz;
            tienda->sig = lista_items;
            lista_items = tienda;
            maz->requiere = tienda;
        } else {
            Item *iter = lista_items;
            while (iter) {
                if (iter->usada_en == NULL) {
                    iter->usada_en = maz;
                    maz->requiere = iter;
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

        // Asignar ítem requerido a la mazmorra paralela
        Item *iter_p = lista_items;
        while (iter_p) {
            if (iter_p->usada_en == NULL) {
                iter_p->usada_en = maz_p;
                snprintf(iter_p->nombre, MAX_NOMBRE, "%s (P)", iter_p->nombre);
                maz_p->requiere = iter_p;
                break;
            }
            iter_p = iter_p->sig;
        }

        // Ítems ocultos en aldea o mazmorra paralela
        if (rand() % 2 == 0 && item_actual) {
            aldea_p->oculto = item_actual;
            item_actual = item_actual->sig;
        }

        if (rand() % 2 == 0 && item_actual) {
            maz_p->oculto = item_actual;
            item_actual = item_actual->sig;
        }

        tmp = tmp->sig;
    }

    // === Visualización básica ===
    printf("\n🌍 Mundo superior:\n");
    tmp = inicio;
    while (tmp) {
        printf("🔸 %s - Mazmorra: %s\n", tmp->nombre, tmp->mazmorra->nombre);
        tmp = tmp->sig;
    }

    printf("\n🌌 Mundo paralelo:\n");
    tmp = inicio_paralelo;
    while (tmp) {
        printf("🔹 %s - Mazmorra: %s\n", tmp->nombre, tmp->mazmorra->nombre);
        tmp = tmp->sig;
    }

    return inicio;
}
