#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"

void jugar(Aldea *inicio) {
    Aldea *aldea_actual = inicio;
    int vidas = 3;
    int monedas = 0;
    char comando[32];
    int desbloqueo_mundo_paralelo = 0;

    // Contar mazmorras totales y no derrotadas
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

    printf("\n🎮 ¡Comienza tu aventura en %s! Mazmorras restantes: %d/%d\n", 
           aldea_actual->nombre, mazmorras_no_derrotadas, total_mazmorras);
    printf("\n🌟 Bienvenido al mundo de Link, señor Aang. Te deseo buena suerte para escapar con vida.\n");
printf("👹 ¡Cuidado! Pueden haber mounstros de camino de una aldea a otra. Sé cuidadoso con tus vidas.\n");
printf("⚔️ Ten cuidado al intentar derrotar al jefe de una mazmorra sin el ítem requerido - perderás vidas.\n");
printf("🔍 Usa 'busq' en aldeas y mazmorras para encontrar ítems ocultos que te ayudarán en tu aventura.\n");
printf("💼 Visita la tienda ('compr') para curarte o conseguir el ítem especial cuando estés en la aldea inicial.\n");
printf("🌌 Después de derrotar tu primera mazmorra, podrás transportarte al mundo paralelo con 'trans'.\n");

    while (vidas > 0 && mazmorras_no_derrotadas > 0) {
        printf("\n📍 Estás en %s (%s) | ❤️ %d | 💰 %d | Mazmorras: %d/%d\n", 
               aldea_actual->nombre,
               aldea_actual->mundo == 0 ? "🌍" : "🌌", 
               vidas, monedas,
               mazmorras_no_derrotadas, total_mazmorras);
        printf("Comandos disponibles: busq, maz, compr, sig, ant, trans\n> ");
        scanf("%s", comando);

        if (strcmp(comando, "busq") == 0) {
            if (aldea_actual->oculto && !aldea_actual->oculto->conseguido) {
                printf("🔎 Has encontrado el ítem oculto: %s\n", aldea_actual->oculto->nombre);
                aldea_actual->oculto->conseguido = 1;
            } else {
                printf("❌ No hay ítem oculto. Te dicen que el ítem que derrota esta mazmorra es: %s\n",
                       aldea_actual->mazmorra->requiere->nombre);
            }
        }

        else if (strcmp(comando, "maz") == 0) {
            Mazmorra *m = aldea_actual->mazmorra;
            printf("🏰 Entrando a la mazmorra: %s...\n", m->nombre);

            int mazmorra_activa = 1;
            int busqueda_realizada = 0;

            while (mazmorra_activa && vidas > 0) {
                printf("\n🏰 Mazmorra: %s | ❤️ %d | 💰 %d\n", m->nombre, vidas, monedas);
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
                            printf("💰 No hay ítem oculto. Ganaste $10\n");
                            monedas += 10;
                        }
                        busqueda_realizada = 1;
                    } else {
                        printf("🚫 Ya buscaste aquí. Pierdes una vida.\n");
                        vidas--;
                    }
                }

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
                            printf("\n🎉 ¡FELICIDADES! ¡Has derrotado todas las mazmorras y ganado el juego! 🎉\n");
                            return;  // Salir de la función, terminando el juego con victoria
                        }
                    } else {
                        printf("🛡️ No tienes el ítem requerido (%s). Pierdes una vida.\n", m->requiere->nombre);
                        vidas--;
                        if (vidas <= 0) break;
                    }
                }

                else if (strcmp(comando, "ant") == 0) {
                    aldea_actual = m->origen;
                    printf("🔙 Regresaste a %s.\n", aldea_actual->nombre);
                    mazmorra_activa = 0;
                }

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

        else if (strcmp(comando, "compr") == 0) {
            printf("🛒 Tienda:\n1. Curar (5$)\n2. Ítem de tienda (25$)\n3. Vida extra (100$)\n> ");
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
                        monedas -= 25;
                        Item *it = aldea_actual->mazmorra->requiere;
                        if (strcmp(it->nombre, "Ítem de tienda") == 0) {
                            it->conseguido = 1;
                            printf("✅ Compraste el ítem de tienda.\n");
                        } else printf("Ese ítem no está disponible aquí.\n");
                    } else printf("❌ Dinero insuficiente.\n");
                    break;
                case 3:
                    if (monedas >= 100 && vidas < 127) {
                        monedas -= 100; vidas++;
                        printf("💖 Vida adicional adquirida.\n");
                    } else printf("❌ No puedes comprar eso.\n");
                    break;
                default: printf("⛔ Compra cancelada.\n");
            }
        }

        else if (strcmp(comando, "sig") == 0) {
            if (aldea_actual->sig) {
                printf("💰 Te has encontrado $10 de camino a %s\n", aldea_actual->sig->nombre);
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

        else if (strcmp(comando, "ant") == 0) {
            if (aldea_actual->ant) {
                printf("💰 Te has encontrado $10 de camino a %s\n", aldea_actual->ant->nombre);
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

        else if (strcmp(comando, "trans") == 0) {
            if (desbloqueo_mundo_paralelo && aldea_actual->vinculada) {
                aldea_actual = aldea_actual->vinculada;
                printf("🌐 Te transportaste a %s.\n", aldea_actual->nombre);
            } else printf("🚫 No puedes transportarte aún.\n");
        }

        else {
            printf("❓ Comando no reconocido.\n");
        }
    }

    // Fuera del bucle principal, verificar por qué terminó el juego
    if (vidas <= 0) {
        printf("\n💀 Game Over. Has perdido todas tus vidas.\n");
    } else if (mazmorras_no_derrotadas == 0) {
        printf("\n🎉 ¡FELICIDADES! ¡Has derrotado todas las mazmorras escapaste del mundo de Link! 🎉\n");
    }
}