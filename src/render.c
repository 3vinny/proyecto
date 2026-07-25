#include "headers.h"

int hText = 16;
int wText = 32;
// estandar pixel art 32x32

void render_contbalas(Game *game);
void render_hp(Game *game); // render.c
void render_Cronometro(Game *game); // render.c
void render_nivel(Game *game);

void game_Render(Game *game)
{
    SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->pantalla.renderer);

    SDL_Rect origen_fondo;
    
    if (game->nivel_actual == 2) {
        origen_fondo = (SDL_Rect){ 112, 0, tam, tam };
    } else if (game->nivel_actual == 3) {
        origen_fondo = (SDL_Rect){ 144, 192, tam, tam };
    } else {
        origen_fondo = (SDL_Rect){ 224, 192, tam, tam };
    }
    
    // DESFASE CAMARA
    int desfase_x = -(game->pantalla.camara.x % tam);
    int desfase_y = -(game->pantalla.camara.y % tam);
    
    int x = 0, y = 0;
    
    for (y = desfase_y - tam; y < game->pantalla.win_h + tam; y = y+tam)
    {
        for (x = desfase_x - tam; x < game->pantalla.win_w + tam; x = x+tam)
        {
            SDL_Rect destino_fondo = { x, y, tam, tam };
            SDL_RenderCopy(game->pantalla.renderer, game->texturaPista, &origen_fondo, &destino_fondo);
        }
    }
    
     // Tilemap dibuja pista desde la textura y sdlrect
    for (int i=0; i < tile_filas; i++)
    {
        for (int j=0; j < (int)tile_cols; j++)
        {
            char tipo = game->tiles[i][j].tipo; //tipo de caracter en txt
            bool dibuja_pista = true;
            
            SDL_Rect destino = {
                game->tiles[i][j].x_tiles - game->pantalla.camara.x,
                game->tiles[i][j].y_tiles - game->pantalla.camara.y,
                game->tiles[i][j].w_tiles,
                game->tiles[i][j].h_tiles
            }; 
            
            // POSICIONES A RECORTAR:
            
            SDL_Rect origen = { 0,0,tam,tam };
            switch(tipo) 
            {
                case '1':
                    origen.x = 0; origen.y = 0; //(0,0) superior izq
                    break;
                case '2':
                    origen.x = 64; origen.y = 0; //(128,0) superior derecha
                    break;
                case '3':
                    origen.x = 0; origen.y = 64; // (0,128) inferior izq
                    break;
                case '4':
                    origen.x = 64; origen.y = 64; // (128,128) inferior derecha
                    break;
                    
                case '5':
                    origen.x = 272; origen.y = 112;
                    break;
                case '6':
                    origen.x = 336; origen.y = 112;
                    break;
                case '7':
                    origen.x = 272; origen.y = 176;
                    break;
                case '8':
                    origen.x = 336; origen.y = 176;
                    break;
    
                case '-':
                    origen.x = 32; origen.y = 0; //(32,0) recta horiz 
                    break;
                case '+':
                    origen.x = 32; origen.y = 64;
                    break;
                case '=':
                    origen.x = 304; origen.y = 112;
                    break;
               
                case '|':
                    origen.x = 0; origen.y = tam; //recta vertical (0, 64)
                    break;
                case 'I':
                    origen.x = 64; origen.y = tam;
                    break;
                case 'i':
                    origen.x = 272; origen.y = 144;
                    break;
                case 'l':
                    origen.x = 64; origen.y = 32;
                    break;
               
                case '#': //asfalto !!!
                    origen.x = 144; origen.y = 192; // 144, 192 COORDENADAS
                    break;
                case 'C':
                    origen.x = 384; origen.y = 144;
                    break;
                case 'c':
                    origen.x = 272; origen.y = 208;
                    break;
                case 'A':
                    origen.x = 384; origen.y = 112;
                    break;
                case 'a':
                    origen.x = 384; origen.y = 112;
                    break;
                case 'B':
                    origen.x = 384; origen.y = 112;
                    break;
                case 'M':
                    origen.x = 384; origen.y = 208;
                    break;
                case 'N':
                    origen.x = 384; origen.y = 176;
                    break;
                case 'T':
                    origen.x = 336; origen.y = 208;
                    break;
                default: // si no es ninguno el booleano se cambia
                    dibuja_pista = false;
                    break;
            }
            
            if (dibuja_pista && game->texturaPista != NULL)
            {
                SDL_RenderCopy(game->pantalla.renderer, game->texturaPista, &origen, &destino);    
            }
        }
    }

    // TEXT
    SDL_QueryTexture(game->interfaz.texturaTexto, NULL, NULL, &wText, &hText);
    SDL_Rect textoRec = { 0, 0, wText/2, hText/2 }; // posicion texto, ancho y alto
    SDL_RenderCopy(game->pantalla.renderer, game->interfaz.texturaTexto, NULL, &textoRec);

    // TEXTO 2
    //render_Cronometro(game);

    // TEXTO 3
    
    // HP
    sprintf(game->interfaz.texto_HP, "HP: %02d", game->jugador.hp);
    // BALAS
    sprintf(game->interfaz.texto_Balas, "Balas restantes: %02d", game->jugador.contador_balas);
    // NIVEL
    sprintf(game->interfaz.texto_Nivel, "Nivel: %01d", game->nivel_actual);
    
    render_hp(game);
    render_contbalas(game);
    render_nivel(game);
    
    // RECTANGULO (interactivo objeto2 txt) destruible!!!!!!
    SDL_SetRenderDrawBlendMode(game->pantalla.renderer, SDL_BLENDMODE_BLEND);
    
    for (int i=0; i<tile_filas; i++) 
    {
        for (int j=0; j<tile_cols; j++)
        {
            // caja
            if (game->tiles[i][j].objeto2)
            {
                SDL_Rect Rectang_Obj2 = {
                    game->tiles[i][j].x_tiles - game->pantalla.camara.x,
                    game->tiles[i][j].y_tiles - game->pantalla.camara.y,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };

                if (game->texturaCaja != NULL)
                {
                    SDL_RenderCopy(game->pantalla.renderer, game->texturaCaja, NULL, &Rectang_Obj2); 
                } else {
                    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 0, 0, 105);
                    SDL_RenderFillRect(game->pantalla.renderer, &Rectang_Obj2);
                }
            }
            
            if (game->tiles[i][j].objeto3)
            {
                SDL_Rect Rectang_Obj3 = {
                    game->tiles[i][j].x_tiles - game->pantalla.camara.x,
                    game->tiles[i][j].y_tiles - game->pantalla.camara.y,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };
                if (game->texturaCajaBalas != NULL)
                {
                    SDL_RenderCopy(game->pantalla.renderer, game->texturaCajaBalas, NULL, &Rectang_Obj3); 
                } else {
                    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 0, 0, 105);
                    SDL_RenderFillRect(game->pantalla.renderer, &Rectang_Obj3);
                }
            }

            if (game->tiles[i][j].aceite)
            {
                SDL_Rect Rectang_Aceite = {
                    game->tiles[i][j].x_tiles - game->pantalla.camara.x,
                    game->tiles[i][j].y_tiles - game->pantalla.camara.y,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };

                if (game->texturaAceite != NULL) {
                    SDL_RenderCopy(game->pantalla.renderer, game->texturaAceite, NULL, &Rectang_Aceite);
                } else {
                    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 0, 0, 105);
                    SDL_RenderFillRect(game->pantalla.renderer, &Rectang_Aceite);
                }
            }
        }
    }
    
    // CICLO QUE DIBUJA ENEMIGOS
    for (int i = 0; i < max_enemigos; i++)
    {
        if (game->enemigos[i].activo)
        {
            SDL_Rect rect_enm = {
                (int)game->enemigos[i].x - game->pantalla.camara.x,
                (int)game->enemigos[i].y - game->pantalla.camara.y,
                game->enemigos[i].lado,
                game->enemigos[i].lado
            };

            // si no entra al if devuelve patrulla (default)
            SDL_Texture *textura_enemigo = game->texturaEnemigo;
            if (game->enemigos[i].es_camion == true) {
                textura_enemigo = game->texturaEnemigo3;
            } else if (game->enemigos[i].es_bote == true) {
                textura_enemigo = game->texturaEnemigo4;
            }

            if (textura_enemigo != NULL)
            {
                SDL_RenderCopyEx(game->pantalla.renderer, textura_enemigo, NULL, &rect_enm, game->enemigos[i].angulo, NULL, SDL_FLIP_NONE);
            } else {
                SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 50, 50, 255);
                SDL_RenderFillRect(game->pantalla.renderer, &rect_enm);
            }        
        }
    }
    
    // DIBUJA PROYECTILES/BALAS DEL ENEMIGO
    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 255, 0, 255);
    for (int i=0; i<max_enemigos; i++) {
        for (int p=0; p<MAX_PROYECTILES; p++) {
            if (game->enemigos[i].proyectiles[p].activo) {
                SDL_Rect rect_bala = {
                    (int)(game->enemigos[i].proyectiles[p].x) - game->pantalla.camara.x,
                    (int)(game->enemigos[i].proyectiles[p].y) - game->pantalla.camara.y,
                    game->enemigos[i].proyectiles[p].lado,
                    game->enemigos[i].proyectiles[p].lado
                };
                SDL_RenderFillRect(game->pantalla.renderer, &rect_bala);
            }
        } 
    }

    // DIBUJA PROYECTILES/BALAS DEL JUGADOR
    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 255, 0, 255);
    for (int p=0; p<MAX_PROYECTILES; p++) {
        if (game->jugador.proyectiles[p].activo) {
            SDL_Rect rect_bala_jug = {
                (int)(game->jugador.proyectiles[p].x) - game->pantalla.camara.x,
                (int)(game->jugador.proyectiles[p].y) - game->pantalla.camara.y,
                game->jugador.proyectiles[p].lado,
                game->jugador.proyectiles[p].lado
            };
            SDL_RenderFillRect(game->pantalla.renderer, &rect_bala_jug);
        }
    }

    // RECTANGULO DEL JUGADOR !!! acaaa
    SDL_Rect Rectang = { 
        (int)game->jugador.x - game->pantalla.camara.x, 
        (int)game->jugador.y - game->pantalla.camara.y, 
        game->jugador.lado, 
        game->jugador.lado 
    };
    SDL_RenderCopyEx(game->pantalla.renderer, game->jugador.textura, NULL, &Rectang, game->jugador.angulo, NULL, SDL_FLIP_NONE);
   
    // balas del jugador
    for(int p=0; p<MAX_PROYECTILES; p++) {
        if(game->jugador.proyectiles[p].activo) {
            SDL_Rect rect_bala = {
                (int)(game->jugador.proyectiles[p].x) - game->pantalla.camara.x,
                (int)(game->jugador.proyectiles[p].y) - game->pantalla.camara.y,
                game->jugador.proyectiles[p].lado,
                game->jugador.proyectiles[p].lado
            };
        }
    }
    
    SDL_RenderPresent(game->pantalla.renderer);
}

