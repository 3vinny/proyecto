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

// fondo: pendiente y unused
typedef struct {
} Fondo;

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
    int velocidad;
    SDL_Rect rect;
    bool activo;
    bool perseguir;
    bool sirena;
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
    bool activo_posJ; //P
    bool objeto2; //X
    char tipo; //- | 1 2 3 4
    
    bool enemigo1; // auto policia
    bool enemigo2; // peaton
    
    int casa; // c y C (0,1,-1=destruida)
    bool agua; // A
    bool obstaculo; // N
    bool neumatico; // o
    bool semaforo; // S
    bool aceite; // M
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
    SDL_Texture *texturaImg;
    SDL_Texture *texturaPista;
    SDL_Texture *texturaEnemigo;
    SDL_Texture *texturaCaja;
    SDL_Texture *texturaAceite;
    
    // texto, cronometro y bocina
    SDL_Texture *texturaTexto;
    SDL_Texture *texturaTexto2;
    Uint32 tiempo_inicio;
    Uint32 ultimo_pitido;
    Uint32 ultimo_tiempo_vozinha;
    char texto_cronometro[64];
    
    // subestructuras
    Pantalla pantalla;
    Personaje jugador;
    Tile tiles[tile_filas][tile_cols];
    Enemigo enemigos[max_enemigos];
} Game;

/*---- PROTOTIPOS DE FUNCIONES -------*/

bool SDL_Inicia(Game *game);  // eventos.c
bool cargarMedia(Game *game); // assets.c

void game_Limpieza(Game *game, int exitStatus); // eventos.c
void game_Main(Game *game, int exitStatus);     // juego.c
void interfaz_Inicia(Game *game);               // interfaz.c

void game_Input(Game *game);   // eventos.c
void carga_Tiles(Game *game);  // cargas.c
void ajusta_Tiles(Game *game); // cargas.c
void game_Update(Game *game);  // update.c
void game_Render(Game *game);  // render.c
void render_Cronometro(Game *game); // render.c

#endif
