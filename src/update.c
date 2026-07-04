#include "headers.h"

int chequea_tiles(Game *game, SDL_Rect *player_rect);

void game_Update(Game *game){
        
   int ancho_act = game->win_w;
   int alto_act = game->win_h;

   int i, j = 0;

   SDL_Rect temp1 = {
      .x = game->x,
      .y = game->y,
      .w = game->lado,
      .h = game->lado
   };

   SDL_Rect temp2 = {
      .x = game->h_x,
      .y = game->h_y,
      .w = game->h_w,
      .h = game->h_h
   };

   // x_colision, y_colision, w_colision, h_colision
   // el rojo
   SDL_Rect temp3 = {
      .x = game->x_colision,
      .y = game->y_colision,
      .w = game->w_colision,
      .h = game->h_colision
   };
   
   SDL_Rect temp6 = {
       .x = w_inicial - game->x_colision,
       .y = h_inicial - game->y_colision,
       .w = game->w_colision,
       .h = game->h_colision  
   };
   
   float paso = (game->delta_time)*(game->velocidad);
   temp1.x = (int)game->x;
   temp1.y = (int)game->y;
        
   if (game->right == 1)
   {
      temp1.x = (int)(game->x + paso);
      if(!SDL_HasIntersection(&temp1, &temp2) && !SDL_HasIntersection(&temp1, &temp3) && !chequea_tiles(game, &temp1) && temp1.x <= ancho_act - game->lado){
         game->x += paso;
      }
   } else if (game->left == 1) {
      temp1.x = (int)(game->x - paso);
      if(!SDL_HasIntersection(&temp1, &temp2) && !SDL_HasIntersection(&temp1, &temp3) && !chequea_tiles(game, &temp1) && temp1.x >= 0)
      {
          game->x -= paso;
      }
   }
   
   temp1.x = (int)game->x;
        
   if (game->down == 1)
   {
      temp1.y = (int)(game->y + paso);
      if(!SDL_HasIntersection(&temp1, &temp2) && !SDL_HasIntersection(&temp1, &temp3) && !chequea_tiles(game, &temp1) && temp1.y <= alto_act - game->lado)
      {
         game->y += paso;
      }
   } else if (game->up == 1)
   {
      temp1.y = (int)(game->y - paso);
      
      if(!SDL_HasIntersection(&temp1, &temp2) && !SDL_HasIntersection(&temp1, &temp3) && !chequea_tiles(game, &temp1) && temp1.y >= 0){
         game->y -= paso;
      }
   }
   
   // objeto cargado desde txt caera en cascada
   for(int i=0; i<tile_filas; i++){
        for(int j=0; j<tile_cols; j++){
           if(game->tiles[i][j].activo){
               if (!chequea_tiles(game, &temp1)){
                  game->tiles[i][j].y_tiles += 200 * (game->delta_time);
               }
           }
       }
   }
}

int chequea_tiles(Game *game, SDL_Rect *player_rect) {
    for (int i = 0; i < tile_filas; i++) {
        for (int j = 0; j < (int)tile_cols; j++) {
            if (game->tiles[i][j].activo) {
                SDL_Rect temp4 = {
                    .x = game->tiles[i][j].x_tiles,
                    .y = game->tiles[i][j].y_tiles,
                    .w = game->tiles[i][j].w_tiles,
                    .h = game->tiles[i][j].h_tiles
                };
                
                if (SDL_HasIntersection(player_rect, &temp4)) {
                    return 1; 
                }
            }
        }
    }
    
    // RECTANGULO (interactivo objeto2 txt)
    for(int i=0; i<tile_filas; i++){
        for(int j=0; j<tile_cols; j++){
            if(game->tiles[i][j].objeto2){
                SDL_Rect temp5 = {
                    game->tiles[i][j].x_tiles,
                    game->tiles[i][j].y_tiles,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };
                if (SDL_HasIntersection(player_rect, &temp5)) {
                    game->tiles[i][j].objeto2 = false; //desaparece el objeto
                }
            }
        }
    }
    return 0; 
}
