// render.c
#include "headers.h"

void game_Render(Game *game)
{
    int x = 0, y = 0;
    
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);

    SDL_RenderClear(game->renderer);

    // BACKGROUND
    SDL_RenderCopy(game->renderer, game->texturaImg, NULL, NULL);

   
    
     // Tilemap dibuja pista desde la textura y sdlrect
    for (int i=0; i < tile_filas; i++)
    {
        for (int j=0; j < (int)tile_cols; j++)
        {
            char tipo = game->tiles[i][j].tipo; //tipo de caracter en txt
            bool dibuja_pista = true;
            
            SDL_Rect destino = {
                game->tiles[i][j].x_tiles,
                game->tiles[i][j].y_tiles,
                game->tiles[i][j].w_tiles,
                game->tiles[i][j].h_tiles
            }; 
            
            // POSICIONES A RECORTAR:
            
            int tam = 32; // estandar pixel art 32x32
            SDL_Rect origen = { 0,0,tam,tam };
            switch(tipo) {
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
                case '.': //ASFALTO !!!
                    origen.x = 192; origen.y = 192; // 144, 192 COORDENADAS
                    break;
                case 'P': // jugador
                    origen.x = tam; origen.y = 0;
                    break;
                default: // si no es ninguno el booleano se cambia
                    dibuja_pista = false;
                    break;
            }
            
            if (dibuja_pista && game->texturaPista != NULL)
            {
                SDL_RenderCopy(game->renderer, game->texturaPista, &origen, &destino);    
            }
        }
    }

    // RECTANGULO (Movible con teclado, cargado desde txt y actualizado en cargas.c)
    SDL_Rect Rectang = { (int)game->x, (int)game->y, game->lado, game->lado };
    SDL_RenderCopyEx(game->renderer, game->texturaJugador, NULL, &Rectang, game->angulo, NULL, SDL_FLIP_NONE);
   
    // TEXT
    SDL_QueryTexture(game->texturaTexto, NULL, NULL, &game->wText, &game->hText);
    SDL_Rect textoRec = { 200, 50, game->wText, game->hText }; // posicion texto, ancho y alto
    SDL_RenderCopy(game->renderer, game->texturaTexto, NULL, &textoRec);

    // TEXTO 2
    SDL_QueryTexture(game->texturaTexto2, NULL, NULL, &game->wText, &game->hText);
    SDL_Rect textoRec2 = { 500, 600, game->wText, game->hText };
    SDL_RenderCopy(game->renderer, game->texturaTexto2, NULL, &textoRec2);

    // RECTANGULO (Colision)
    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
    SDL_Rect Rectang_Colision = { game->y_colision, game->y_colision, game->w_colision, game->h_colision };
    SDL_RenderFillRect(game->renderer, &Rectang_Colision);
    
    // RECTANGULO (interactivo objeto2 txt)
    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
    for (int i=0; i<tile_filas; i++) {
        for (int j=0; j<tile_cols; j++) {
            if (game->tiles[i][j].objeto2) {
                SDL_Rect Rectang_Obj2 = {
                    game->tiles[i][j].x_tiles,
                    game->tiles[i][j].y_tiles,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].w_tiles
                };
                SDL_RenderFillRect(game->renderer, &Rectang_Obj2);
            }
        }
    }

    SDL_RenderPresent(game->renderer);
}
