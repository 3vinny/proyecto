#ifndef CONSTANTS_H
#define CONSTANTS_H

// titulo y flags
#define titulo "Fugitive:Racer"
#define FLAGS_IMAGEN IMG_INIT_PNG
#define FLAGS_AUDIO MIX_INIT_MP3

// resolución
#define w_inicial 1200
#define h_inicial 900

// tiles: valores de referencia /px 40
// voy a cambiarlos a 50x30
#define tile_cols 50
#define tile_filas 30
#define tam 32
#define ZOOM 2

// jugador
#define HP_INICIAL 100

// enemigos
#define max_enemigos 10
#define VELOCIDAD_ENEMIGO1 70.0f
#define VELOCIDAD_PERSECUCION 80.0f
#define MAX_PROYECTILES 50
#define VELOCIDAD_DISPARO 350.0f
#define COOLDOWN_DISPARO 2.0f
#define RADIO_EJ 100.0f // radio enemigo-jugador

#endif
