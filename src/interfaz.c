#include "headers.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

void interfaz_Inicia(Game *game)
{
    // texto que dice teclas awsd
    SDL_Color colorTexto = { 0, 0, 0, 255 }; //Blanco y 255 de opacidad
    
    SDL_Surface *surfaceTexto1 = TTF_RenderText_Solid(game->fuente, "Usa AWSD, Flechas o el pad del mando para moverte. H para bocina y J para disparar", colorTexto);
    if (surfaceTexto1 != NULL)
    {
        game->interfaz.texturaTexto = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTexto1);
        SDL_FreeSurface(surfaceTexto1);
    }
}
