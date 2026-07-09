// carga tiles desde archivo de 20x15
#include "headers.h"

void ajusta_Tiles(Game *game)
{
    // tamaño tile pixeles = resolucion ventana / num tiles o filas
    int tile_w_px = game->pantalla.win_w / tile_cols;
    int tile_h_px = game->pantalla.win_h / tile_filas;
    
    for (int i=0; i<tile_filas;i++)
    {
        for(int j=0; j<tile_cols; j++)
        {
            game->tiles[i][j].x_tiles = j * tile_w_px;
            game->tiles[i][j].y_tiles = i * tile_h_px;
            game->tiles[i][j].w_tiles = tile_w_px;
            game->tiles[i][j].h_tiles = tile_h_px;
            
            if (game->tiles[i][j].activo_posJ)
            {
                game->jugador.x = j*tile_w_px;
                game->jugador.y = i*tile_h_px;
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

   char linea[256]; // arreglo grande
   
   if (game->pantalla.win_w == 0) game->pantalla.win_w = w_inicial;
   if (game->pantalla.win_h == 0) game->pantalla.win_h = h_inicial;

   /*booleanos:
   .(textura) y #: cuadrado colision
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
