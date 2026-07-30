// menu principal
#include "headers.h"

bool mouse_rect(int mouse_x, int mouse_y, SDL_Rect *rect);
void panel_ranking(Game *game);
void panel_config(Game *game);

void game_Menu(Game *game)
{
    SDL_Color gris = {150,150,150,255}; //gris
    SDL_Color negro = {0,0,0,255}; //negro
    SDL_Color plata = {192,192,192,255}; //plateado

    char *etiquetas[3] = {"JUGAR", "RANKING", "SALIR"};
    SDL_Texture *texturas_inactivas[3];
    SDL_Texture *texturas_activas[3];
    int texto_w[3];
    int texto_h[3];

    SDL_Surface *surf_config = TTF_RenderText_Solid(game->fuente, "Presiona Espacio para Config", gris);
    SDL_Texture *textura_config = SDL_CreateTextureFromSurface(game->pantalla.renderer, surf_config);

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
    int opc_seleccionada = 0; //0 jugar 1 ranking 2 salir

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
                    case SDLK_ESCAPE:
                        game->quit = true;
                        break;
                    case SDLK_RETURN:
                        if (opc_seleccionada == 0) {
                            Mix_PlayChannel(-1, game->sel_menu, 0);
                            en_menu = false;
                        } else if (opc_seleccionada == 1) {
                            Mix_PlayChannel(-1, game->sel_menu, 0);
                            SDL_Log("Ranking desde keyboard\n\n");
                            panel_ranking(game);
                        } else if (opc_seleccionada == 2){
                            Mix_PlayChannel(-1, game->sel_menu, 0);
                            game->quit = true;
                            en_menu = false;
                        }
                        break;
                    case SDLK_SPACE:
                        SDL_Log("Menu config\n\n");
                        Mix_PlayChannel(-1, game->sel_menu, 0);
                        panel_config(game);
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
                    Mix_PlayChannel(-1, game->sel_menu, 0);
                    en_menu = false;
                } else if (mouse_rect(mouse_x, mouse_y, &botones[1])) {
                    SDL_Log("Ranking desde keyboard\n\n");
                    Mix_PlayChannel(-1, game->sel_menu, 0);
                    panel_ranking(game);
                } else if (mouse_rect(mouse_x, mouse_y, &botones[2])) {
                    Mix_PlayChannel(-1, game->sel_menu, 0);
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

        // renderizar texto plateado Config
        SDL_Rect r_config = {
            20,
            (int)(game->pantalla.win_w)*0.6,
            (int)(game->pantalla.win_w)*0.25,
            15
        };
        SDL_RenderCopy(game->pantalla.renderer, textura_config, NULL, &r_config);
        
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

void panel_ranking(Game *game)
{
    // lee ultimas 10
    int lineas_deseadas = 10;
    int selecciona_niv = 1;

    SDL_Color gris = {150,150,150,255}; // titulo gris
    SDL_Color negro = {255,255,255,255}; // puntajes negro

    bool visible = true;

    while(visible && !game->quit) {
        // para formateo con snprintf
        char ruta_archivo[64];
        char texto_titulo[64];
        snprintf(ruta_archivo, sizeof(ruta_archivo), "./data/score/score_%d.txt", selecciona_niv);
        snprintf(texto_titulo, sizeof(texto_titulo), "--- RANKING ULTIMOS %d NIVEL %d ---", lineas_deseadas, selecciona_niv);

        SDL_Surface *s_titulo = TTF_RenderText_Solid(game->fuente, texto_titulo, gris);
        SDL_Texture *t_titulo = SDL_CreateTextureFromSurface(game->pantalla.renderer, s_titulo);
        int t_w = s_titulo->w;
        int t_h = s_titulo->h;
        SDL_FreeSurface(s_titulo);

        SDL_Surface *s_subtitulo = TTF_RenderText_Solid(game->fuente, "Enter o click: salir | Flechas: selecc. nivel", gris);
        SDL_Texture *t_subtitulo = SDL_CreateTextureFromSurface(game->pantalla.renderer, s_subtitulo);
        SDL_FreeSurface(s_subtitulo);

        char linea[100]; // buffer
        int punt_w[10];
        int punt_h[10];

        SDL_Texture *t_puntajes[lineas_deseadas];
        FILE *archivo = fopen(ruta_archivo, "r");
        int total_lineas = 0;
        int total_pjes = 0;

        if (archivo) {
            int saltar = 0;

            while (fgets(linea, sizeof(linea), archivo)){
                total_lineas++; // cuenta cuantos scores hay
            }

            if (total_lineas > 10) {
                saltar = total_lineas-10;
            }

            // vuelve archivo desde el principio func stdio
            rewind(archivo);

            int j = 0;
            while (total_pjes < lineas_deseadas && fgets(linea, sizeof(linea), archivo))
            {
                if (j >= saltar) {
                    for (int i=0; linea[i]; i++) {
                        if (linea[i] == '\r' || linea[i] == '\n') {
                            linea[i] = '\0';
                            break;
                        }
                    }

                    SDL_Surface *s = TTF_RenderText_Solid(game->fuente, linea, negro);
                    if (s) {
                        t_puntajes[total_pjes] = SDL_CreateTextureFromSurface(game->pantalla.renderer, s);
                        punt_w[total_pjes] = s->w;
                        punt_h[total_pjes] = s->h;
                        SDL_FreeSurface(s);
                        total_pjes++;
                    }
                }
                j++;
            }
            fclose(archivo);
        }

        if (total_pjes == 0) {
            strcpy(linea, "Aun no hay puntajes.");
            SDL_Surface *s = TTF_RenderText_Solid(game->fuente, linea, negro);
            t_puntajes[0] = SDL_CreateTextureFromSurface(game->pantalla.renderer, s);
            punt_w[0] = s->w;
            punt_h[0] = s->h;
            SDL_FreeSurface(s);
            total_pjes = 1;
        }

        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                game->quit = true;
                visible = false;
            }
            if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) {
                visible = false;
            }
            if (ev.type == SDL_KEYDOWN) {
                if (ev.key.keysym.sym == SDLK_ESCAPE || ev.key.keysym.sym == SDLK_RETURN) {
                    visible = false;
                }
                // seleccion de nivel ranking
                if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_DOWN) {
                    if (selecciona_niv < 4) {
                        selecciona_niv++;
                    } else {
                        selecciona_niv = 1;
                    }
                    Mix_PlayChannel(-1, game->sel_menu, 0);
                }
                if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_UP) {
                    if (selecciona_niv > 1) {
                        selecciona_niv --;
                        
                    } else {
                        selecciona_niv = 4;
                    } 
                    Mix_PlayChannel(-1, game->sel_menu, 0);
                }
            }
        }

        // fondo bg
        SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->pantalla.renderer);
        if (game->texturaImg) {
            SDL_RenderCopy(game->pantalla.renderer, game->texturaImg, NULL, NULL);
        }

        // rectangulo oscuro atras texto
        SDL_SetRenderDrawBlendMode(game->pantalla.renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 0, 0, 200);
        SDL_Rect fondo = { 5, 5, game->pantalla.win_w - 10, game->pantalla.win_h - 10 };
        SDL_RenderFillRect(game->pantalla.renderer, &fondo);

        // titulo
        SDL_Rect r_titulo = {
            (game->pantalla.win_w - t_w)/2,
            30,
            t_w,
            t_h
        };
        SDL_RenderCopy(game->pantalla.renderer, t_titulo, NULL, &r_titulo);

        SDL_Rect r_subtitulo = {
            (game->pantalla.win_w - t_w)/2,
            (game->pantalla.win_h - t_h) - 30,
            (int)(game->pantalla.win_w)*0.7,
            t_h
        };
        SDL_RenderCopy(game->pantalla.renderer, t_subtitulo, NULL, &r_subtitulo);

        // Dibuja lista puntajes
        int y = 100;
        for (int i=0; i<total_pjes; i++) {
            SDL_Rect r_linea = {
                (int)(game->pantalla.win_w) / 6,
                y,
                punt_w[i]/2,
                punt_h[i]/2,
                };
            SDL_RenderCopy(game->pantalla.renderer, t_puntajes[i], NULL, &r_linea);
            y+=20;
        }

        SDL_RenderPresent(game->pantalla.renderer);

        SDL_DestroyTexture(t_titulo);
        SDL_DestroyTexture(t_subtitulo);
        
        for (int i=0; i<total_pjes; i++) {
            SDL_DestroyTexture(t_puntajes[i]);
        }
        SDL_Delay(16);
    }
}

