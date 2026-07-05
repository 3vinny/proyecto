// render.c
#include "headers.h"

void game_Render(Game *game)
{
    int x = 0, y = 0;
    
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);

    SDL_RenderClear(game->renderer);

    // BACKGROUND
    SDL_RenderCopy(game->renderer, game->texturaImg, NULL, NULL);

    // TEXT
    SDL_QueryTexture(game->texturaTexto, NULL, NULL, &game->wText, &game->hText);
    SDL_Rect textoRec = { 200, 50, game->wText, game->hText }; // posicion texto, ancho y alto
    SDL_RenderCopy(game->renderer, game->texturaTexto, NULL, &textoRec);

    // TEXTO 2
    SDL_QueryTexture(game->texturaTexto2, NULL, NULL, &game->wText, &game->hText);
    SDL_Rect textoRec2 = { 500, 600, game->wText, game->hText };
    SDL_RenderCopy(game->renderer, game->texturaTexto2, NULL, &textoRec2);

    /* RECTANGULO (hitbox)
    SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(game->renderer, 237, 237, 255, 125);
    SDL_Rect Hitbox = { game->h_x, game->h_y, game->h_w, game->h_h };
    SDL_RenderFillRect(game->renderer, &Hitbox);*/
    
    // RECTANGULO hitbox2
   /*  SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(game->renderer, 237, 237, 255, 125)
     SDL_Rect Hitbox2 = {
        w_inicial - game->w_colision,
       h_inicial - game->h_colision,
        game->w_colision,
       game->h_colision  
    };
     SDL_RenderFillRect(game->renderer, &Hitbox2);*/

    // RECTANGULO (Movible con teclado, cargado desde txt y actualizado en cargas.c)
    SDL_Rect Rectang = { (int)game->x, (int)game->y, game->lado, game->lado };
    SDL_RenderCopyEx(game->renderer, game->texturaJugador, NULL, &Rectang, game->angulo, NULL, SDL_FLIP_NONE);
   
    // RECTANGULO (leible desde archivo txt)
    SDL_SetRenderDrawColor(game->renderer, 0, 60, 255, 255);
    for (int i=0; i<tile_filas; i++){
       for(int j=0; j<(int)tile_cols; j++){
          if (game->tiles[i][j].activo){
             SDL_Rect r = {
                game->tiles[i][j].x_tiles,
                game->tiles[i][j].y_tiles,
                game->tiles[i][j].w_tiles,
                h_inicial
             };
             SDL_RenderFillRect(game->renderer, &r);
          }
       }
    }

    // RECTANGULO (Colision)
    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
    SDL_Rect Rectang_Colision = { game->x_colision, game->y_colision, game->w_colision, game->h_colision };
    SDL_RenderFillRect(game->renderer, &Rectang_Colision);
    
    // RECTANGULO (interactivo objeto2 txt)
    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
    for(int i=0; i<tile_filas; i++){
        for(int j=0; j<tile_cols; j++){
            if(game->tiles[i][j].objeto2){
                SDL_Rect Rectang_Obj2 = {
                    game->tiles[i][j].x_tiles,
                    game->tiles[i][j].y_tiles,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };
                SDL_RenderFillRect(game->renderer, &Rectang_Obj2);
            }
        }
    }

    SDL_RenderPresent(game->renderer);
}
