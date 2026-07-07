// puro textoooooooo
#include "headers.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

void interfaz_Inicia(Game *game)
{
    // texto que dice teclas awsd
    SDL_Color colorTexto = { 255, 255, 255, 255 }; //Blanco y 255 de brillo
    game->surfaceTexto = TTF_RenderText_Solid(game->fuente, "Usa AWSD, Flechas o el pad del mando para moverte", colorTexto);
    game->texturaTexto = SDL_CreateTextureFromSurface(game->renderer, game->surfaceTexto);
    
    // Cronometro
    game->surfaceTexto2 = TTF_RenderText_Solid(game->fuente, "Tiempo: [placeholder] segundos", colorTexto);
    game->texturaTexto2 = SDL_CreateTextureFromSurface(game->renderer, game->surfaceTexto2);
}
