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
                game->enemigos[contador_enemigos].hp = HP_ENEMIGOS;
                game->enemigos[contador_enemigos].activo = true;
                contador_enemigos++;
            }
            // enemigo camion
            if (game->tiles[i][j].enemigo3 && contador_enemigos < max_enemigos)
            {
                game->enemigos[contador_enemigos].x = j*tile_w_px;
                game->enemigos[contador_enemigos].y = i*tile_h_px;
                game->enemigos[contador_enemigos].lado = 32;
                game->enemigos[contador_enemigos].velocidad = VELOCIDAD_ENEMIGO3;
                game->enemigos[contador_enemigos].dir_x = 1;
                game->enemigos[contador_enemigos].dir_y = 0;
                game->enemigos[contador_enemigos].hp = HP_CAMION;
                game->enemigos[contador_enemigos].activo = true;
                game->enemigos[contador_enemigos].es_camion = true;
                contador_enemigos++;
            }
            // enemigo bote (este debera moverse circular solo donde A/a)
            if (game->tiles[i][j].enemigo4 && contador_enemigos < max_enemigos)
            {
                game->enemigos[contador_enemigos].x = j*tile_w_px;
                game->enemigos[contador_enemigos].y = i*tile_h_px;
                game->enemigos[contador_enemigos].lado = 32;
                game->enemigos[contador_enemigos].velocidad = VELOCIDAD_BOTE;
                game->enemigos[contador_enemigos].dir_x = 1;
                game->enemigos[contador_enemigos].dir_y = 0;
                game->enemigos[contador_enemigos].hp = HP_BOTE;
                game->enemigos[contador_enemigos].activo = true;
                game->enemigos[contador_enemigos].es_bote = true;
                contador_enemigos++;
            }
        }
    }
}

void carga_Tiles(Game *game)
{
    char ruta_archivo[32];
    char linea[LIMITE_LINEA]; // arreglo grande
    // para evitar errores formateo ruta
    snprintf(ruta_archivo, sizeof(ruta_archivo), "./data/%d.txt", game->nivel_actual);
    
    FILE *archivo = fopen(ruta_archivo, "r");
    if (!archivo){
        printf("Error: No se pudo abrir el archivo -> %s\n", ruta_archivo);
        return;
    }
    
    if (game->pantalla.win_w == 0) game->pantalla.win_w = w_inicial;
    if (game->pantalla.win_h == 0) game->pantalla.win_h = h_inicial;

/*booleanos:
.(textura) y #: cuadrado colision
P: posicion jugador
X: item destruible/powerup
E: enemigo
C: casa | c: casa destruida 
M: mancha
A: agua
*/
    for(int i=0; i<tile_filas; i++)
    {
        if (fgets(linea, sizeof(linea), archivo) == NULL) break;
        for (int j=0; j<(int)tile_cols; j++)
        {
        
            char linea_actual = linea[j];
            // copia caracter txt para el render despues
            game->tiles[i][j].tipo = linea[j];

            game->tiles[i][j].activo = (linea_actual == '.' || linea_actual == 'N' || linea_actual == 'T');
            game->tiles[i][j].activo_posJ = (linea_actual == 'P');
            game->tiles[i][j].objeto2 = (linea_actual == 'X'); // caja
            game->tiles[i][j].objeto3 = (linea_actual == 'Z');
        
            if (linea_actual == 'A' || linea_actual == 'B')
            {
                game->tiles[i][j].agua = true;
            }
            
            if (linea_actual == 'B') game->tiles[i][j].enemigo4 = true;
            
            if (linea_actual == 'F') game->tiles[i][j].meta = true;

            if (linea_actual =='C')
            {
                game->tiles[i][j].casa = true;
            } else if (linea_actual == 'c') 
            {
                //game->tiles[i][j].casa = -1;               
            }
        
            if (linea_actual == 'P' || linea_actual == 'E' || linea_actual == 'X' || linea_actual == 'Z' || linea_actual == 'M' || linea_actual == 'G')
            {
                if (j>0 && (game->tiles[i-1][j].tipo == '-' || game->tiles[i-1][j].tipo == '1' || game->tiles[i-1][j].tipo == '3'))
                {
                    game->tiles[i][j].tipo = '=';
                }
                else if (j>0 && (game->tiles[i-1][j].tipo == '|' || game->tiles[i-1][j].tipo == '1' || game->tiles[i-1][j].tipo == '2'))
                {
                    game->tiles[i][j].tipo = '|';
                }
                else if (j>0 && (game->tiles[i-1][j].tipo == '='))
                {
                    game->tiles[i][j].tipo = '+';
                }
                else if (j>0 && (game->tiles[i-1][j].tipo == '.') && (game->tiles[i+1][j].tipo == '=') || (game->tiles[i+1][j].tipo == '=') || (game->tiles[i-1][j].tipo == 'A') || (game->tiles[i+1][j].tipo == 'A')) 
                {
                    game->tiles[i][j].tipo = '-';
                }
            
                if (linea_actual == 'E') game->tiles[i][j].enemigo1 = true;
                if (linea_actual == 'M') game->tiles[i][j].aceite = true;
                if (linea_actual == 'G') game->tiles[i][j].enemigo3 = true;
            } else {
                game->tiles[i][j].tipo = linea_actual;
            }
        }
    }
    fclose(archivo);
    ajusta_Tiles(game);
}

void carga_Nivel(Game *game, int nuevo_nivel)
{
    game->nivel_actual = nuevo_nivel;
    
    // limpieza de objetos
    for (int i=0; i<max_enemigos; i++) {
        game->enemigos[i].activo = false;
        for (int p=0; p<MAX_PROYECTILES; p++) {
            game->enemigos[i].proyectiles[p].activo = false;
        }
    }
    
    // limpia proyectiles (jugador)
    for (int p=0; p<MAX_PROYECTILES; p++) {
        game->jugador.proyectiles[p].activo = false;
    }
    
    carga_Tiles(game);
}
