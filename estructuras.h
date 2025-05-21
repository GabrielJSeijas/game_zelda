#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#define MAX_NOMBRE 80
#define MAX_ITEMS 32
// Definición de estructuras para el juego listas enlazadas
typedef struct Mazmorra Mazmorra;
typedef struct Aldea Aldea;
// Definición de estructuras para los ítems
typedef struct Item {
    char nombre[MAX_NOMBRE];
    int conseguido;
    Mazmorra *usada_en;
    struct Item *sig;
} Item;
// Definición de estructuras para las mazmorras
struct Mazmorra {
    char nombre[MAX_NOMBRE];
    Item *requiere;
    Item *oculto;
    Aldea *origen;
    struct Mazmorra *sig;
    int derrotada;
};
// Definición de estructuras para las aldeas
struct Aldea {
    char nombre[MAX_NOMBRE];
    Item *oculto;
    Mazmorra *mazmorra;
    Aldea *sig;
    Aldea *ant;
    Aldea *vinculada;
    int mundo;
};

#endif
