#ifndef HEADERS_H
#define HEADERS_H

// --- Header central proyecto ---***

// --- Includes ---
#include "constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*---- Estructuras & funciones -------*/
// --- Pantalla ---
typedef struct {
    SDL_Window *ventana;
    SDL_Renderer *renderer;
    SDL_Rect camara;
    
    bool Fullscreen;
    int win_w;
    int win_h;
    
    int nivel_w;
    int nivel_h;
 } Pantalla;

// -- Balas/proyectiles/armamento --
typedef struct {
    float x;
    float y;
    float angulo;
    float dir_x;
    float dir_y;
    int lado;
    int velocidad;
    bool activo;
    bool es_enemigo;
    bool sonido;
} Proyectil;

// fondo & interfaz: pendiente y unused
typedef struct {
    SDL_Texture *texturaTexto;
    SDL_Texture *texturaTexto2;
    SDL_Texture *texturaTexto3;
    SDL_Texture *texturaHP;
    SDL_Texture *texturaBalas;
    SDL_Texture *texturaNivel;
    char texto_Balas[64];
    char texto_HP[64];
    char texto_cronometro[64];
    char texto_Mision[20];
    char texto_Nivel[64];
} Interfaz;

typedef struct {
    float x;
    float y;
    float tiempo;
    bool activa;
} Explosion;

// --- JUGADOR/PERSONAJE ---
typedef struct {
    SDL_Texture *textura;
    int lado;
    float x;
    float y;
    float angulo;
    int hp;
    
    // fisicas auto
    int velocidad;
    float velocidad_actual;
    int dir_x;
    int dir_y;
    float tiempo_arranque;
    int colisionando;
    
    // input
    int up;
    int down;
    int left;
    int right;
    int freno;
    int bocina;
    int disparo;
    float cooldown_disparo;
    float cooldown_agua;
    float cooldown_choque;
    int contador_balas;
    
    //hitbox
    int x_colision;
    int y_colision;
    int w_colision;
    int h_colision;
    SDL_Rect rect;
    SDL_Rect rect_colision;
    Proyectil proyectiles[MAX_PROYECTILES];
    // arreglo balas personaje
    // colocar cantidad almacena cantidad de balas
    // poner otro objeto que recargue
} Personaje;

// --- ENEMIGOS ---
typedef struct {
    float x;
    float y;
    float angulo;
    float cooldown_disparo;
    int dir_x;
    int dir_y;
    int lado;
    int hp; // o impactos
    int velocidad;
    SDL_Rect rect;
    bool activo;
    bool perseguir;
    bool escapando;
    bool sirena;
    bool es_patrulla;
    bool es_camion;
    int contador_camiones;
    bool es_bote;
    bool es_torreta;
    bool indicador;
    //bool es_peaton;
    Proyectil proyectiles[MAX_PROYECTILES];
    //colocar arreglo de balas aqui
} Enemigo;

// --- TILES ---
typedef struct {
    int x_tiles;
    int y_tiles;
    int w_tiles;
    int h_tiles;
    
    bool activo; //# .
    bool activo_posJ; //P y p
    bool objeto2; //X (caja con +1HP)
    bool objeto3; //Z (caja con balas)
    char tipo; //- | 1 2 3 4
    
    bool enemigo1; // auto policia (E)
    bool enemigo2; // peaton (W)
    bool enemigo3; // camion gas (G)
    bool enemigo4; // Bote (B)
    bool enemigo5; //torreta (V)
    
    int casa; // c y C (0,1,-1=destruida)
    int movediza; // 0, 1=salvable, 2=hundido
    bool agua; // A
    bool meta; // F
    bool obstaculo; // N
    bool neumatico; // o
    bool semaforo; // S
    bool aceite; // M
    int direccion; // D y d
} Tile;

// --- GAME GENERAL ---
typedef struct {
    bool quit;
    float delta_time;
    
    // multimedia
    SDL_GameController *mando;
    TTF_Font *fuente;
    Mix_Chunk *vozinha;
    Mix_Chunk *sirena1;
    Mix_Chunk *bala;
    Mix_Chunk *sel_menu;
    SDL_Texture *texturaImg;
    SDL_Texture *texturaPista;
    
    SDL_Texture *texturaEnemigo; // patrulla (E)
    SDL_Texture *indicadorEnemigo; // patrulla (E)
    
    SDL_Texture *texturaEnemigo2; //peaton (W)
    SDL_Texture *texturaEnemigo3; // gas (G)
    SDL_Texture *texturaEnemigo4; // bote (B)
    SDL_Texture *texturaEnemigo5; // torreta (V)
    SDL_Texture *texturaCaja; // caja (X)
    SDL_Texture *texturaCajaBalas; // caja (Z)
    SDL_Texture *texturaAceite; // aceite (M)
    SDL_Texture *texturaMeta; // finish (F)
    SDL_Texture *texturaDireccion; // direccion (D) y (d)
    
    // meta y finalizacion
    int vueltas;
    bool llego_meta;
    int contador_camiones;
    
    // texto, cronometro y bocina
    
    Uint32 tiempo_inicio;
    Uint32 ultimo_pitido;
    Uint32 ultimo_tiempo_vozinha;
    Uint32 ultimo_tiempo_meta;
    
    // subestructuras
    Pantalla pantalla;
    Personaje jugador;
    Interfaz interfaz;

    // explosiones y arreglos
    SDL_Texture *texturaExplosion[FRAMES_EXPLOSION];
    Explosion explosiones[MAX_EXPLOSIONES];
    Tile tiles[tile_filas][tile_cols];
    Enemigo enemigos[max_enemigos];
    
    // mecanica niveles (1,2,3)
    int nivel_actual;
    bool cambiar_nivel;
} Game;

/*---- PROTOTIPOS DE FUNCIONES -------*/

bool SDL_Inicia(Game *game);  // eventos.c
bool cargarMedia(Game *game); // assets.c

void game_Menu(Game *game); // menu.c
void game_Limpieza(Game *game, int exitStatus); // eventos.c
void game_Input(Game *game);   // eventos.c
void game_Main(Game *game, int exitStatus);     // juego.c
void interfaz_Inicia(Game *game);               // interfaz.c
void carga_Tiles(Game *game);  // cargas.c
void ajusta_Tiles(Game *game); // cargas.c
void carga_Nivel(Game *game, int nuevo_nivel); // cargas.c
void game_Update(Game *game);  // update.c
void game_Render(Game *game);  // render.c

#endif
