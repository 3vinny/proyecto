#include "headers.h"

#define ACELERACION 600.0f
#define FRICCION 1000.0f
#define REDUCE_COLISION 0.4f
#define DELAY_ARRANQUE 0.11f

int chequea_tiles(Game *game, SDL_Rect *player_rect);
void construir_rects(Game *game);

void game_Update(Game *game)
{
   int ancho_act = game->win_w;
   int alto_act = game->win_h;

   int i, j = 0;
   
   construir_rects(game);

   SDL_Rect temp1 = game->rect_jugador;
   SDL_Rect temp2 = game->rect_hitbox;
   // el rojo
   SDL_Rect temp3 = game->rect_colision;
   SDL_Rect temp6 = game->rect_colision2;
   
   /* direccion solicitada x input game->DIRECCION 
   condiciones para las fisicas del auto
   0=nada 1=derecha/abajo -1=izq/arriba */
   
   int dir_x_input = 0, dir_y_input = 0;
   if (game->right == 1)
   {
       dir_x_input = 1;
   } else if (game->left == 1)
   {
       dir_x_input = -1;
   }
   
   if (game->down == 1)
   {
       dir_y_input = 1;
   } else if (game->up == 1)
   {
       dir_y_input = -1;
   }
   
   /*Movimiento y invertir sentido*/
   bool movimiento = (dir_x_input != 0 || dir_y_input != 0);
   int invirtiendo = 0;
   
   if (dir_x_input != 0 && game->dir_x != 0 && dir_x_input != game->dir_x) invirtiendo = 1;
   if (dir_y_input != 0 && game->dir_y != 0 && dir_y_input != game->dir_y) invirtiendo = 1;
   float paso;
   
    /* usamos sistema d pasos con el auto ahora tenemos 3 casos:
       1. cuando se mete reversa 2. cuando ta en movimiento 3. cuando no se presiona nada */
  
   if (invirtiendo)
   {
       game->velocidad_actual -= FRICCION * game->delta_time;
       if (game->velocidad_actual <= 0.0f)
       {
           // si ya freno resetea el delay y cambia direccion dir_x
           game->velocidad_actual = 0.0f;   
           game->dir_x = dir_x_input;
           game->dir_y = dir_y_input;
           game->tiempo_arranque = 0.0f;
       }
       paso = 0.0f;
   } else if (movimiento)
   {
       game->dir_x = dir_x_input;
       game->dir_y = dir_y_input;
       
       if (dir_x_input == 0 && dir_y_input == -1) game->angulo = 0.0;
       else if (dir_x_input == 1 && dir_y_input == -1) game->angulo = 45.0;
       else if (dir_x_input == 1 && dir_y_input == 0) game->angulo = 90.0;
       else if (dir_x_input == 1 && dir_y_input == 1) game->angulo = 135.0;
       else if (dir_x_input == 0 && dir_y_input == 1) game->angulo = 180.0;
       else if (dir_x_input == -1 && dir_y_input == 1) game->angulo = 225.0; 
       else if (dir_x_input == -1 && dir_y_input == 0) game->angulo = 270.0;
       else if (dir_x_input == -1 && dir_y_input == -1) game->angulo = 315.0;
       
       // aceleracion
       if (game->velocidad_actual <= 0.0f)
       {
           game->tiempo_arranque += game->delta_time;
           if (game->tiempo_arranque >= DELAY_ARRANQUE)
           {
               game->velocidad_actual += ACELERACION * game->delta_time;
           }
       } else {
           game->velocidad_actual += ACELERACION * game->delta_time;
       }
       
       if (game->velocidad_actual > game->velocidad) game->velocidad_actual = game->velocidad;
       paso = (game->delta_time) * (game->velocidad_actual);
   } else {
       game->velocidad_actual -= FRICCION * game->delta_time;
       if (game->velocidad_actual < 0.0f) game->velocidad_actual = 0.0f;
       if (game->velocidad_actual == 0.0f)
       {
           game->dir_x = 0;
           game->dir_y = 0;
           game->tiempo_arranque = 0.0f;
       }
       paso = (game->delta_time) * (game->velocidad_actual);
   }
   
   // conversion flotante entero variables temporales
   temp1.x = (int)game->x;
   temp1.y = (int)game->y;
   
   /* gestion al presionar teclas: hitbox, velocidades, colisiones
   casos en y, casos en x
   */
        
   if (game->right == 1)
   {
      temp1.x = (int)(game->x + paso);
      if(!SDL_HasIntersection(&temp1, &temp3) && !SDL_HasIntersection(&temp1, &temp3) && !chequea_tiles(game, &temp1) && temp1.x <= ancho_act - game->lado)
      {
         game->x += paso;
         game->colisionando = 0;
      } else if (!game->colisionando) 
      {
          game->velocidad_actual *= REDUCE_COLISION;
          game->colisionando = 1;
      }
   } else if (game->left == 1) {
      temp1.x = (int)(game->x - paso);
      if(!SDL_HasIntersection(&temp1, &temp3) && !SDL_HasIntersection(&temp1, &temp3) && !chequea_tiles(game, &temp1) && temp1.x >= 0)
      {
          game->x -= paso;
          game->colisionando = 0;
      } else if (!game->colisionando) 
      {
          game->velocidad_actual *= REDUCE_COLISION;
          game->colisionando = 1;
      }
   }
   
   temp1.x = (int)game->x;
        
   if (game->down == 1)
   {
      temp1.y = (int)(game->y + paso);
      if(!SDL_HasIntersection(&temp1, &temp3) && !SDL_HasIntersection(&temp1, &temp3) && !chequea_tiles(game, &temp1) && temp1.y <= alto_act - game->lado)
      {
         game->y += paso;
         game->colisionando = 0;
      } else if (!game->colisionando) 
      {
          game->velocidad_actual *= REDUCE_COLISION;
          game->colisionando = 1;
      }
   } else if (game->up == 1)
   {
      temp1.y = (int)(game->y - paso);
      
      if(!SDL_HasIntersection(&temp1, &temp3) && !SDL_HasIntersection(&temp1, &temp3) && !chequea_tiles(game, &temp1) && temp1.y >= 0)
      {
         game->y -= paso;
         game->colisionando = 0;
      } else if (!game->colisionando) 
      {
          game->velocidad_actual *= REDUCE_COLISION;
          game->colisionando = 1;
      }
   }
}

