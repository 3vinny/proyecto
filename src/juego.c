// conecta todas las funciones en una (podria cambiarlo a main.c)
#include "headers.h"

void game_Main(Game *game, int exitStatus)
{
    interfaz_Inicia(game);
    
    game->tiempo_inicio = SDL_GetTicks();
    Uint32 tiempo_anterior = SDL_GetTicks();
    

    while (!game->quit)
    {
        // delta time
        Uint32 tiempo_actual = SDL_GetTicks();
        game->delta_time = (tiempo_actual - tiempo_anterior) / 1000.0f;
        tiempo_anterior = tiempo_actual;
                
        // funciones juego
        game_Input(game);
        game_Update(game);
        game_Render(game);
        
        SDL_Delay(16); //60FPS
    }
}
