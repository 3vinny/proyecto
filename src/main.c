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
          .nivel_w = 3200,
          .nivel_h = 3200
      },
      
      .jugador = {
          .textura = NULL,
          .lado = 32,
          .x = 5,
          .y = 5,
          .angulo = 0.0,
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
          { .x=300, .y=400, .velocidad=150, .activo=true },
          { .x=500, .y=200, .velocidad=420, .activo=true }
      }
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
