#include "headers.h"

int hText = 32;
int wText = 32;

void game_Render(Game *game)
{
    SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->pantalla.renderer);

    // BACKGROUND
    SDL_RenderCopy(game->pantalla.renderer, game->texturaImg, NULL, NULL);
    
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
            int tam = 32; // estandar pixel art 32x32
            SDL_Rect origen = { 0,0,tam,tam };
            switch(tipo) 
            {
                case '1':
                    origen.x = 0; origen.y = 0; //(0,0) superior izq
                    break;
                case '-':
                    origen.x = tam; origen.y = 0; //(32,0) recta horiz 
                    break;
                case 'X':
                    origen.x = tam; origen.y = 0; // objeto powerup : 'objeto2'
                    break;
                case '2':
                    origen.x = 2*tam; origen.y = 0; //(128,0) superior derecha
                    break;
                case '|':
                    origen.x = 0; origen.y = tam; //recta vertical (0, 64)
                    break;
                case '3':
                    origen.x = 0; origen.y = 2*tam; // (0,128) inferior izq
                    break;
                case '4':
                    origen.x = 2*tam; origen.y = 2*tam; // (128,128) inferior derecha
                    break;
                case '#': //asfalto !!!
                    origen.x = 144; origen.y = 192; // 144, 192 COORDENADAS
                    break;
                case 'P': // jugador
                    origen.x = tam; origen.y = 0;
                    break;
                case '.': // pasto
                    origen.x = 224; origen.y = 192;
                    break;
                case 'C':
                    origen.x = 384; origen.y = 144;
                    break;
                case 'c':
                    break;
                case 'N':
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
    SDL_Rect textoRec = { 200, 10, wText, hText }; // posicion texto, ancho y alto
    SDL_RenderCopy(game->pantalla.renderer, game->texturaTexto, NULL, &textoRec);

    // TEXTO 2
    render_Cronometro(game);

    /* RECTANGULO (Colision)*/
    
    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 0, 0, 255);
    SDL_Rect Rectang_Colision = { 
        game->jugador.x_colision - game->pantalla.camara.x, 
        game->jugador.y_colision - game->pantalla.camara.y, 
        game->jugador.w_colision, 
        game->jugador.h_colision 
    };
    SDL_RenderFillRect(game->pantalla.renderer, &Rectang_Colision);
    
    // RECTANGULO (interactivo objeto2 txt) destruible
    SDL_SetRenderDrawBlendMode(game->pantalla.renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 0, 0, 105);
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
                SDL_RenderFillRect(game->pantalla.renderer, &Rectang_Obj2);
            }
        }
    }
    // Todo al render
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
        SDL_Rect textoRec2 = { w_inicial - wText, h_inicial - hText, wText, hText };
        SDL_RenderCopy(game->pantalla.renderer, game->texturaTexto2, NULL, &textoRec2);
    }
}
