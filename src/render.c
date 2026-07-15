#include "headers.h"

int hText = 16;
int wText = 32;
// estandar pixel art 32x32

void game_Render(Game *game)
{
    SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->pantalla.renderer);

    // BACKGROUND
    SDL_Rect origen_pasto = { 224, 192, tam, tam };
    int desfase_x = -(game->pantalla.camara.x % tam);
    int desfase_y = -(game->pantalla.camara.y % tam);
    
    int x = 0, y = 0;
    
    for (y = desfase_y - tam; y < game->pantalla.win_h + tam; y = y+tam)
    {
        for (x = desfase_x - tam; x < game->pantalla.win_w + tam; x = x+tam)
        {
            SDL_Rect destino_pasto = { x, y, tam, tam };
            SDL_RenderCopy(game->pantalla.renderer, game->texturaPista, &origen_pasto, &destino_pasto);
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
    
                case 'E':
                    origen.x = 32; origen.y = 0; //(32,0) recta horiz 
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
                case 'P': // jugador
                    origen.x = tam; origen.y = 0;
                    break;
                    //case '.': break; //pasto
                case 'C':
                    origen.x = 384; origen.y = 144;
                    break;
                case 'c':
                    break;
                case 'A':
                    origen.x = 384; origen.y = 112;
                    break;
                case 'M':
                    origen.x = 384; origen.y = 208;
                    break;
                case 'X':
                    origen.x = tam; origen.y = 0; // objeto powerup : 'objeto2'
                    break;
                case 'N':
                    origen.x = 384; origen.y = 176;
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

    // RECTANGULO DEL JUGADOR !!! acaaa
    SDL_Rect Rectang = { 
        (int)game->jugador.x - game->pantalla.camara.x, 
        (int)game->jugador.y - game->pantalla.camara.y, 
        game->jugador.lado, 
        game->jugador.lado 
    };
    SDL_RenderCopyEx(game->pantalla.renderer, game->jugador.textura, NULL, &Rectang, game->jugador.angulo, NULL, SDL_FLIP_NONE);
   
    // TEXT
    SDL_QueryTexture(game->texturaTexto, NULL, NULL, &wText, &hText);
    SDL_Rect textoRec = { 0, 0, wText/2, hText/2 }; // posicion texto, ancho y alto
    SDL_RenderCopy(game->pantalla.renderer, game->texturaTexto, NULL, &textoRec);

    // TEXTO 2
    render_Cronometro(game);
    
    // RECTANGULO (interactivo objeto2 txt) destruible!!!!!!
    SDL_SetRenderDrawBlendMode(game->pantalla.renderer, SDL_BLENDMODE_BLEND);
    
    for (int i=0; i<tile_filas; i++) 
    {
        for (int j=0; j<tile_cols; j++)
        {
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
        }
    }
    
    // ENEMIGOs
    //
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
            
            if (game->texturaEnemigo != NULL)
            {
                SDL_RenderCopyEx(game->pantalla.renderer, game->texturaEnemigo, NULL, &rect_enm, 90.0, NULL, SDL_FLIP_NONE);
            } else {
                SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 50, 50, 255);
                SDL_RenderFillRect(game->pantalla.renderer, &rect_enm);
            }        
        }
    }
    SDL_RenderPresent(game->pantalla.renderer);
}

void render_Cronometro(Game *game)
{
    // PREVIENE FUGAS DE MEMORIA (ya paso :v)
    if (game->texturaTexto2 != NULL) 
    {
        SDL_DestroyTexture(game->texturaTexto2);
        game->texturaTexto2 = NULL;
    }
    
    SDL_Color colorBlanco = {255,255,255,155}; //rgb y transparencia
    SDL_Surface *surfaceTemporal = TTF_RenderText_Solid(game->fuente, game->texto_cronometro, colorBlanco);
    
    if (surfaceTemporal != NULL)
    {
        game->texturaTexto2 = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTemporal);
        SDL_FreeSurface(surfaceTemporal);
    }
    
    if (game->texturaTexto2 != NULL)
    {
        SDL_QueryTexture(game->texturaTexto2, NULL, NULL, &wText, &hText);
        SDL_Rect textoRec2 = { game->pantalla.win_w - wText, game->pantalla.win_h - hText, wText/2, hText/2 };
        SDL_RenderCopy(game->pantalla.renderer, game->texturaTexto2, NULL, &textoRec2);
    }
}
