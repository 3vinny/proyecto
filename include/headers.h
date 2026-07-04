#ifndef HEADERS_H
#define HEADERS_H

// --- Header central proyecto ---***

// --- Includes ---
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"

typedef struct {
      SDL_Window *ventana;
      SDL_GameController *mando;
      SDL_Renderer *renderer;
      SDL_Texture *texturaTexto;
      SDL_Texture *texturaTexto2;
      SDL_Texture *texturaImg;
      SDL_Texture *texturaJugador;
      SDL_Surface *surfaceTexto;
      SDL_Surface *surfaceTexto2;
      TTF_Font *fuente;
      bool quit; // Flag
      int wText, hText; // texto Comic Sans
      
      int lado; // tam cuadrado
      float x;
      float y;
      float delta_time;
      int velocidad; // multiplicador de velocidad en update.c (posicioens son int .'v)

      int x_ant; // variable que guarda la posicion anterior de mi cuadrado azul antes de entrar al evento
      int y_ant;

      int h_x; // hitbox
      int h_y; 
      int h_w;
      int h_h;

      int x_colision;
      int y_colision;
      int w_colision;
      int h_colision;

      int up;
      int down;
      int left;
      int right;

      // tamaño tile en pixeles pq el render se rige en pixeles enteros
      int tile_w_px;
      int tile_h_px;
      int win_w;
      int win_h;
      
      struct
      {
         int x_tiles; // posicion mi rectangulo
         int y_tiles;
         int w_tiles; // alto y ancho mi rectangulo
         int h_tiles;
         
         int jugador_x;
         int jugador_y;
         
         bool activo;
         bool activo_posJ;
         bool objeto2;
         bool borde1;
         bool borde2;
      } tiles[tile_filas][tile_cols];


      bool Fullscreen;

}Game;

/*---- PROTOTIPOS DE FUNCIONES -------*/
bool SDL_Inicia(Game *game);  //eventos.c
bool cargarMedia(Game *game); //assets.c

void game_Limpieza(Game *game, int exitStatus); //eventos.c
void game_Main(Game *game, int exitStatus); //juego.c
void interfaz_Inicia(Game *game);  //interfaz.c

void game_Input(Game *game); //eventos.c
void carga_Tiles(Game *game); //cargas.c
void ajusta_Tiles(Game *game); //cargas.c
void game_Update(Game *game); //update.c
void game_Render(Game *game); //render.c

#endif
