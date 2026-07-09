// archivo inputstate, tendra opcion de gamepad
#include "headers.h"
// rango range: -32768 to 32767 | zona muerta es el valor maximo
int zona_muerta = 20100;

void game_Input(Game *game)
{
   SDL_Event evento;
    
   while (SDL_PollEvent(&evento)) {
      if(evento.type == SDL_QUIT){
         game->quit = true;
      }

      if (evento.type == SDL_CONTROLLERBUTTONDOWN){
         printf("Se detecto presion mando: \n");
         switch(evento.cbutton.button)
         {
             case SDL_CONTROLLER_BUTTON_DPAD_UP: 
                 game->up = 1;
                 break;
             case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: 
                 game->right = 1;
                 break;
             case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                 game->down = 1;
                 break;
             case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                 game->left = 1;
                 break;
             case SDL_CONTROLLER_BUTTON_B:
                 game->freno = 1;
                 break;
             default: game->up, game->down, game->left, game->right = 0; game->freno = 0; break;
         }
      }

      if (evento.type == SDL_CONTROLLERBUTTONUP){
         switch(evento.cbutton.button)
         {
            case SDL_CONTROLLER_BUTTON_DPAD_UP: 
               game->up = 0;
               break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: 
               game->right = 0; 
               break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN: 
               game->down = 0; 
               break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT: 
               game->left = 0; 
               break;
           case SDL_CONTROLLER_BUTTON_B:
               game->freno = 0;
               break;
            default: game->up, game->down, game->left, game->right = 0; game->freno = 0; break;
         }
      }

      // CASO PALANCAS GAMEPAD
      // rango: -32768 to 32767 | AXIS_LEFTX : palanca izquierda
      if (evento.type == SDL_CONTROLLERAXISMOTION){
          
          if (evento.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
          {
              if (evento.caxis.value < -zona_muerta) {
                  game->left = 1; 
                  game->right = 0;
              } else if (evento.caxis.value > zona_muerta) {
                  game->right = 1; 
                  game->left = 0;
              } else {
                  game->left = 0;
                  game->left = 0;
              }
          }
          
          if (evento.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
          {
              if (evento.caxis.value < -zona_muerta) {
                  game->up = 1; 
                  game->down = 0;
              } else if (evento.caxis.value > zona_muerta) {
                  game->down = 1; 
                  game->up = 0;
              } else {
                  game->up = 0; 
                  game->down = 0;
              }
          }
      }

      if(evento.type == SDL_KEYDOWN){
         switch(evento.key.keysym.sym)
         {
            case SDLK_UP: case SDLK_w: game->up = 1; break;
            case SDLK_DOWN: case SDLK_s: game->down = 1; break;
            case SDLK_LEFT: case SDLK_a: game->left = 1; break;
            case SDLK_RIGHT: case SDLK_d: game->right = 1; break;
            case SDLK_SPACE: game->freno = 1; break;
            case SDLK_h:
                int tiempo_actual = SDL_GetTicks();
                int cooldown = 200;
                if (tiempo_actual - game->ultimo_tiempo_vozinha >= cooldown){
                    if (game->bocina == 0){
                        printf("vozinha bip bipppppppppppppppppppppppp\n");
                        if (game->vozinha != NULL){
                            Mix_PlayChannel(-1, game->vozinha, 0);
                        }
                    }
                    game->ultimo_tiempo_vozinha = tiempo_actual;
                }
                game->bocina = 1;
                break;
            case SDLK_F11:
               game->Fullscreen = !(game->Fullscreen); // se alterna
               if (game->Fullscreen){
                  SDL_SetWindowFullscreen(game->ventana, SDL_WINDOW_FULLSCREEN_DESKTOP);
               } else {
                  SDL_SetWindowFullscreen(game->ventana, 0);
               }
               break;
            case SDLK_ESCAPE:
               game->quit = true;
               break;
            default:
               break;
         }
         break;
      }

      if(evento.type == SDL_KEYUP){
         switch(evento.key.keysym.sym){
            case SDLK_UP: case SDLK_w: game->up = 0; break;
            case SDLK_DOWN: case SDLK_s: game->down = 0; break;
            case SDLK_LEFT: case SDLK_a: game->left = 0; break;
            case SDLK_RIGHT: case SDLK_d: game->right = 0; break;
            case SDLK_SPACE: game->freno = 0; break;
            case SDLK_h: game->bocina = 0; break;
            default:
                break;
         }
      }
   }
}
