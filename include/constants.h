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
// 50x30 -> 60x40 -> 80x60
#define tile_cols 80
#define tile_filas 60
#define LIMITE_LINEA 256
#define tam 32
#define ZOOM 2
#define PI 3.1415926535

// -- jugador --
#define HP_INICIAL 10
#define HP_MAX 20
#define MAX_JUGADORES 2
#define COOLDOWN_DISPARO_JUGADOR 0.3f
#define MAX_VUELTAS 3
//#define COOLDOWN_META 5000

// -- enemigos --
#define max_enemigos 50
#define VELOCIDAD_ENEMIGO1 70.0f
#define VELOCIDAD_PERSECUCION 80.0f
#define HP_ENEMIGOS 15
#define HP_BOTE 15
#define VELOCIDAD_ENEMIGO3 50.0f
#define VELOCIDAD_BOTE 25.0f

// enemigo-jugador
#define MAX_PROYECTILES 50
#define VELOCIDAD_DISPARO 350.0f
#define COOLDOWN_DISPARO 2.0f
#define RADIO_EJ 100.0f
#define RADIO_PERDIDO 600.0f

// enemigo 3 camion gas
#define EMPUJE_CAMION 90.0f
#define HP_CAMION 4
// explosion camion gas
#define RADIO_EXPLOSION 80.0f
#define DURACION_EXPLOSION 0.6f
#define DANO_EXPLOSION 3
#define MAX_EXPLOSIONES 10
#define FRAMES_EXPLOSION 6
#define TAM_EXPLOSION 64

#endif