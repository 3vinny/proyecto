#ifndef HEADERS_H
#define HEADERS_H

// --- Header central proyecto ---***

// --- Includes ---
#include "constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// god object -/> varios structs

typedef struct {
  // variables SDL2
  SDL_Window *ventana;
  SDL_GameController *mando;
  SDL_Renderer *renderer;
  TTF_Font *fuente;
  SDL_Texture *texturaTexto;
  SDL_Texture *texturaTexto2;
  SDL_Texture *texturaImg;
  SDL_Texture *texturaJugador;
  SDL_Texture *texturaPista;
  SDL_Surface *surfaceTexto;
  SDL_Surface *surfaceTexto2;

  // Flags: si el juego esta corriendo ,fullscreen
  bool quit;
  bool Fullscreen;

  // texto
  int wText, hText;

  // tam cuadrado, posiciones game->x,y
  int lado;
  float x;
  float y;
  float angulo;

  // delta time y velocidades
  float delta_time;
  int velocidad;
  float velocidad_actual;

  // direccion auto
  int dir_x;
  int dir_y;
  float tiempo_arranque;
  int colisionando;

  // input auto
  int up;
  int down;
  int left;
  int right;

  // variables que guarda la posicion anterior de mi cuadrado azul antes de
  // entrar al evento
  int x_ant;
  int y_ant;

  // hitbox
  int h_x;
  int h_y;
  int h_w;
  int h_h;

  int x_colision;
  int y_colision;
  int w_colision;
  int h_colision;

  // tamaño tile en pixeles pq el render se rige en pixeles enteros
  int tile_w_px;
  int tile_h_px;
  int win_w;
  int win_h;

  // Estructura de los tiles
  struct {
    // posicion, alto,ancho mi rectangulo
    int x_tiles;
    int y_tiles;
    int w_tiles;
    int h_tiles;

    int jugador_x;
    int jugador_y;

    // booleanos: objetos presentes que se cargan por txt
    bool activo;
    bool activo_posJ;
    bool objeto2;
    bool borde1;
    bool borde2;
    // guarda caracter txt pista
    char tipo;
  } tiles[tile_filas][tile_cols];
  

  // rectangulos update.c
  SDL_Rect rect_jugador;
  SDL_Rect rect_hitbox;
  SDL_Rect rect_colision;
  SDL_Rect rect_colision2;

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

#endif
