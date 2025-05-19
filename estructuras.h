#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#define MAX_NOMBRE 64
#define MAX_ITEMS 32

typedef struct Mazmorra Mazmorra;
typedef struct Aldea Aldea;

typedef struct Item {
    char nombre[MAX_NOMBRE];
    int conseguido;
    Mazmorra *usada_en;
    struct Item *sig;
} Item;

struct Mazmorra {
    char nombre[MAX_NOMBRE];
    Item *requiere;
    Item *oculto;
    Aldea *origen;
    struct Mazmorra *sig;
    int derrotada;
};

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
