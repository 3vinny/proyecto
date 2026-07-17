#include "headers.h"

int main(int argc, char **argv)
{
   // 3200 = 50*64 = columnas-filas * tile
   Game game = {
      .mando = NULL,
      .fuente = NULL,
      .vozinha = NULL,
      .ultimo_tiempo_vozinha = 0,
      .texturaImg = NULL,
      .texturaPista = NULL,
      .texturaTexto2 = NULL,
      .quit = false,
      .tiempo_inicio = 0,
      .ultimo_pitido = 0,
      
      .pantalla = {
         .ventana = NULL,
         .renderer = NULL,
         .Fullscreen = false,
         .win_w = w_inicial,
         .win_h = h_inicial,
         .camara = { 0,0,w_inicial,h_inicial },
         .nivel_w = tile_cols*tam,
         .nivel_h = tile_cols*tam
      },
      
      .jugador = {
         .textura = NULL,
         .lado = 32,
         .x = 5,
         .y = 5,
         .angulo = 0.0,
         .hp = HP_INICIAL,
         .velocidad = 200,
         .velocidad_actual = 0.0f,
         .dir_x = 0,
         .dir_y = 0,
         .tiempo_arranque = 0.0f,
         .colisionando = 0,
         .x_colision = 500,
         .y_colision = 700,
         .w_colision = 50,
         .h_colision = 25,
         // input
         .up = 0,
         .down = 0,
         .left = 0,
         .right = 0,
         .bocina = 0,
      },
      
      .enemigos = {
         { .x=0, .y=0, .angulo=90.0, .velocidad=0, .activo=true, .cooldown_disparo=0.0f },
         { .x=0, .y=0, .angulo=90.0, .velocidad=0, .activo=true, .cooldown_disparo=0.0f },
         { .x=0, .y=0, .angulo=90.0, .velocidad=0, .activo=true, .cooldown_disparo=0.0f }
      }
   };

   for (int i=0; i < MAX_PROYECTILES; i++)
   {
      game.proyectiles[i].activo = false;
   }

   fprintf(stderr, "----Inicializando SDL----\n");

   if(SDL_Inicia(&game)) 
   {
      game_Limpieza(&game, EXIT_FAILURE);
      exit(1);
   }
   
   // importante
   SDL_RenderSetLogicalSize(game.pantalla.renderer, w_inicial/ZOOM, h_inicial/ZOOM);
   game.pantalla.win_w = w_inicial / ZOOM;
   game.pantalla.win_h = h_inicial / ZOOM;
   game.pantalla.camara.w = w_inicial / ZOOM;
   game.pantalla.camara.h = h_inicial / ZOOM;
   
   SDL_Delay(16);

   if (cargarMedia(&game))
   {
      game_Limpieza(&game, EXIT_FAILURE);
      exit(1);
   }

   carga_Tiles(&game);
   
   game_Main(&game, EXIT_SUCCESS);
   game_Limpieza(&game, EXIT_SUCCESS);

   return 0;
}
