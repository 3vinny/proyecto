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

/*---- Estructuras & funciones -------
PANTALLA*/

typedef struct {
    SDL_Window *ventana;
    SDL_Renderer *renderer;
    bool Fullscreen;
    int win_w;
    int win_h;
    
    SDL_Rect camara;
    int nivel_w;
    int nivel_h;
 }Pantalla;

// JUGADOR/PERSONAJE
typedef struct {
    SDL_Texture *textura;
    int lado;
    float x;
    float y;
    float angulo;
    
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
} Personaje;

// ENEMIGOS
typedef struct {
    float x;
    float y;
    int lado;
    int velocidad;
    SDL_Rect rect;
    bool activo; //objeto enemigo txt
} Enemigo;

// TILES
typedef struct {
    int x_tiles;
    int y_tiles;
    int w_tiles;
    int h_tiles;
    
    bool activo; //# .
    bool activo_posJ; //P
    bool objeto2; //X
    char tipo; //- | 1 2 3 4
} Tile;

// GAME GENERAL
typedef struct {
    bool quit;
    float delta_time;
    
    // multimedia
    SDL_GameController *mando;
    TTF_Font *fuente;
    Mix_Chunk *vozinha;
    SDL_Texture *texturaImg;
    SDL_Texture *texturaPista;
    
    // texto y cronometro
    SDL_Texture *texturaTexto;
    SDL_Texture *texturaTexto2;
    Uint32 tiempo_inicio;
    Uint32 ultimo_pitido;
    Uint32 ultimo_tiempo_vozinha;
    char texto_cronometro[64];
    int wText, hText;
    
    // subestructuras
    Pantalla pantalla;
    Personaje jugador;
    Tile tiles[tile_filas][tile_cols];
    Enemigo enemigos[10];
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
