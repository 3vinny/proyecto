// archivo mas importante del proyecto. colisiones direccion y fisicas, etc
#include "headers.h"

#define ACELERACION 200.0f
#define FRICCION 1000.0f
#define REDUCE_COLISION 0.4f
#define DELAY_ARRANQUE 0.11f
#define POTENCIA_FRENO 1000.0f
#define LIMITE 0.0f

int chequea_tiles(Game *game, SDL_Rect *player_rect);
int chequea_enemigos(Game *game, SDL_Rect *player_rect);
void construir_rects(Game *game);

void game_Update(Game *game)
{
   int ancho_act = game->pantalla.win_w;
   int alto_act = game->pantalla.win_h;
   
   construir_rects(game);

   SDL_Rect temp1 = game->jugador.rect;
   SDL_Rect temp2 = game->jugador.rect_colision;
   
   /* direccion solicitada x input
   0=nada 1=derecha/abajo -1=izq/arriba */
   
   int dir_x_input = 0, dir_y_input = 0;
   if (game->jugador.right == 1)
   {
       dir_x_input = 1;
   } else if (game->jugador.left == 1)
   {
       dir_x_input = -1;
   }
   
   if (game->jugador.down == 1)
   {
       dir_y_input = 1;
   } else if (game->jugador.up == 1)
   {
       dir_y_input = -1;
   }
   
   /*Movimiento y invertir sentido*/
   
   bool movimiento = (dir_x_input != 0 || dir_y_input != 0);
   int invirtiendo = 0;
   
   if (dir_x_input != 0 && game->jugador.dir_x != 0 && dir_x_input != game->jugador.dir_x) invirtiendo = 1;
   if (dir_y_input != 0 && game->jugador.dir_y != 0 && dir_y_input != game->jugador.dir_y) invirtiendo = 1;
   float paso;
   
    /* usamos sistema d pasos con el auto ahora tenemos 3 casos:
   0. freno 
   1. cuando se mete reversa 
   2. cuando ta en movimiento 
   3. cuando no se presiona nada 
   */
   
   if (game->jugador.freno == 1) {
       game->jugador.velocidad_actual -= POTENCIA_FRENO * game->delta_time;
       if (game->jugador.velocidad_actual <= LIMITE) {

           game->jugador.velocidad_actual = LIMITE;
           game->jugador.dir_x = 0;
           game->jugador.dir_y = 0;
           game->jugador.tiempo_arranque = 0.0f;
       }
       paso = (game->delta_time) * (game->jugador.velocidad_actual);
       
   } else if (invirtiendo) {
       game->jugador.velocidad_actual -= FRICCION * game->delta_time;
       if (game->jugador.velocidad_actual <= 0.0f)
       {
           // si ya freno resetea el delay y cambia direccion dir_x
           game->jugador.velocidad_actual = 0.0f;   
           game->jugador.dir_x = dir_x_input;
           game->jugador.dir_y = dir_y_input;
           game->jugador.tiempo_arranque = 0.0f;
       }
       paso = 0.0f;
       
   } else if (movimiento) {
       game->jugador.dir_x = dir_x_input;
       game->jugador.dir_y = dir_y_input;
       
       // ANGULOS: 0 45 90 135 180 225 270 315
       if (dir_x_input == 0 && dir_y_input == -1) game->jugador.angulo = 0.0;
       else if (dir_x_input == 1 && dir_y_input == -1) game->jugador.angulo = 45.0;
       else if (dir_x_input == 1 && dir_y_input == 0) game->jugador.angulo = 90.0;
       else if (dir_x_input == 1 && dir_y_input == 1) game->jugador.angulo = 135.0;
       else if (dir_x_input == 0 && dir_y_input == 1) game->jugador.angulo = 180.0;
       else if (dir_x_input == -1 && dir_y_input == 1) game->jugador.angulo = 225.0; 
       else if (dir_x_input == -1 && dir_y_input == 0) game->jugador.angulo = 270.0;
       else if (dir_x_input == -1 && dir_y_input == -1) game->jugador.angulo = 315.0;
       
       // aceleracion
       if (game->jugador.velocidad_actual <= 0.0f) {
           game->jugador.tiempo_arranque += game->delta_time;
           if (game->jugador.tiempo_arranque >= DELAY_ARRANQUE) {
               game->jugador.velocidad_actual += ACELERACION * game->delta_time;
           }
       } else {
           game->jugador.velocidad_actual += ACELERACION * game->delta_time;
       }
       
       if (game->jugador.velocidad_actual > game->jugador.velocidad) game->jugador.velocidad_actual = game->jugador.velocidad;
       paso = (game->delta_time) * (game->jugador.velocidad_actual);
       
   } else {
       game->jugador.velocidad_actual -= FRICCION * game->delta_time;
       
       if (game->jugador.velocidad_actual < 0.0f) game->jugador.velocidad_actual = 0.0f;
       if (game->jugador.velocidad_actual == 0.0f)
       {
           game->jugador.dir_x = 0;
           game->jugador.dir_y = 0;
           game->jugador.tiempo_arranque = 0.0f;
       }
       paso = (game->delta_time) * (game->jugador.velocidad_actual);
   }
   
   // conversion flotante entero variables temporales
   temp1.x = (int)game->jugador.x;
   temp1.y = (int)game->jugador.y;
   
   /* gestion al presionar teclas: hitbox, velocidades, colisiones
   casos en y, casos en x
   CASOS X
   */
   
   if (game->jugador.right == 1) 
   {
      temp1.x = (int)(game->jugador.x + paso);
      if(!chequea_tiles(game, &temp1) && !chequea_enemigos(game, &temp1) && temp1.x <= ancho_act - game->jugador.lado) 
      {
         game->jugador.x += paso;
         game->jugador.colisionando = 0;
      } else if (!game->jugador.colisionando) 
      {
          game->jugador.velocidad_actual *= REDUCE_COLISION;
          game->jugador.colisionando = 1;
      }
      
   } else if (game->jugador.left == 1) 
   {
      temp1.x = (int)(game->jugador.x - paso);
      if(!chequea_tiles(game, &temp1) && !chequea_enemigos(game, &temp1) && temp1.x >= 0) 
      {
          game->jugador.x -= paso;
          game->jugador.colisionando = 0;
      } else if (!game->jugador.colisionando) 
      {
          game->jugador.velocidad_actual *= REDUCE_COLISION;
          game->jugador.colisionando = 1;
      }
   }
   
   // reinicio temp x
   temp1.x = (int)game->jugador.x;
   
   // CASOS Y
        
   if (game->jugador.down == 1)
   {
      temp1.y = (int)(game->jugador.y + paso);
      if(!chequea_tiles(game, &temp1) && !chequea_enemigos(game, &temp1) && temp1.y <= alto_act - game->jugador.lado) 
      {
         game->jugador.y += paso;
         game->jugador.colisionando = 0;
      } else if (!game->jugador.colisionando) 
      {
          game->jugador.velocidad_actual *= REDUCE_COLISION;
          game->jugador.colisionando = 1;
      }
   } else if (game->jugador.up == 1)
   {
      temp1.y = (int)(game->jugador.y - paso);
      if(!chequea_tiles(game, &temp1) && !chequea_enemigos(game, &temp1) && temp1.y >= 0) 
      {
         game->jugador.y -= paso;
         game->jugador.colisionando = 0;
      } else if (!game->jugador.colisionando) 
      {
          game->jugador.velocidad_actual *= REDUCE_COLISION;
          game->jugador.colisionando = 1;
      }
   }
   
   if (game->jugador.up == 0 && game->jugador.down == 0 && game->jugador.left == 0 && game->jugador.right == 0)
   {
       game->jugador.x -= 0.2*(paso);
   }
   
   // - CAMARA
   //  definiendo el centro
   game->pantalla.camara.x = (int)game->jugador.x + (game->jugador.lado/2) - (game->pantalla.win_w/2);
   game->pantalla.camara.y = (int)game->jugador.y + (game->jugador.lado/2) - (game->pantalla.win_h/2);
   
   //  centrando la camara
   if (game->pantalla.camara.x < 0) game->pantalla.camara.x = 0;
   if (game->pantalla.camara.y < 0) game->pantalla.camara.y = 0;
   
   if (game->pantalla.camara.x > game->pantalla.nivel_w - game->pantalla.camara.w) 
   {
       game->pantalla.camara.x = game->pantalla.nivel_w - game->pantalla.camara.w;
   }
   
   if (game->pantalla.camara.y > game->pantalla.nivel_h - game->pantalla.camara.h) 
   {
       game->pantalla.camara.y = game->pantalla.nivel_h - game->pantalla.camara.h;
   }
   
    // MEcanica enemigosssssssssssssssssssss
    for (int i = 0; i < max_enemigos; i++)
    {
        if (game->enemigos[i].activo)
        {
            float paso_enm = game->enemigos[i].velocidad * game->delta_time;
            
            game->enemigos[i].x += game->enemigos[i].dir_x * paso_enm;
            SDL_Rect rect_enm = {
                (int)game->enemigos[i].x,
                (int)game->enemigos[i].y,
                game->enemigos[i].lado,
                game->enemigos[i].lado
            };
            
            if (chequea_tiles(game, &rect_enm) || SDL_HasIntersection (&rect_enm, &game->jugador.rect_colision) || rect_enm.x >= game->pantalla.nivel_w - game->enemigos[i].lado || rect_enm.x <= 0)
            {
                game->enemigos[i].x -= game->enemigos[i].dir_x * paso_enm;
                game->enemigos[i].dir_x *= -1;
            }
            
            game->enemigos[i].rect.x = (int)game->enemigos[i].x;
            game->enemigos[i].rect.y = (int)game->enemigos[i].y;
            game->enemigos[i].rect.w = game->enemigos[i].lado;
            game->enemigos[i].rect.h = game->enemigos[i].lado;
        }
    }
    
   // cronometro
   Uint32 tiempo_actual = SDL_GetTicks();
   Uint32 transcurrido = tiempo_actual - game->tiempo_inicio;
   
   int minutos = (transcurrido / 60000); // ms a min
   int segundos = (transcurrido / 1000)%60;
   int centesimas = (transcurrido % 1000)/10;
   
   // esto le pedi ayuda a la ia: esto formatea mi string (01:23:40) como en js
   sprintf(game->texto_cronometro, "%02d:%02d:%02d", minutos, segundos, centesimas);
}

int chequea_tiles(Game *game, SDL_Rect *player_rect)
{
    // LO MISMO Q EN RENDER.C
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
                    .h = game->tiles[i][j].h_tiles
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

int chequea_enemigos(Game *game, SDL_Rect *player_rect)
{
    for (int i = 0; i < max_enemigos; i++)
    {
        if (game->enemigos[i].activo)
        {
            if (SDL_HasIntersection(player_rect, &game->enemigos[i].rect))
            {
                return 1;
            }
        }
    }
    return 0;
}

void construir_rects(Game *game)
{
    game->jugador.rect = (SDL_Rect) {
        .x = (int)game->jugador.x,
        .y = (int)game->jugador.y,
        .w = game->jugador.lado,
        .h = game->jugador.lado
    };
    game->jugador.rect_colision = (SDL_Rect) {
        .x = game->jugador.x_colision,
        .y = game->jugador.y_colision,
        .w = game->jugador.w_colision,
        .h = game->jugador.h_colision
    };
}