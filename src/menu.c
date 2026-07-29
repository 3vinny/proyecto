// menu principal
#include "headers.h"

bool mouse_rect(int mouse_x, int mouse_y, SDL_Rect *rect);

void game_Menu(Game *game)
{
    SDL_Color gris = {150,150,150,255}; //gris
    SDL_Color negro = {0,0,0,255}; //negro

    char *etiquetas[3] = {"JUGAR", "RANKING", "SALIR"};
    SDL_Texture *texturas_inactivas[3];
    SDL_Texture *texturas_activas[3];
    int texto_w[3];
    int texto_h[3];

    for (int i=0; i<3; i++) {
        // texto gris
        SDL_Surface *surface_inactiva = TTF_RenderText_Solid(game->fuente, etiquetas[i], gris);
        texturas_inactivas[i] = SDL_CreateTextureFromSurface(game->pantalla.renderer, surface_inactiva);

        SDL_Surface *surface_activa = TTF_RenderText_Solid(game->fuente, etiquetas[i], negro);
        texturas_activas[i] = SDL_CreateTextureFromSurface(game->pantalla.renderer, surface_activa);

        texto_w[i] = surface_inactiva->w;
        texto_h[i] = surface_inactiva->h;
        SDL_FreeSurface(surface_inactiva);
        SDL_FreeSurface(surface_activa);
    }

    int separacion = 40;
    int ancho_total = texto_w[0] + texto_w[1] + texto_w[2] + (2*separacion);
    int x_actual = (game->pantalla.win_w - ancho_total) / 2;
    int y_pos = game->pantalla.win_h - separacion;
    // arreglo de rectangulos /botones
    SDL_Rect botones[3];
    
    for (int i=0; i<3; i++) {
        botones[i].x = x_actual;
        botones[i].y = y_pos;
        botones[i].w = texto_w[i];
        botones[i].h = texto_h[i];
        x_actual += texto_w[i] + separacion;
    }

    bool en_menu = true;
    int opc_seleccionada = 0; //0 jugar 1 config 2 salir

    while (en_menu && !game->quit) {
        SDL_Event evento;
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                game->quit = true;
                en_menu = false;
            }

            if (evento.type == SDL_KEYDOWN) {
                switch(evento.key.keysym.sym)
                {
                    case SDLK_UP:
                    case SDLK_LEFT:
                        opc_seleccionada--;
                        if (opc_seleccionada < 0) opc_seleccionada = 2;
                        break;
                    case SDLK_DOWN:
                    case SDLK_RIGHT:
                        opc_seleccionada++;
                        if (opc_seleccionada > 2) opc_seleccionada = 0;
                        break;
                    case SDLK_RETURN:
                        if (opc_seleccionada == 0) {
                            Mix_PlayChannel(-1, game->sel_menu, 0);
                            en_menu = false;
                        } else if (opc_seleccionada == 1) {
                            Mix_PlayChannel(-1, game->sel_menu, 0);
                            SDL_Log("Ranking desde keyboard\n\n");
                        } else if (opc_seleccionada == 2){
                            Mix_PlayChannel(-1, game->sel_menu, 0);
                            game->quit = true;
                            en_menu = false;
                        }
                        break;
                    default:
                        break;
                }
            }

            // evento de hovear con el mouse
            if (evento.type == SDL_MOUSEMOTION) {
                int mouse_x = evento.motion.x;
                int mouse_y = evento.motion.y;
                for (int i=0; i<3; i++) {
                    if (mouse_rect(mouse_x, mouse_y, &botones[i])) {
                        opc_seleccionada = i;
                    }
                }
            }

            // evento del click MOUSE
            if (evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT)
            {
                int mouse_x = evento.button.x;
                int mouse_y = evento.button.y;

                if (mouse_rect(mouse_x,mouse_y, &botones[0])) {
                    en_menu = false;
                } else if (mouse_rect(mouse_x, mouse_y, &botones[1])) {
                    SDL_Log("Ranking desde keyboard\n\n");
                } else if (mouse_rect(mouse_x, mouse_y, &botones[2])) {
                    game->quit = true;
                    en_menu = false;
                }
            }
        }

        SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->pantalla.renderer);

        SDL_RenderCopy(game->pantalla.renderer, game->texturaImg, NULL, NULL);

        // renderizar texto gris o negro
        for (int i=0; i<3; i++) {
            if (i == opc_seleccionada) {
                SDL_RenderCopy(game->pantalla.renderer, texturas_activas[i], NULL, &botones[i]);
            } else {
                SDL_RenderCopy(game->pantalla.renderer, texturas_inactivas[i], NULL, &botones[i]);
            }
        }
        SDL_RenderPresent(game->pantalla.renderer);
        SDL_Delay(16);
    }

    for (int i=0; i<3; i++) {
        SDL_DestroyTexture(texturas_activas[i]);
        SDL_DestroyTexture(texturas_inactivas[i]);
    }
}

bool mouse_rect(int mouse_x, int mouse_y, SDL_Rect *rect)
{
    bool en_x = mouse_x >= rect->x && mouse_x <= rect->x + rect->w;
    bool en_y = mouse_y >= rect->y && mouse_y <= rect->y + rect->h;
    if (en_x && en_y) {
        return true;
    } else {
        return false;
    }
}