int chequea_tiles(Game *game, SDL_Rect *player_rect)
{
    for (int i = 0; i < tile_filas; i++) 
    {
        for (int j = 0; j < (int)tile_cols; j++) 
        {
            if (game->tiles[i][j].activo) 
            {
                SDL_Rect temp4 = {
                    .x = game->tiles[i][j].x_tiles,
                    .y = game->tiles[i][j].y_tiles,
                    .w = game->tiles[i][j].w_tiles,
                    .h = h_inicial
                };
                
                if (SDL_HasIntersection(player_rect, &temp4)) {
                    return 1; 
                }
            }
        }
    }
    
    // RECTANGULO (interactivo objeto2 txt)
    for (int i=0; i<tile_filas; i++)
    {
        for (int j=0; j<tile_cols; j++)
        {
            if (game->tiles[i][j].objeto2)
            {
                SDL_Rect temp5 = {
                    game->tiles[i][j].x_tiles,
                    game->tiles[i][j].y_tiles,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };
                
                if (SDL_HasIntersection(player_rect, &temp5))
                {
                    game->tiles[i][j].objeto2 = false; //desaparece el objeto
                }
            }
        }
    }
    return 0; 
}

void construir_rects(Game *game)
{
    game->rect_jugador = (SDL_Rect) {
        .x = (int)game->x,
        .y = (int)game->y,
        .w = game->lado,
        .h = game->lado
    };
    game->rect_hitbox = (SDL_Rect) {
        .x = game->h_x,
        .y = game->h_y,
        .w = game->h_w,
        .h = game->h_h
    };
    game->rect_colision = (SDL_Rect) {
        .x = game->x_colision,
        .y = game->y_colision,
        .w = game->w_colision,
        .h = game->h_colision
    };
    game->rect_colision2 = (SDL_Rect) {
        .x = w_inicial - game->x_colision,
        .y = h_inicial - game->y_colision,
        .w = game->w_colision,
        .h = game->h_colision
    };
}