#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"


// Función para jugar el juego
// Esta función maneja la lógica del juego, incluyendo la interacción con el usuario
void jugar(Aldea *inicio) {
    Aldea *aldea_actual = inicio;
    int vidas = 3;
    int monedas = 0;
    char comando[32];
    int desbloqueo_mundo_paralelo = 0;

    // Contamos mazmorras totales y no derrotadas
    int total_mazmorras = 0;
    int mazmorras_no_derrotadas = 0;
    Aldea *tmp = inicio;
    while (tmp) {
        total_mazmorras++;
        if (!tmp->mazmorra->derrotada) mazmorras_no_derrotadas++;
        if (tmp->vinculada) {
            total_mazmorras++;
            if (!tmp->vinculada->mazmorra->derrotada) mazmorras_no_derrotadas++;
        }
        tmp = tmp->sig;
    }

    // Mensaje de bienvenida
    printf("\n🌟 Bienvenido al universo de Yggdrasil.");
    printf("\n\n💎 Eres un guerrero destinado a cruzar los nueve mundos y enfrentar a sus guardianes.");
    printf("\n👹 Cuidado con los draugar y bestias que acechan entre reinos. Pueden quitarte vidas al desplazarte.");
    printf("\n⚔️  Solo los dignos pueden vencer a los guardianes de cada reino. Sin el artefacto sagrado, perderás vidas.");
    printf("\n🔍 Busca en cada reino reliquias de los dioses ocultas en los rincones más peligrosos.");
    printf("\n💼 Acude al herrero rúnico ('compr') para sanar o forjar artefactos únicos.");
    printf("\n🌌 Tras vencer a tu primer guardián, accederás a los mundos paralelos de los elfos y los muertos con 'trans'.");
    printf("\n🌿 Los Fragmentos de Yggdrasil son la energía vital del árbol del mundo. Úsalos para comerciar, curarte o obtener artefactos sagrados.");
    printf("\n\n¡Buena suerte, guerrero! Que los dioses te guíen en tu travesía.\n");

    // Mensaje de inicio
    printf("\n\n🎮 ¡Comienza tu aventura en %s!\n", aldea_actual->nombre);
    // Mostrar el mundo actual
    while (vidas > 0 && mazmorras_no_derrotadas > 0) {
        printf("\n📍 Estás en %s (%s) |  ❤️  %d | 🌿 %d | Mazmorras: %d/%d\n", 
               aldea_actual->nombre,
               aldea_actual->mundo == 0 ? "🌍 " : "🌌 ", 
               vidas, monedas,
               mazmorras_no_derrotadas, total_mazmorras);
        printf("Comandos disponibles: busq, maz, compr, sig, ant, trans\n> ");
        scanf("%s", comando);
            // Comando para buscar ítem oculto
        if (strcmp(comando, "busq") == 0) {
            if (aldea_actual->oculto && !aldea_actual->oculto->conseguido) {
                printf("🔎 Has encontrado el ítem oculto: %s\n", aldea_actual->oculto->nombre);
                aldea_actual->oculto->conseguido = 1;
            } else {
                printf("❌ No hay ningún artefacto rúnico. Un viejo sabio te dice que necesitas %s para derrotar esta mazmorra\n",
                       aldea_actual->mazmorra->requiere->nombre);
            }
        }
        // Comando para entrar a la mazmorra
        else if (strcmp(comando, "maz") == 0) {
            Mazmorra *m = aldea_actual->mazmorra;
            printf("🏰 Entrando a la mazmorra: %s...\n", m->nombre);

            int mazmorra_activa = 1;
            int busqueda_realizada = 0;

            while (mazmorra_activa && vidas > 0) {
                printf("\n 🏰  Mazmorra: %s | ❤️ %d | 🌿 %d\n", m->nombre, vidas, monedas);
                printf("Comandos: busq, atac, ant, sig\n> ");
                scanf("%s", comando);

                if (rand() % 3 == 0) {
                    vidas--;
                    printf("⚠️ Sufriste daño al realizar esta acción. -1 vida\n");
                    if (vidas <= 0) break;
                }

                if (strcmp(comando, "busq") == 0) {
                    if (!busqueda_realizada) {
                        if (m->oculto && !m->oculto->conseguido) {
                            printf("🔎 Encontraste el ítem oculto: %s\n", m->oculto->nombre);
                            m->oculto->conseguido = 1;
                        } else {
                            printf("🌿 No hay ítem oculto. Ganaste Fragmentos10\n");
                            monedas += 10;
                        }
                        busqueda_realizada = 1;
                    } else {
                        printf("🚫 Ya buscaste aquí. Pierdes una vida.\n");
                        vidas--;
                    }
                }
                // Comando para atacar al Dios de la mazmorra
                else if (strcmp(comando, "atac") == 0) {
                    if (m->requiere && m->requiere->conseguido) {
                        printf("⚔️ ¡Has derrotado la mazmorra!\n");
                        if (!m->derrotada) {
                            m->derrotada = 1;
                            mazmorras_no_derrotadas--;
                            printf("Mazmorras restantes: %d/%d\n", mazmorras_no_derrotadas, total_mazmorras);
                        }
                        desbloqueo_mundo_paralelo = 1;
                        mazmorra_activa = 0;

                        if (mazmorras_no_derrotadas == 0) {
                            printf("\n🎉 ¡VICTORIA! Has recorrido los nueve mundos y derrotado a sus guardianes. Odín te recibe en el Valhalla. 🎉\n");
                            return;  // Salir de la función, terminando el juego con victoria
                        }
                    } else {
                        printf("🛡️ No tienes el ítem requerido (%s). Pierdes una vida.\n", m->requiere->nombre);
                        vidas--;
                        if (vidas <= 0) break;
                    }
                }
                // Comando para regresar a la aldea
                else if (strcmp(comando, "ant") == 0) {
                    aldea_actual = m->origen;
                    printf("🔙 Regresaste a %s.\n", aldea_actual->nombre);
                    mazmorra_activa = 0;
                }
                // Comando para ir a la siguiente aldea desde la mazmorra
                else if (strcmp(comando, "sig") == 0) {
                    if (m->origen->sig) {
                        aldea_actual = m->origen->sig;
                        printf("➡️ Fuiste a %s en el camino.\n", aldea_actual->nombre);
                        monedas += 10;
                        mazmorra_activa = 0;
                    } else {
                        printf("🚧 No hay aldea siguiente desde esta mazmorra.\n");
                    }
                }

                else {
                    printf("❓ Comando inválido dentro de la mazmorra.\n");
                }
            }
        }
        // Comando para comprar ítem
        else if (strcmp(comando, "compr") == 0) {
            printf("🛒 Tienda:\n1. Beber sangre (5🌿)\n2. Item de tienda (25🌿)\n> ");
            int opcion;
            scanf("%d", &opcion);
            switch (opcion) {
                case 1:
                    if (monedas >= 5 && vidas < 127) {
                    monedas -= 5; vidas++;
                    printf("❤️ Recuperaste una vida.\n");
                    } else printf("❌ No puedes comprar eso.\n");
                break;
                case 2:
                    if (monedas >= 25) {
                    // Buscar el ítem de tienda principal en la lista de ítems
                    Aldea *tmp = inicio;
                    while (tmp) {
                        if (tmp->mazmorra->requiere && strstr(tmp->mazmorra->requiere->nombre, "tienda") != NULL) {
                            tmp->mazmorra->requiere->conseguido = 1;
                            monedas -= 25;
                            printf("✅ Compraste el item de tienda.\n");
                            break;
                        }
                        tmp = tmp->sig;
                }
                if (!tmp) {
                    printf("Ese ítem no está disponible aquí.\n");
                }
            } else printf("❌ Dinero insuficiente.\n");
            break;
        case 3:
        default: printf("⛔ Compra cancelada.\n");
    }
}
        // Comando para ir a la siguiente aldea pero desde la aldea
        else if (strcmp(comando, "sig") == 0) {
            if (aldea_actual->sig) {
                printf("🌿 Te has encontrado 10 Fragmentos de camino a %s\n", aldea_actual->sig->nombre);
                monedas += 10;
                aldea_actual = aldea_actual->sig;
                if (rand() % 2 == 0) {
                    vidas--;
                    printf("⚠️ Perdiste una vida en el camino.\n");
                }
            } else {
                printf("🚧 No hay aldea siguiente.\n");
            }
        }
        // Comando para retroceder a la aldea anterior
        else if (strcmp(comando, "ant") == 0) {
            if (aldea_actual->ant) {
                printf("🌿 Te has encontrado 10 Fragmentos de camino a %s\n", aldea_actual->ant->nombre);
                monedas += 10;
                aldea_actual = aldea_actual->ant;
                if (rand() % 2 == 0) {
                    vidas--;
                    printf("⚠️ Perdiste una vida en el camino.\n");
                }
            } else {
                printf("🍼 No puedes retroceder más.\n");
            }
        }
        // Comando para ir al mundo paralelo
        else if (strcmp(comando, "trans") == 0) {
            if (desbloqueo_mundo_paralelo && aldea_actual->vinculada) {
                aldea_actual = aldea_actual->vinculada;
                printf(" 🌐 Te transportaste a %s.\n", aldea_actual->nombre);
            } else printf("🚫 No puedes transportarte aún.\n");
        }

        else {
            printf("❓ Comando no reconocido.\n");
        }
    }

    // Fuera del bucle principal, verificar por qué terminó el juego e indicarlo al jugador
    if (vidas <= 0) {
        printf("\n💀 Has caído en el campo de batalla. Hel te reclama. Tu alma no verá el Valhalla.\n");
    } else if (mazmorras_no_derrotadas == 0) {
        printf("\n🎉 ¡VICTORIA! Has recorrido los nueve mundos y derrotado a sus guardianes. Odín te recibe en el Valhalla. 🎉\n");
    }
}