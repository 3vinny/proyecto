#include "headers.h"

int main(int argc, char **argv)
{
   
   Game game = {
      .ventana = NULL,
      .mando = NULL,
      .renderer = NULL,
      .surfaceTexto = NULL,
      .texturaTexto = NULL,
      .fuente = NULL,
      .quit = false,
      .velocidad = 450,
      .velocidad_actual = 0.0f,
      .dir_x = 0,
      .dir_y = 0,
      .tiempo_arranque = 0.0f,
      .colisionando = 0,
      .lado = 64,
      .x = 5,
      .y = 5,
      .angulo = 0.0,
      .x_ant = 208,
      .y_ant = 188,
      .h_x = 0, 
      .h_y = 0, 
      .h_w = w_inicial/4, 
      .h_h = h_inicial,
      .x_colision = 100,
      .y_colision = 300,
      .w_colision = 50,
      .h_colision = 25,
      .up = 0,
      .down = 0,
      .left = 0,
      .right = 0,
      .Fullscreen = false
   };
   
   fprintf(stderr, "----Inicializando SDL----\n");

   if(SDL_Inicia(&game)) 
   {
      game_Limpieza(&game, EXIT_FAILURE);
      exit(1);
   }
   
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
