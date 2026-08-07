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
    
    SDL_Surface *surfaceTemp = IMG_Load("./assets/sprites/auto1.png");
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
    
    game->texturaPista = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/tileset_2.png");
    
    game->texturaEnemigo = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/carab.png");
    if (!game->texturaEnemigo)
    {
        printf("Error al iniciar textura enemigo patrulla(s): %s\n", IMG_GetError());
        return true;
    }
    
    game->indicadorEnemigo = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/alerta.png");
    if (!game->indicadorEnemigo)
    {
        printf("Error al iniciar indicador enemigo: %s", IMG_GetError());
        return true;
    }

    game->texturaEnemigo3 = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/camion.png");
    if (!game->texturaEnemigo3)
    {
        printf("Error al iniciar textura enemigo gas: %s\n", IMG_GetError());
        return true;
    }
    
    game->texturaEnemigo4 = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/bote.png");
    if (!game->texturaEnemigo4)
    {
        printf("Error al iniciar textura enemigo bote: %s\n", IMG_GetError());
        return true;
    }

    game->texturaEnemigo5 = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/torreta.png");
    if (!game->texturaEnemigo4)
    {
        printf("Error al iniciar textura enemigo torreta: %s\n", IMG_GetError());
        return true;
    }
    
    game->texturaMeta = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/meta.png");
    if (!game->texturaMeta)
    {
        printf("Error al iniciar tex meta: %s\n", IMG_GetError());
        return true;
    }
    
    game->texturaDireccion = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/direccion.png");
    if (!game->texturaDireccion)
    {
        printf("Error al iniciar tex direccion: %s\n", IMG_GetError());
        return true;
    }
    
    game->texturaCaja = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/box.png");
    if (!game->texturaCaja)
    {
        printf("Error al iniciar textura caja(s): %s\n", IMG_GetError());
        return true;
    }
    
    game->texturaCajaBalas = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/box_balas.png");
    if (!game->texturaCajaBalas)
    {
        printf("Error al iniciar textura caja(s): %s\n", IMG_GetError());
        return true;
    }

    game->texturaMinimapa = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/box_mapa.png");
    if (!game->texturaMinimapa)
    {
        printf("Error al iniciar textura caja(s) minimapa: %s\n", IMG_GetError());
        return true;
    }
    
    game->texturaAceite = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/oil.png");
    if (!game->texturaAceite)
    {
        printf("Error al iniciar textura aceite: %s\n", IMG_GetError());
        return true;
    }
    
    game->vozinha = Mix_LoadWAV("./assets/sfx/bocina3.wav");
    if(game->vozinha == NULL)
    {
        printf("error con bocina.wav : %s\n", Mix_GetError());
    }

    game->sirena1 = Mix_LoadWAV("./assets/sfx/sirena_pato.wav");
    if(game->sirena1 == NULL)
    {
        printf("error con sirena1.wav : %s\n", Mix_GetError());
    }
    
    game->bala = Mix_LoadWAV("./assets/sfx/disparo_1.wav");
    if (game->bala == NULL)
    {
        printf("error con disparo_1.wav : %s\n", Mix_GetError());
    }
    
    game->explosion = Mix_LoadWAV("./assets/sfx/explosion.wav");
    if (game->explosion == NULL)
    {
        printf("error con explosion.wav : %s\n", Mix_GetError());
    }
    
    game->powerup = Mix_LoadWAV("./assets/sfx/powerup.wav");
    if (game->powerup == NULL)
    {
        printf("error con powerup.wav : %s\n", Mix_GetError());
    }

    game->sel_menu = Mix_LoadWAV("./assets/sfx/menu.wav");
    if (game->sel_menu == NULL)
    {
        printf("error con seleccion del menu : %s\n", Mix_GetError());
    }

    game->audio.endgame = Mix_LoadWAV("./assets/music/endgame.mp3");
    if (game->audio.endgame == NULL)
    {
        printf("error con endgame : %s\n", Mix_GetError());
    }

    game->audio.fail = Mix_LoadWAV("./assets/music/fail.mp3");
    if (game->audio.fail == NULL)
    {
        printf("error con endgame(FAIL) : %s\n", Mix_GetError());
    }

    // falta audio.fondo con las cancioneS

    // iluminacion nivel 3
    game->texturaVignette = IMG_LoadTexture(game->pantalla.renderer, "./assets/sprites/vignette.png");
    if (game->texturaVignette == NULL)
    {
        printf("Error al iniciar textura iluminacion: %s\n", IMG_GetError());
        return true;
    }
    SDL_SetTextureBlendMode(game->texturaVignette, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(game->texturaVignette, 128);

    // explosiones
    char ruta_explosion[64];
    for (int i=0; i<FRAMES_EXPLOSION; i++) 
    {
        snprintf(ruta_explosion, sizeof(ruta_explosion), "./assets/sprites/explosion%d.png", i+1);
        game->texturaExplosion[i] = IMG_LoadTexture(game->pantalla.renderer, ruta_explosion);
        if (game->texturaExplosion[i] == NULL)
        {
            printf("Error al cargar frame %d explosion: %s\n", i, IMG_GetError());
        }
    }
    
    return false;
}
