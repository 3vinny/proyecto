// carga tiles desde archivo de 20x15
#include "headers.h"

void ajusta_Tiles(Game *game)
{
    // tamaño tile pixeles = resolucion ventana / num tiles o filas
    int tile_w_px = 32;
    int tile_h_px = 32;
    int contador_enemigos = 0;
    game->contador_camiones = 0;

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
                printf("Spawn ->: %d, %d, caracter leido: %c\n", game->jugador.x, game->jugador.y, game->tiles[i][j].tipo);
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
                game->enemigos[contador_enemigos].lado = 64;
                game->enemigos[contador_enemigos].velocidad = VELOCIDAD_ENEMIGO3;

                game->enemigos[contador_enemigos].dir_x = 1;
                game->enemigos[contador_enemigos].dir_y = 0;
                game->enemigos[contador_enemigos].angulo = 90.0f;
                game->enemigos[contador_enemigos].hp = HP_CAMION;
                game->enemigos[contador_enemigos].activo = true;

                game->enemigos[contador_enemigos].es_camion = true;
                game->enemigos[contador_enemigos].es_bote = false;
                game->enemigos[contador_enemigos].es_patrulla = false;
                game->enemigos[contador_enemigos].es_torreta = false;

                game->enemigos[contador_enemigos].rect.x = j*tile_w_px;
                game->enemigos[contador_enemigos].rect.y = i*tile_w_px;
                game->enemigos[contador_enemigos].rect.w = 64;
                game->enemigos[contador_enemigos].rect.h = 64;
                game->contador_camiones++;
                printf("Camiones: %d en (%.1f, %.1f)\n", game->contador_camiones, (float)game->enemigos[contador_enemigos].x, (float)game->enemigos[contador_enemigos].y);
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
            // torreta
            if (game->tiles[i][j].enemigo5 && contador_enemigos < max_enemigos)
            {
                game->enemigos[contador_enemigos].x = j*tile_w_px;
                game->enemigos[contador_enemigos].y = i*tile_h_px;
                game->enemigos[contador_enemigos].lado = 32;
                game->enemigos[contador_enemigos].velocidad = 0;
                game->enemigos[contador_enemigos].dir_x = 1;
                game->enemigos[contador_enemigos].dir_y = 0;
                game->enemigos[contador_enemigos].hp = HP_CAMION;
                game->enemigos[contador_enemigos].activo = true;
                game->enemigos[contador_enemigos].es_torreta = true;
                contador_enemigos++;
            }
        }
    }
}

