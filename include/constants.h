#ifndef CONSTANTS_H
#define CONSTANTS_H

// -- titulo y flags --
#define titulo "Fugitive:Racer"
#define FLAGS_IMAGEN IMG_INIT_PNG
#define FLAGS_AUDIO MIX_INIT_MP3

// -- resolucion --
#define w_inicial 1200
#define h_inicial 900

// -- tiles: valores de referencia & geometria --
// 50x30 -> 60x40
#define tile_cols 60
#define tile_filas 40
#define LIMITE_LINEA 256
#define tam 32
#define ZOOM 2
#define PI 3.1415926535

// -- jugador --
#define HP_INICIAL 10
#define HP_MAX 20
#define MAX_JUGADORES 2

// -- enemigos --
#define max_enemigos 10
#define VELOCIDAD_ENEMIGO1 70.0f
#define VELOCIDAD_PERSECUCION 80.0f
#define HP_ENEMIGOS 10
#define VELOCIDAD_ENEMIGO3 40.0f

// enemigo-jugador
#define MAX_PROYECTILES 50
#define VELOCIDAD_DISPARO 350.0f
#define COOLDOWN_DISPARO 2.0f
#define RADIO_EJ 100.0f
#define RADIO_PERDIDO 500.0f


#endif