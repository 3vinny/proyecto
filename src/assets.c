#include "headers.h"

bool cargarMedia(Game *game)
{
//./data/hitbox.txt
    game->texturaImg = IMG_LoadTexture(game->pantalla.renderer, "./assets/bg/bg.png");
    if (!game->texturaImg)
    {
        printf("Error iniciando textura: %s\n", IMG_GetError());
        return true;
    }
    
    SDL_Surface *surfaceTemp = IMG_Load("./assets/sprites/mclaren.png");
    if(!surfaceTemp)
    {
        printf("Error cargando imagen car1 : %s\n", IMG_GetError());
    }
    
    game->jugador.textura = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTemp);
    SDL_FreeSurface(surfaceTemp); // se limpia surface de la ram
    
    if(!game->jugador.textura)
    {
        printf("Error iniciando textura jugador: %s\n", SDL_GetError());
        return true;
    }
    
    game->texturaPista = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/tileset.png");
    
    game->texturaEnemigo = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/carab.png");
    if (!game->texturaEnemigo)
    {
        printf("Error al iniciar textura enemigo(s): %s\n", IMG_GetError());
        return true;
    }
    
    game->texturaCaja = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/box.png");
    if (!game->texturaCaja)
    {
        printf("Error al iniciar textura caja(s): %s\n", IMG_GetError());
        return true;
    }
    
    game->texturaAceite = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/oil.png");
    if (!game->texturaAceite) return true;
    
    game->vozinha = Mix_LoadWAV("./assets/sfx/bocina2.wav");
    if(game->vozinha == NULL)
    {
        printf("error con bocina.wav : %s\n", Mix_GetError());
    }

    game->sirena1 = Mix_LoadWAV("./assets/sfx/sirena_pato.wav");
    if(game->sirena1 == NULL)
    {
        printf("error con sirena1.wav : %s\n", Mix_GetError());
    }
    
    return false;
}