void panel_config(Game *game)
{
    SDL_Color gris = {150,150,150,255}; // titulo gris
    SDL_Color blanco = {255,255,255,255}; // texto blanco
    bool visible = true;

    char *opciones[2] = {"Fullscreen: OFF", "Volumen: 100%"};

    while(visible && !game->quit) {
        SDL_Surface *s_titulo = TTF_RenderText_Solid(game->fuente, "Config / Opciones", gris);
        SDL_Texture *t_titulo = SDL_CreateTextureFromSurface(game->pantalla.renderer, s_titulo);
        int t_w = s_titulo->w;
        int t_h = s_titulo->h;
        SDL_FreeSurface(s_titulo);

        SDL_Texture *t_textos[2];
        int opc_w[2];
        int opc_h[2];

        for (int i=0; i<2; i++){
            SDL_Surface *s_opciones = TTF_RenderText_Solid(game->fuente, opciones[i], blanco);
            t_textos[i] = SDL_CreateTextureFromSurface(game->pantalla.renderer, s_opciones);
            opc_w[i] = s_opciones->w;
            opc_h[i] = s_opciones->h;
            SDL_FreeSurface(s_opciones);
        }

        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                game->quit = true;
                visible = false;
            }

            if (ev.type == SDL_KEYDOWN) {
                if (ev.key.keysym.sym == SDLK_SPACE || ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_ESCAPE){
                    visible = false;
                }
            }

            if (ev.type == SDL_MOUSEBUTTONDOWN || ev.button.button == SDL_BUTTON_LEFT) {
                visible = false;
            }
        }

        // renderizado
        SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->pantalla.renderer);

        // fondo bg
        if (game->texturaImg) {
            SDL_RenderCopy(game->pantalla.renderer, game->texturaImg, NULL, NULL);
        }

        // rectangulo oscuro atras texto
        SDL_SetRenderDrawBlendMode(game->pantalla.renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 0, 0, 200);
        SDL_Rect fondo = { 5, 5, game->pantalla.win_w - 10, game->pantalla.win_h - 10 };
        SDL_RenderFillRect(game->pantalla.renderer, &fondo);

        // titulo
        SDL_Rect r_titulo = {
            (game->pantalla.win_w - t_w)/2,
            30,
            t_w,
            t_h
        };
        SDL_RenderCopy(game->pantalla.renderer, t_titulo, NULL, &r_titulo);

        int y = 120;
        for (int i=0; i<2; i++) {
            SDL_Rect r_opc = {
                (int)(game->pantalla.win_w - opc_w[i])/2,
                y,
                opc_w[i],
                opc_h[i]
            };
            SDL_RenderCopy(game->pantalla.renderer, t_textos[i], NULL, &r_opc);
            y += 50;
        }

        // se debe presentar el render
        SDL_RenderPresent(game->pantalla.renderer);

        // limpia
        SDL_DestroyTexture(t_titulo);
        for (int i=0; i<2; i++){
            SDL_DestroyTexture(t_textos[i]);
        }
        SDL_Delay(16);
    }
}