void render_Cronometro(Game *game)
{
    // PREVIENE FUGAS DE MEMORIA (ya paso :v)
    if (game->interfaz.texturaTexto2 != NULL) 
    {
        SDL_DestroyTexture(game->interfaz.texturaTexto2);
        game->interfaz.texturaTexto2 = NULL;
    }
    
    SDL_Color colorBlanco = {255,255,255,155}; //rgb y transparencia
    SDL_Surface *surfaceTemporal = TTF_RenderText_Solid(game->fuente, game->interfaz.texto_cronometro, colorBlanco);
    
    if (surfaceTemporal != NULL)
    {
        game->interfaz.texturaTexto2 = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTemporal);
        SDL_FreeSurface(surfaceTemporal);
    }
    
    if (game->interfaz.texturaTexto2 != NULL)
    {
        SDL_QueryTexture(game->interfaz.texturaTexto2, NULL, NULL, &wText, &hText);
        SDL_Rect textoRec2 = { game->pantalla.win_w - wText, game->pantalla.win_h - hText, wText/2, hText/2 };
        SDL_RenderCopy(game->pantalla.renderer, game->interfaz.texturaTexto2, NULL, &textoRec2);
    }
}

void render_hp(Game *game)
{
    if (game->interfaz.texturaHP != NULL) {
        SDL_DestroyTexture(game->interfaz.texturaHP);
        game->interfaz.texturaHP = NULL;
    }

    SDL_Color colorBlanco = {255,255,255,255}; //rgb y transparencia
    SDL_Surface *surfaceTemporal = TTF_RenderText_Solid(game->fuente, game->interfaz.texto_HP, colorBlanco);

    if (surfaceTemporal != NULL)
    {
        game->interfaz.texturaHP = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTemporal);
        SDL_FreeSurface(surfaceTemporal);
    }

    if (game->interfaz.texturaHP != NULL)
    {
        SDL_QueryTexture(game->interfaz.texturaHP, NULL, NULL, &wText, &hText);
        SDL_Rect textoRec2 = { 0, game->pantalla.win_h - hText, wText/2, hText/2 };
        SDL_RenderCopy(game->pantalla.renderer, game->interfaz.texturaHP, NULL, &textoRec2);
    }
}

