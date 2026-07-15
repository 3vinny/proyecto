// carga tiles desde archivo de 20x15
#include "headers.h"

void ajusta_Tiles(Game *game)
{
    // tamaño tile pixeles = resolucion ventana / num tiles o filas
    int tile_w_px = 32;
    int tile_h_px = 32;
    int contador_enemigos = 0;
    
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
            
            if (game->tiles[i][j].enemigo1 && contador_enemigos < max_enemigos)
            {
                game->enemigos[contador_enemigos].x = j*tile_w_px;
                game->enemigos[contador_enemigos].y = i*tile_h_px;
                game->enemigos[contador_enemigos].lado = 32;
                game->enemigos[contador_enemigos].velocidad = VELOCIDAD_ENEMIGO1;
                game->enemigos[contador_enemigos].dir_x = 1;
                game->enemigos[contador_enemigos].dir_y = 0;
                game->enemigos[contador_enemigos].activo = true;
                contador_enemigos++;
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
   E: enemigo
   C: casa | c: casa destruida */
   for(int i=0; i<tile_filas; i++)
   {
       if (fgets(linea, sizeof(linea), archivo) == NULL) break;
       for (int j=0; j<(int)tile_cols; j++)
       {
           // copia caracter txt para el render despues
           game->tiles[i][j].tipo = linea[j];

           game->tiles[i][j].activo = (linea[j] == '.' || linea[j] == 'N');
           game->tiles[i][j].activo_posJ = (linea[j] == 'P');
           game->tiles[i][j].objeto2 = (linea[j] == 'X'); // caja
           
           if (linea[j] == 'E')
           {
               game->tiles[i][j].enemigo1 = 1;
           }
           
           if (linea[j] == 'A')
           {
               game->tiles[i][j].agua = 1;
           }

           if (linea[j]=='C')
           {
               game->tiles[i][j].casa = 1;
           } else if (linea[j] == 'c') 
           {
               //game->tiles[i][j].casa = 0;               
           }
           
           if (linea[j]=='F')
           {
               printf("Peaton detectado\n");
               game->tiles[i][j].enemigo2 = 1;
           }
       }
   }
   fclose(archivo);
   ajusta_Tiles(game);
}
