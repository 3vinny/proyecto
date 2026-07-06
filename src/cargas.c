// carga tiles desde archivo de 20x15
#include "headers.h"

void ajusta_Tiles(Game *game)
{
    // tamaño tile pixeles = resolucion ventana / num tiles o filas
    game->tile_w_px = game->win_w / tile_cols;
    game->tile_h_px = game->win_h / tile_filas;
    for (int i=0; i<tile_filas;i++)
    {
        for(int j=0; j<tile_cols; j++)
        {
            game->tiles[i][j].x_tiles = j * game->tile_w_px;
            game->tiles[i][j].y_tiles = i * game->tile_h_px;
            game->tiles[i][j].w_tiles = game->tile_w_px;
            game->tiles[i][j].h_tiles = game->tile_h_px;
            
            game->tiles[i][j].jugador_x = j*game->tile_w_px;
            game->tiles[i][j].jugador_y = i*game->tile_h_px;
            
            if (game->tiles[i][j].activo_posJ)
            {
                game->x = j*game->tile_w_px;
                game->y = i*game->tile_h_px;
            }
        }
    }
}

void carga_Tiles(Game *game)
{
   FILE *archivo = fopen("./data/hitbox.txt", "r");
   if (!archivo){
      printf("Error: No se pudo abrir el archivo -> %s\n", "./data/hitbox.txt");
      return;
   }

   char linea[256]; // buffer grande x seguridad
   
   if (game->win_w == 0) game->win_w = w_inicial;
   if (game->win_h == 0) game->win_h = h_inicial;

   /*booleanos:
   #: cuadrado colision
   P: posicion jugador
   X: item destruible/powerup
   */
   for(int i=0; i<tile_filas; i++)
   {
       if (fgets(linea, sizeof(linea), archivo) == NULL) break;
       for (int j=0; j<(int)tile_cols; j++)
       {
           // copia caracter txt para el render despues
           game->tiles[i][j].tipo = linea[j];
           
           game->tiles[i][j].activo = (linea[j] == '.' || linea[j] == '#');
           game->tiles[i][j].activo_posJ = (linea[j] == 'P');
           game->tiles[i][j].objeto2 = (linea[j] == 'X');
       }
   }
   fclose(archivo);
   ajusta_Tiles(game);
}