void render_contbalas(Game *game)
{
    if (game->interfaz.texturaBalas != NULL) {
        SDL_DestroyTexture(game->interfaz.texturaBalas);
        game->interfaz.texturaBalas = NULL;
    }
    
    SDL_Color colorBlanco = {255,255,255,255}; //rgb y transparencia
    SDL_Surface *surfaceTemporal = TTF_RenderText_Solid(game->fuente, game->interfaz.texto_Balas, colorBlanco);
    
    if (surfaceTemporal != NULL)
    {
        game->interfaz.texturaBalas = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTemporal);
        SDL_FreeSurface(surfaceTemporal);
    }
    
    if (game->interfaz.texturaBalas != NULL)
    {
        SDL_QueryTexture(game->interfaz.texturaBalas, NULL, NULL, &wText, &hText);
        SDL_Rect textoRec = { 0, game->pantalla.win_h - 2*hText, wText/2, hText/2 };
        SDL_RenderCopy(game->pantalla.renderer, game->interfaz.texturaBalas, NULL, &textoRec);
    }
}

void render_nivel(Game *game)
{
    if (game->interfaz.texturaNivel != NULL) {
        SDL_DestroyTexture(game->interfaz.texturaNivel);
        game->interfaz.texturaNivel = NULL;
    }
    
    SDL_Color colorBlanco = {255,255,255,255};
    SDL_Surface *surfaceTemporal = TTF_RenderText_Solid(game->fuente, game->interfaz.texto_Nivel, colorBlanco);
    
    if (surfaceTemporal != NULL)
    {
    game->interfaz.texturaNivel = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTemporal);
    SDL_FreeSurface(surfaceTemporal);
    }
    
    if (game->interfaz.texturaNivel != NULL)
    {
    SDL_QueryTexture(game->interfaz.texturaNivel, NULL, NULL, &wText, &hText);
    SDL_Rect textoRec = { game->pantalla.win_w - wText, 0, wText/2, hText/2 };
    SDL_RenderCopy(game->pantalla.renderer, game->interfaz.texturaNivel, NULL, &textoRec);
    }
}