void carga_Tiles(Game *game)
{
    char ruta_archivo[32];
    char linea[LIMITE_LINEA]; // arreglo grande
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
    // limpieza o inicializacion de tiles (evitar basura)
    for(int i=0; i<tile_filas; i++) {
        for (int j=0; j<tile_cols; j++) {
            game->tiles[i][j].activo = false;
            game->tiles[i][j].activo_posJ = false;
            game->tiles[i][j].objeto2 = false;
            game->tiles[i][j].objeto3 = false;
            game->tiles[i][j].agua = false;
            game->tiles[i][j].meta = false;
            game->tiles[i][j].casa = false;
            game->tiles[i][j].movediza = 0;
            game->tiles[i][j].aceite = false;
            game->tiles[i][j].enemigo1 = false;
            game->tiles[i][j].enemigo2 = false;
            game->tiles[i][j].enemigo3 = false;
            game->tiles[i][j].enemigo4 = false;
            game->tiles[i][j].enemigo5 = false;
            game->tiles[i][j].obstaculo = false;
            game->tiles[i][j].direccion = 0;
            game->tiles[i][j].tipo = ' ';
        }
    }

    for(int i=0; i<tile_filas; i++) {
        // LIMPIA BUFFER ANTES FGETS
        memset(linea, 0, sizeof(linea));

        if (fgets(linea, sizeof(linea), archivo) == NULL) break;
        for (int j=0; j<(int)tile_cols; j++) {
        
            char linea_actual = linea[j];

            // 32 tabla ascii=caracter vacio
            if (linea_actual < 32) {
                break; //corta bucle para evitar basura
            }
            game->tiles[i][j].tipo = linea_actual;

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
            
            if (linea_actual == 's'){
                game->tiles[i][j].movediza = 1;
            } else if (linea_actual == 'S'){
                game->tiles[i][j].movediza = 2;
            }
            
            if (linea_actual == 'B') game->tiles[i][j].enemigo4 = true;
            if (linea_actual == 'V') game->tiles[i][j].enemigo5 = true;
            if (linea_actual == 'F') game->tiles[i][j].meta = true;

            if (linea_actual =='C')
            {
                game->tiles[i][j].casa = true;
            }
            
            if (linea_actual == 'D') {
                game->tiles[i][j].direccion = 1;
            } else if (linea_actual == 'd') {
                game->tiles[i][j].direccion = -1;
            }
        
            if (linea_actual == 'P' || linea_actual == 'E' || linea_actual == 'X' || linea_actual == 'Z' || linea_actual == 'M' || linea_actual == 'G' || linea_actual == 'F')
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
                else if (j>0 && (game->tiles[i-1][j].tipo == '.') && (game->tiles[i+1][j].tipo == '=') || (game->tiles[i+1][j].tipo == '=') || (game->tiles[i-1][j].tipo == 'A') || (game->tiles[i+1][j].tipo == 'A') || (game->tiles[i-1][j].tipo == 'a')) 
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
    
    // limpieza de proyectiles enemigo
    for (int i=0; i<max_enemigos; i++) {
        game->enemigos[i].activo = false;
        game->enemigos[i].perseguir = false;
        game->enemigos[i].escapando = false;
        game->enemigos[i].sirena = false;
        game->enemigos[i].indicador = false;
        game->enemigos[i].cooldown_disparo = 3.0f; // 3 seg cooldown

        game->enemigos[i].es_patrulla = false;
        game->enemigos[i].es_bote = false;
        game->enemigos[i].es_camion = false;
        game->enemigos[i].es_torreta = false;

        for (int p=0; p<MAX_PROYECTILES; p++) {
            game->enemigos[i].proyectiles[p].activo = false;
        }
    }
    
    // limpia proyectiles (jugador)
    for (int p=0; p<MAX_PROYECTILES; p++) {
        game->jugador.proyectiles[p].activo = false;
    }

    // reinicio jugador
    game->jugador.velocidad_actual = 0.0f;
    game->jugador.dir_x = 0;
    game->jugador.dir_y = 0;
    game->jugador.colisionando = 0;
    game->jugador.tiempo_arranque = 0.0f;
    game->jugador.cooldown_choque = 1.5f; // 1 seg inmune
    game->jugador.cooldown_agua = 1.0f;

    // input
    game->jugador.up = 0;
    game->jugador.down = 0;
    game->jugador.left = 0;
    game->jugador.right = 0;
    game->jugador.freno = 0;
    game->jugador.disparo = 0;
    
    // reinicio nivel y hitbox
    carga_Tiles(game);

    game->pantalla.nivel_w = tile_cols * tam;
    game->pantalla.nivel_h = tile_filas * tam;

    // resetea posiciones jugador y colisiones al spawn
    game->jugador.rect.x = (int)game->jugador.x;
    game->jugador.rect.y = (int)game->jugador.y;
    game->jugador.rect.w = (int)game->jugador.lado;
    game->jugador.rect.h = (int)game->jugador.lado;

    game->jugador.rect_colision.x = (int)game->jugador.x;
    game->jugador.rect_colision.y = (int)game->jugador.y;
    game->jugador.rect_colision.w = (int)game->jugador.w_colision;
    game->jugador.rect_colision.h = (int)game->jugador.h_colision;

    // resetea interfaz
    if (game->interfaz.texturaTexto2 != NULL){
        SDL_DestroyTexture(game->interfaz.texturaTexto2);
        game->interfaz.texturaTexto2 = NULL;
    }
    if (game->interfaz.texturaHP != NULL){
        SDL_DestroyTexture(game->interfaz.texturaHP);
        game->interfaz.texturaHP = NULL;
    }
    if (game->interfaz.texturaBalas != NULL){
        SDL_DestroyTexture(game->interfaz.texturaBalas);
        game->interfaz.texturaBalas = NULL;
    }
    if (game->interfaz.texturaNivel != NULL){
        SDL_DestroyTexture(game->interfaz.texturaNivel);
        game->interfaz.texturaNivel = NULL;
    }
    if (game->interfaz.texturaTexto3 != NULL){
        SDL_DestroyTexture(game->interfaz.texturaTexto3);
        game->interfaz.texturaTexto3 = NULL;
    }
    
    // reset camara
    game->pantalla.camara.x = (int)game->jugador.x + (game->jugador.lado / 2) - (game->pantalla.win_w / 2);
    game->pantalla.camara.y = (int)game->jugador.y + (game->jugador.lado / 2) - (game->pantalla.win_h / 2);

    if (game->pantalla.camara.x < 0) game->pantalla.camara.x = 0;
    if (game->pantalla.camara.y < 0) game->pantalla.camara.y = 0;
    if (game->pantalla.camara.x > game->pantalla.nivel_w - game->pantalla.camara.w) {
        game->pantalla.camara.x = game->pantalla.nivel_w - game->pantalla.camara.w;
    }
    if (game->pantalla.camara.y > game->pantalla.nivel_h - game->pantalla.camara.h) {
        game->pantalla.camara.y > game->pantalla.nivel_h - game->pantalla.camara.h;
    }
    
    printf("Nivel %d cargado. pos inicial jug %.1f, %.1f", nuevo_nivel, game->jugador.x, game->jugador.y);
}
