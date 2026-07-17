#include "headers.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

void interfaz_Inicia(Game *game)
{
    // texto que dice teclas awsd
    SDL_Color colorTexto = { 255, 255, 255, 255 }; //Blanco y 255 de opacidad
    
    SDL_Surface *surfaceTexto1 = TTF_RenderText_Solid(game->fuente, "Usa AWSD, Flechas o el pad del mando para moverte. H para bocina", colorTexto);
    if (surfaceTexto1 != NULL)
    {
        game->texturaTexto = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTexto1);
        SDL_FreeSurface(surfaceTexto1);
    }
    
    // Cronometro
    /*SDL_Surface *surfaceTexto2 = TTF_RenderText_Solid(game->fuente, "Tiempo: xx:xx:xx segundos", colorTexto);
    if (surfaceTexto2 != NULL)
    {
        game->texturaTexto2 = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTexto2);
        SDL_FreeSurface(surfaceTexto2);
    }*/
}
