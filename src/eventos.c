#include "headers.h"
// rango range: -32768 to 32767 | zona muerta es el valor maximo

void game_Input(Game *game)
{
    int tiempo_actual = SDL_GetTicks();
    int cooldown = 200;
    int zona_muerta = 20100;

    SDL_Event evento;
    while (SDL_PollEvent(&evento)) 
    {
        if(evento.type == SDL_QUIT) game->quit = true;

        if (evento.type == SDL_CONTROLLERBUTTONDOWN)
        {
            printf("Se detecto presion mando: \n");
            switch(evento.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_DPAD_UP: 
                    game->jugador.up = 1;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: 
                    game->jugador.right = 1;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    game->jugador.down = 1;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    game->jugador.left = 1;
                    break;
                case SDL_CONTROLLER_BUTTON_B:
                    game->jugador.freno = 1;
                    break;
                default:
                    game->jugador.up = 0;
                    game->jugador.down = 0;
                    game->jugador.left = 0;
                    game->jugador.right = 0;
                    game->jugador.freno = 0;
                    break;
            }
        }

        if (evento.type == SDL_CONTROLLERBUTTONUP)
        {
            switch(evento.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_DPAD_UP: 
                    game->jugador.up = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: 
                    game->jugador.right = 0; 
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN: 
                    game->jugador.down = 0; 
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT: 
                    game->jugador.left = 0; 
                    break;
                case SDL_CONTROLLER_BUTTON_B:
                    game->jugador.freno = 0;
                    break;
                default: 
                    game->jugador.up = 0;
                    game->jugador.down = 0;
                    game->jugador.left = 0;
                    game->jugador.right = 0;
                    game->jugador.freno = 0;
                    break;
            }
        }

      // CASO PALANCAS GAMEPAD  rango: -32768 to 32767 | AXIS_LEFTX : palanca izquierda
        if (evento.type == SDL_CONTROLLERAXISMOTION)
        {
            if (evento.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
            {
                if (evento.caxis.value < -zona_muerta) 
                {
                    game->jugador.left = 1; 
                    game->jugador.right = 0;
                } 
                else if (evento.caxis.value > zona_muerta)
                {
                    game->jugador.right = 1; 
                    game->jugador.left = 0;
                } 
                else 
                {
                    game->jugador.left = 0;
                    game->jugador.left = 0;
                }
            }
          
            if (evento.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
            {
                if (evento.caxis.value < -zona_muerta) 
                {
                    game->jugador.up = 1; 
                    game->jugador.down = 0;
                } 
                else if (evento.caxis.value > zona_muerta) 
                {
                    game->jugador.down = 1; 
                    game->jugador.up = 0;
                } 
                else
                {
                    game->jugador.up = 0; 
                    game->jugador.down = 0;
                }
            }
        }

        if(evento.type == SDL_KEYDOWN)
        {
            switch(evento.key.keysym.sym)
            {
                case SDLK_UP: case SDLK_w: 
                    game->jugador.up = 1; break;
                case SDLK_DOWN: case SDLK_s: 
                    game->jugador.down = 1; break;
                case SDLK_LEFT: case SDLK_a: 
                    game->jugador.left = 1; break;
                case SDLK_RIGHT: case SDLK_d: 
                    game->jugador.right = 1; break;
                case SDLK_SPACE: 
                    game->jugador.freno = 1;
                    break;

                case SDLK_j:
                    game->jugador.disparo = 1;
                    break;

                case SDLK_h:
                    if (tiempo_actual - game->ultimo_tiempo_vozinha >= cooldown)
                    {
                        if (game->jugador.bocina == 0)
                        {
                            printf("vozinha bip bipppppppppppppppppppppppp\n");
                            if (game->vozinha != NULL)
                            {
                                Mix_PlayChannel(-1, game->vozinha, 0);
                            }
                        }
                        game->ultimo_tiempo_vozinha = tiempo_actual;
                    }
                    game->jugador.bocina = 1;
                    break;
                case SDLK_F11:
                    game->pantalla.Fullscreen = !(game->pantalla.Fullscreen); // se alterna
                    if (game->pantalla.Fullscreen)
                    {
                        SDL_SetWindowFullscreen(game->pantalla.ventana, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    } 
                    else
                    {
                        SDL_SetWindowFullscreen(game->pantalla.ventana, 0);
                    }
                    break;
                case SDLK_ESCAPE:
                    game->quit = true;
                    break;
                default:
                    break;
            }
        }

        if(evento.type == SDL_KEYUP)
        {
            switch(evento.key.keysym.sym)
            {
                case SDLK_UP: case SDLK_w: game->jugador.up = 0; break;
                case SDLK_DOWN: case SDLK_s: game->jugador.down = 0; break;
                case SDLK_LEFT: case SDLK_a: game->jugador.left = 0; break;
                case SDLK_RIGHT: case SDLK_d: game->jugador.right = 0; break;
                case SDLK_SPACE: game->jugador.freno = 0; break;
                case SDLK_h: game->jugador.bocina = 0; break;
                case SDLK_j:
                    game->jugador.disparo = 0;
                    break;
                    
                default:
                    break;
            }
        }
    }
}
