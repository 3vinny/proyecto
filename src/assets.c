// archivo para los assets
// d momento solo carga el fondo
#include "headers.h"

bool cargarMedia(Game *game)
{
//./data/hitbox.txt
    game->texturaImg = IMG_LoadTexture(game->renderer, "./assets/bg/xp.png");
    if (!game->texturaImg) {
        printf("Error iniciando textura: %s\n", IMG_GetError());
        return true;
    }
    
    SDL_Surface *surfaceTemp = IMG_Load("./assets/sprites/car1.png");
    if(!surfaceTemp){
        printf("Error cargando imagen car1 : %s\n", IMG_GetError());
    }
    
    game->texturaJugador = SDL_CreateTextureFromSurface(game->renderer, surfaceTemp);
    SDL_FreeSurface(surfaceTemp); // se limpia surface de la ram
    if(!game->texturaJugador) {
        printf("Error iniciando textura jugador: %s\n", SDL_GetError());
        return true;
    }
    
    game->texturaPista = IMG_LoadTexture(game->renderer, "./assets/sprites/tileset.png");
    
    return false;
}
