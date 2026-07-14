#include "headers.h"

bool cargarMedia(Game *game)
{
//./data/hitbox.txt
    game->texturaImg = IMG_LoadTexture(game->pantalla.renderer, "./assets/bg/bg.png");
    if (!game->texturaImg) {
        printf("Error iniciando textura: %s\n", IMG_GetError());
        return true;
    }
    
    SDL_Surface *surfaceTemp = IMG_Load("./assets/sprites/mclaren.png");
    if(!surfaceTemp){
        printf("Error cargando imagen car1 : %s\n", IMG_GetError());
    }
    
    game->jugador.textura = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTemp);
    SDL_FreeSurface(surfaceTemp); // se limpia surface de la ram
    
    if(!game->jugador.textura) {
        printf("Error iniciando textura jugador: %s\n", SDL_GetError());
        return true;
    }
    
    game->texturaPista = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/tileset.png");
    
    game->vozinha = Mix_LoadWAV("./assets/sfx/bocina2.wav");
    if(game->vozinha == NULL){
        printf("error con bocina.wav : %s\n", Mix_GetError());
    }
    
    return false;
}
