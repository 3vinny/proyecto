// Configuracion
#include "SDL.h"
#include "headers.h"

bool SDL_Inicia(Game *game)
{
   if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER))
   {
      printf("Error al iniciar SDL: %s\n", SDL_GetError());
      return true;
   }

   if(SDL_NumJoysticks() > 0)
   {
      game->mando = SDL_GameControllerOpen(0);
      if (game->mando)
      {
         printf("Mando 1 conectado: %s\n", SDL_GameControllerName(game->mando));
      }
   }

   int inicia_Imagen = IMG_Init(FLAGS_IMAGEN);
   if ((inicia_Imagen & FLAGS_IMAGEN) != FLAGS_IMAGEN)
   {
      printf("Error iniciando imagen SDL: %s\n", IMG_GetError());
   }

   // carga ventana
   game->pantalla.ventana = SDL_CreateWindow(titulo, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_inicial, h_inicial, 0);
   if (!game->pantalla.ventana)
   {
      printf("Error al crear ventana: %s\n", SDL_GetError());
      return true;
   }
   
   //manda la resolución actual a punteros win_w y win_h
   SDL_GetWindowSize(game->pantalla.ventana, &game->pantalla.win_w, &game->pantalla.win_h);

   // carga render
   game->pantalla.renderer = SDL_CreateRenderer(game->pantalla.ventana, -1, 0);
   if (!game->pantalla.renderer)
   {
      printf("Error al crear renderer: %s\n", SDL_GetError());
      return true;
   }

   // Inicia ttf
   if (TTF_Init() == -1)
   {
      printf("Error al cargar TTF: %s\n", TTF_GetError());
      return true;
   }

   // Carga de fuente ttf
   // ./data/hitbox.txt
   game->fuente = TTF_OpenFont("./assets/ttf/helvetica.ttf", 30);
   if (!(game->fuente))
   {
      printf("Error al cargar fuente .ttf: %s\n", TTF_GetError());
      return true;
   }
   
   // carga audio
   if((Mix_Init(FLAGS_AUDIO) & FLAGS_AUDIO) != FLAGS_AUDIO)
   {
       printf("Error al iniciar SDL_MIXER mp3:%s", SDL_GetError());
       return true;
   }
   
   if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
   {
       printf("Error al abrir dispositivo audio mp3: %s", SDL_GetError());
       return true;
   }
   
   game->vozinha = Mix_LoadWAV("./assets/sfx/bocina2.wav");
   if (game->vozinha == NULL)
   {
       printf("Error cargando sonido vozinha.wav: %s", Mix_GetError());
   }

   return false;
}

void game_Limpieza(Game *game, int exitStatus)
{
   SDL_DestroyRenderer(game->pantalla.renderer);
   SDL_DestroyTexture(game->texturaTexto);
   SDL_DestroyTexture(game->texturaTexto2);
   SDL_DestroyTexture(game->texturaImg);
   SDL_DestroyTexture(game->texturaPista);
   SDL_DestroyTexture(game->texturaCaja);
   SDL_DestroyTexture(game->texturaEnemigo);
   SDL_DestroyTexture(game->jugador.textura);
   SDL_DestroyWindow(game->pantalla.ventana);

   Mix_FreeChunk(game->vozinha);
   Mix_FreeChunk(game->sirena1);
   Mix_FreeChunk(game->bala);
   Mix_CloseAudio();
   
   Mix_Quit();
   IMG_Quit();
   TTF_Quit();
   SDL_Quit();
  
   exit(exitStatus);
}
