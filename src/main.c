#include "headers.h"

void inicializa_game(Game *game);
void muestra_tutorial(Game *game);

int main(int argc, char **argv)
{
   Game game;
   inicializa_game(&game);

   fprintf(stderr, "----Inicializando SDL----\n");

   if(SDL_Inicia(&game)) 
   {
      game_Limpieza(&game, EXIT_FAILURE);
      exit(1); // stdlib
   }
   
   // importante resolucion, para camara scroll
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

   // tiles->menu->main->limpia al salir
   carga_Tiles(&game);
   game_Menu(&game);
   if (!game.quit) 
   {
      game_Main(&game, EXIT_SUCCESS);
   }
   game_Limpieza(&game, EXIT_SUCCESS);

   return 0;
}

void inicializa_game(Game *game)
{
    // 3200 = 50*64 = columnas-filas * tile // pasar a funcion void
   *game = (Game){
      .mando = NULL,
      .fuente = NULL,
      .vozinha = NULL,
      .ultimo_tiempo_vozinha = 0,
      .ultimo_tiempo_meta = 0,
      .texturaImg = NULL,
      .vueltas = 1, // iniciado en 1/3
      .llego_meta = false,
      .quit = false,
      .tiempo_inicio = 0,
      .ultimo_pitido = 0,
      .nivel_actual = 2,     // partimos desde nivel 1
      
      .pantalla = {
         .ventana = NULL,
         .renderer = NULL,
         .Fullscreen = false,
         .win_w = w_inicial,
         .win_h = h_inicial,
         .camara = { 0,0,w_inicial,h_inicial },
         .nivel_w = tile_cols*tam,
         .nivel_h = tile_filas*tam
      },
      
      .interfaz = {
         .texturaTexto = NULL,
         .texturaTexto2 = NULL,
         .texturaHP = NULL
      },
      
      .jugador = {
         .textura = NULL,
         .lado = 32,
         .x = 5,
         .y = 5,
         .angulo = 0.0,
         .hp = HP_INICIAL,
         .velocidad = 200,
         .contador_balas = MAX_PROYECTILES,
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

      .ranking = {
         .tiempo = 0,
         .puntaje = 0,
         .cuenta_camiones = 0,
         .cuenta_enemigos = 0
      },
      
      .enemigos = {
         { .x=0, .y=0, .angulo=90.0, .velocidad=0, .activo=true, .cooldown_disparo=0.0f, .hp = 4, .indicador = false },
         { .x=0, .y=0, .angulo=90.0, .velocidad=0, .activo=true, .cooldown_disparo=0.0f, .hp = 4, .indicador = false },
         { .x=0, .y=0, .angulo=90.0, .velocidad=0, .activo=true, .cooldown_disparo=0.0f, .hp = 4, .indicador = false }
      }
   };

   // inicializando proyectiles jugador
   for (int i=0; i < MAX_PROYECTILES; i++) 
   {
      game->jugador.proyectiles[i].activo = false;
   }
   
   // inicia proyectiles cada enemigo
   for (int i=0; i < max_enemigos; i++) 
   {
       for (int j=0; j < MAX_PROYECTILES; j++) 
       {
           game->enemigos[i].proyectiles[j].activo = false;
       }
   }
}

void muestra_tutorial(Game *game)
{
   SDL_Surface *s_tutorial = IMG_Load("./assets/bg/tutorial.png");
   if (!s_tutorial)
   {
      printf("Error al cargar imagen tutorial: %s\n", IMG_GetError());
      return;
   }
   
   SDL_Texture *t_tutorial = SDL_CreateTextureFromSurface(game->pantalla.renderer, s_tutorial);
   SDL_FreeSurface(s_tutorial);

   bool en_tutorial = true;
   while (en_tutorial && !game->quit)
   {
      SDL_Event evento;
      while (SDL_PollEvent(&evento))
      {
         if (evento.type == SDL_QUIT)
         {
            game->quit = true;
            en_tutorial = false;
         }
         else if (evento.type == SDL_KEYDOWN || evento.type == SDL_MOUSEBUTTONDOWN || evento.type == SDL_CONTROLLERBUTTONDOWN)
         {
            en_tutorial = false;
         }
     }

     SDL_SetRenderDrawColor(game->pantalla.renderer, 0,0,0,255);
     SDL_RenderClear(game->pantalla.renderer);
     SDL_RenderCopy(game->pantalla.renderer, t_tutorial, NULL, NULL);
     SDL_RenderPresent(game->pantalla.renderer);
     SDL_Delay(16);
   }
   SDL_DestroyTexture(t_tutorial); // importante al salir
}

// conecta todas las funciones en una (podria cambiarlo a main.c)
// juego.c
void game_Main(Game *game, int exitStatus)
{  
   muestra_tutorial(game);
   
   // empieza a contar el cronometro 
   game->tiempo_inicio = SDL_GetTicks();
   Uint32 tiempo_anterior = SDL_GetTicks();
   
   while (!game->quit)
   {
      // delta time
      Uint32 tiempo_actual = SDL_GetTicks();
      game->delta_time = (tiempo_actual - tiempo_anterior) / 1000.0f;
      tiempo_anterior = tiempo_actual;
               
      // funciones juego
      game_Input(game);
      game_Update(game);
      game_Render(game);
      
      SDL_Delay(16); //60FPS
   }
}