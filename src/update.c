// colisiones direccion y fisicas, etc
#include "headers.h"

#define ACELERACION 200.0f
#define FRICCION 1000.0f
#define REDUCE_COLISION 0.4f
#define DELAY_ARRANQUE 0.11f
#define POTENCIA_FRENO 1000.0f
#define LIMITE 1.0f

int margen = 5;

int chequea_tiles(Game *game, SDL_Rect *player_rect, int es_enemigo);
int chequea_enemigos(Game *game, SDL_Rect *player_rect);
int chequea_entre_enemigos(Game *game, SDL_Rect *rect, int numero_enemigo);
float calcula_direccion(int dir_x_input, int dir_y_input);
void construir_rects(Game *game);
void actualiza_proyectiles(Game *game, Proyectil *proyectiles, bool es_enemigo);

void game_Update(Game *game)
{
    int ancho_act = game->pantalla.nivel_w;
    int alto_act = game->pantalla.nivel_h;
    int dir_x_input = 0;
    int dir_y_input = 0;
   
    construir_rects(game);

    SDL_Rect temp1 = game->jugador.rect;
    SDL_Rect temp2 = game->jugador.rect_colision;
   
   /* direccion solicitada x input
       0=nada 1=derecha/abajo -1=izq/arriba */
    if (game->jugador.right == 1)
    {
        dir_x_input = 1;
    } else if (game->jugador.left == 1)
    {
        dir_x_input = -1;
    }
    
    if (game->jugador.down == 1)
    {
        dir_y_input = 1;
    } else if (game->jugador.up == 1)
    {
        dir_y_input = -1;
    }
   
   /*Movimiento y invertir sentido*/
   
    bool movimiento = (dir_x_input != 0 || dir_y_input != 0);
    int invirtiendo = 0;

    if (dir_x_input != 0 && game->jugador.dir_x != 0 && dir_x_input != game->jugador.dir_x) invirtiendo = 1;
    if (dir_y_input != 0 && game->jugador.dir_y != 0 && dir_y_input != game->jugador.dir_y) invirtiendo = 1;
    float paso;
   
    /* usamos sistema d pasos con el auto ahora tenemos 3 casos:
    0. freno 
    1. cuando se mete reversa 
    2. cuando ta en movimiento 
    3. cuando no se presiona nada 
    */
    if (game->jugador.freno == 1){
        game->jugador.velocidad_actual -= POTENCIA_FRENO * game->delta_time;

        if (game->jugador.velocidad_actual <= LIMITE){
            game->jugador.velocidad_actual = LIMITE;
            game->jugador.dir_x = 0;
            game->jugador.dir_y = 0;
            game->jugador.tiempo_arranque = 0.0f;
        }
        paso = (game->delta_time) * (game->jugador.velocidad_actual);
       
    } else if (invirtiendo){
        game->jugador.velocidad_actual -= FRICCION * game->delta_time;
        if (game->jugador.velocidad_actual <= 0.0f)
        {
            // si ya freno resetea el delay y cambia direccion dir_x
            game->jugador.velocidad_actual = 0.0f;   
            game->jugador.dir_x = dir_x_input;
            game->jugador.dir_y = dir_y_input;
            game->jugador.tiempo_arranque = 0.0f;
        }
        paso = 0.0f;
       
    } else if (movimiento){
        game->jugador.dir_x = dir_x_input;
        game->jugador.dir_y = dir_y_input;
        
        // ANGULOS: 0 45 90 135 180 225 270 315
        if (dir_x_input == 0 && dir_y_input == -1) game->jugador.angulo = 0.0;
        else if (dir_x_input == 1 && dir_y_input == -1) game->jugador.angulo = 45.0;
        else if (dir_x_input == 1 && dir_y_input == 0) game->jugador.angulo = 90.0;
        else if (dir_x_input == 1 && dir_y_input == 1) game->jugador.angulo = 135.0;
        else if (dir_x_input == 0 && dir_y_input == 1) game->jugador.angulo = 180.0;
        else if (dir_x_input == -1 && dir_y_input == 1) game->jugador.angulo = 225.0; 
        else if (dir_x_input == -1 && dir_y_input == 0) game->jugador.angulo = 270.0;
        else if (dir_x_input == -1 && dir_y_input == -1) game->jugador.angulo = 315.0;
       
        // aceleracion
        if (game->jugador.velocidad_actual <= 0.0f){
            game->jugador.tiempo_arranque += game->delta_time;
            if (game->jugador.tiempo_arranque >= DELAY_ARRANQUE) {
                game->jugador.velocidad_actual += ACELERACION * game->delta_time;
            }
        } else {
            game->jugador.velocidad_actual += ACELERACION * game->delta_time;
        }
        
        if (game->jugador.velocidad_actual > game->jugador.velocidad){
            game->jugador.velocidad_actual = game->jugador.velocidad;
        }
        paso = (game->delta_time) * (game->jugador.velocidad_actual);
    } else {
        game->jugador.velocidad_actual -= FRICCION * game->delta_time;
        
        if (game->jugador.velocidad_actual < 0.0f) game->jugador.velocidad_actual = 0.0f;
        if (game->jugador.velocidad_actual == 0.0f){
            game->jugador.dir_x = 0;
            game->jugador.dir_y = 0;
            game->jugador.tiempo_arranque = 0.0f;
        }
        paso = (game->delta_time) * (game->jugador.velocidad_actual);
    }
   
    // conversion flotante entero variables temporales
    temp1.x = (int)game->jugador.x;
    temp1.y = (int)game->jugador.y;
   
    /* gestion al presionar teclas: hitbox, velocidades, colisiones
    casos en y, casos en x
    CASOS X
    */
    if (game->jugador.right == 1){
        temp1.x = (int)(game->jugador.x + paso);
        if(!chequea_tiles(game, &temp1, 0) && !chequea_enemigos(game, &temp1) && temp1.x <= ancho_act - game->jugador.lado) 
        {
            game->jugador.x += paso;
            game->jugador.colisionando = 0;
        } else if (!game->jugador.colisionando) 
        {
            game->jugador.velocidad_actual *= REDUCE_COLISION;
            game->jugador.colisionando = 1;
        }    
    } else if (game->jugador.left == 1){
        temp1.x = (int)(game->jugador.x - paso);
        if(!chequea_tiles(game, &temp1, 0) && !chequea_enemigos(game, &temp1) && temp1.x >= 0) 
        {
            game->jugador.x -= paso;
            game->jugador.colisionando = 0;
        } else if (!game->jugador.colisionando) {
            game->jugador.velocidad_actual *= REDUCE_COLISION;
            game->jugador.colisionando = 1;
        }
    }
   
    // reinicio temp x
    temp1.x = (int)game->jugador.x;
   
    // CASOS Y
    if (game->jugador.down == 1) {
        temp1.y = (int)(game->jugador.y + paso);
        if(!chequea_tiles(game, &temp1, 0) && !chequea_enemigos(game, &temp1) && temp1.y <= alto_act - game->jugador.lado) 
        {
            game->jugador.y += paso;
            game->jugador.colisionando = 0;
        } else if (!game->jugador.colisionando){
            game->jugador.velocidad_actual *= REDUCE_COLISION;
            game->jugador.colisionando = 1;
        }
    } else if (game->jugador.up == 1){
        temp1.y = (int)(game->jugador.y - paso);
        if(!chequea_tiles(game, &temp1, 0) && !chequea_enemigos(game, &temp1) && temp1.y >= 0) 
        {
            game->jugador.y -= paso;
            game->jugador.colisionando = 0;
        } else if (!game->jugador.colisionando) {
            game->jugador.velocidad_actual *= REDUCE_COLISION;
            game->jugador.colisionando = 1;
        }
    }
   
    if (game->jugador.up == 0 && game->jugador.down == 0 && game->jugador.left == 0 && game->jugador.right == 0)
    {
        game->jugador.x -= 0.05*(paso);
        game->jugador.velocidad_actual *= 0.97f;
    }
   
    // - CAMARA
    //          definiendo el centro
    game->pantalla.camara.x = (int)game->jugador.x + (game->jugador.lado/2) - (game->pantalla.win_w/2);
    game->pantalla.camara.y = (int)game->jugador.y + (game->jugador.lado/2) - (game->pantalla.win_h/2);
    
    //          centrando camara
    if (game->pantalla.camara.x < 0) game->pantalla.camara.x = 0;
    if (game->pantalla.camara.y < 0) game->pantalla.camara.y = 0;
    
    if (game->pantalla.camara.x > game->pantalla.nivel_w - game->pantalla.camara.w){
        game->pantalla.camara.x = game->pantalla.nivel_w - game->pantalla.camara.w;
    }
    
    if (game->pantalla.camara.y > game->pantalla.nivel_h - game->pantalla.camara.h){
        game->pantalla.camara.y = game->pantalla.nivel_h - game->pantalla.camara.h;
    }
   
    //         Mecanica de los  enemigos
    for (int i = 0; i < max_enemigos; i++) {
        if (game->enemigos[i].activo) {
            float paso_enm = game->enemigos[i].velocidad * game->delta_time;
            
            if (!game->enemigos[i].perseguir) {
                game->enemigos[i].x += game->enemigos[i].dir_x * paso_enm; 
                SDL_Rect rect_enm1 = {
                    (int)(game->enemigos[i].x),
                    (int)(game->enemigos[i].y),
                    game->enemigos[i].lado,
                    game->enemigos[i].lado
                };
                
                if (chequea_tiles(game, &rect_enm1, 1) || rect_enm1.x >= game->pantalla.nivel_w - game->enemigos[i].lado || rect_enm1.x <= 0)
                {
                    game->enemigos[i].x -= game->enemigos[i].dir_x*(paso_enm);
                    game->enemigos[i].dir_x *= -1;
                }
            } else {
                float dx = game->jugador.x - game->enemigos[i].x;
                float dy = game->jugador.y - game->enemigos[i].y;
                float dist = sqrtf(dx*dx + dy*dy);
                
                if (dist > RADIO_PERDIDO) {
                    game->enemigos[i].perseguir = false;
                    game->enemigos[i].sirena = false;  
                } else {
                    if (game->enemigos[i].cooldown_disparo > 0.0f) {
                    game->enemigos[i].cooldown_disparo -= game->delta_time;
                    }
    
                    if (game->enemigos[i].cooldown_disparo <= 0.0f && dist > 0.0f) {
                    for (int p = 0; p<MAX_PROYECTILES; p++) {
                        if(!game->enemigos[i].proyectiles[p].activo) {
                            game->enemigos[i].proyectiles[p].activo = true;
                            game->enemigos[i].proyectiles[p].x = game->enemigos[i].x + (game->enemigos[i].lado/2.0f);
                            game->enemigos[i].proyectiles[p].y = game->enemigos[i].y + (game->enemigos[i].lado/2.0f);
    
                            game->enemigos[i].proyectiles[p].dir_x = dx/dist;
                            game->enemigos[i].proyectiles[p].dir_y = dy/dist;
    
                            game->enemigos[i].proyectiles[p].velocidad = VELOCIDAD_DISPARO;
                            game->enemigos[i].proyectiles[p].lado = 6;
                            game->enemigos[i].cooldown_disparo = COOLDOWN_DISPARO;
                            game->enemigos[i].proyectiles[p].sonido = true;
                            break; //necesario romper el ciclo para que no continue
                        }
                    }
                }
            }
                

                if (dist > RADIO_EJ) {
                    float paso_x = (dx/dist) * paso_enm;
                    float paso_y = (dy/dist) * paso_enm;
                    
                    SDL_Rect rect_enm2 = {
                       (int)(game->enemigos[i].x + paso_x),
                       (int)(game->enemigos[i].y),
                       game->enemigos[i].lado,
                       game->enemigos[i].lado
                    };
                    
                    if (!chequea_tiles(game, &rect_enm2, 1) && !SDL_HasIntersection(&rect_enm2, &game->jugador.rect) && !chequea_entre_enemigos(game, &rect_enm2, i) && rect_enm2.x >= 0 && rect_enm2.x <= game->pantalla.nivel_w - game->enemigos[i].lado)
                    {
                        game->enemigos[i].x += paso_x;
                        if (paso_x >= 0) {
                            game->enemigos[i].dir_x = 1;
                        } else {
                            game->enemigos[i].dir_x = -1;
                        }
                    }
                    
                    rect_enm2.x = (int)game->enemigos[i].x;
                    rect_enm2.y = (int)(game->enemigos[i].y + paso_y);
                    
                    if (!chequea_tiles(game, &rect_enm2, 1) && !SDL_HasIntersection(&rect_enm2, &game->jugador.rect) && !chequea_entre_enemigos(game, &rect_enm2, i) && rect_enm2.y >= 0 && rect_enm2.y <= game->pantalla.nivel_h - game->enemigos[i].lado)
                    {
                        game->enemigos[i].y += paso_y;
                        if (paso_y >= 0) {
                            game->enemigos[i].dir_x = 1;
                        } else {
                            game->enemigos[i].dir_x = -1;
                        }
                    }
                }
                
                SDL_Rect rect_eactual = {
                    (int)game->enemigos[i].x,
                    (int)game->enemigos[i].y,
                    game->enemigos[i].lado - margen,
                    game->enemigos[i].lado - margen
                };
            }
            
            game->enemigos[i].rect.x = (int)game->enemigos[i].x;
            game->enemigos[i].rect.y = (int)game->enemigos[i].y;
            game->enemigos[i].rect.w = game->enemigos[i].lado;
            game->enemigos[i].rect.h = game->enemigos[i].lado;
            game->enemigos[i].angulo = calcula_direccion(game->enemigos[i].dir_x, game->enemigos[i].dir_y);
        }
    }
    
    // cronometro
    int tiempo_actual = SDL_GetTicks();
    int transcurrido = tiempo_actual - game->tiempo_inicio;
   
    int minutos = (transcurrido / 60000); // ms a min
    int segundos = (transcurrido / 1000)%60;
    int centesimas = (transcurrido % 1000)/10;
    // esto formatea mi string (ejemplo 01:23:40) como en js
    // snprintf(buffer,sizeof(buffer), "Texto a formatear"); buffer es mi char definido
    //sprintf(game->texto_cronometro, "%02d:%02d:%02d", minutos, segundos, centesimas);
    
    for (int i=0; i<max_enemigos; i++)
    {
        actualiza_proyectiles(game, game->enemigos[i].proyectiles, true);
    }
    actualiza_proyectiles(game, game->jugador.proyectiles, false);
}

// chequea proyectiles del enemigo y jugador:
void actualiza_proyectiles(Game *game, Proyectil *proyectiles, bool es_enemigo)
{
    for (int p=0; p<MAX_PROYECTILES; p++) {
        if(!proyectiles[p].activo) continue;
        
        proyectiles[p].x += proyectiles[p].dir_x * proyectiles[p].velocidad * game->delta_time;
        proyectiles[p].y += proyectiles[p].dir_y * proyectiles[p].velocidad * game->delta_time;
    
        SDL_Rect rect_bala = {
            .x = (int)proyectiles[p].x,
            .y = (int)proyectiles[p].y,
            .w = proyectiles[p].lado,
            .h = proyectiles[p].lado
        };
    
        if (rect_bala.x < 0 || rect_bala.y < 0 || chequea_tiles(game, &rect_bala, 1))
        {
           proyectiles[p].activo = false;
           continue;
        }
        
        if (es_enemigo)
        {
            if (SDL_HasIntersection(&rect_bala, &game->jugador.rect)) 
            {
                proyectiles[p].activo = false;
                game->jugador.hp -= 1;
                printf("El jugador recibio disparo !! -1 de HP : \n--HP: %d --velocidad_actual: %.2f\n", game->jugador.hp, game->jugador.velocidad_actual);
            } 
        } else {
            for (int i=0; i<max_enemigos; i++) {
                if (game->enemigos[i].activo && SDL_HasIntersection(&rect_bala, &game->enemigos[i].rect)) {
                    proyectiles[p].activo = false;
                    printf("Enemigo n°%d recibio disparo!\n", i);
                    break;
                }
            }
        }
    }
}

int chequea_tiles(Game *game, SDL_Rect *player_rect, int es_enemigo)
{
    // LO MISMO Q EN RENDER.C
    for (int i = 0; i < tile_filas; i++) {
        for (int j = 0; j < (int)tile_cols; j++) {
            if (game->tiles[i][j].activo) {
                SDL_Rect temp4 = {
                    .x = game->tiles[i][j].x_tiles,
                    .y = game->tiles[i][j].y_tiles,
                    .w = game->tiles[i][j].w_tiles,
                    .h = game->tiles[i][j].h_tiles
                };
                
                // comprueba si chocaste con cualquier tile
                if (SDL_HasIntersection(player_rect, &temp4)) {
                    return 1; 
                }
            }
            
            if (game->tiles[i][j].casa) {
                    SDL_Rect tempcasa = {
                        .x = game->tiles[i][j].x_tiles,
                        .y = game->tiles[i][j].y_tiles,
                        .w = game->tiles[i][j].w_tiles,
                        .h = game->tiles[i][j].h_tiles
                    };
                
                    if (SDL_HasIntersection(player_rect, &tempcasa)) {
                        return 1;
                    }
            }

            if (game->tiles[i][j].agua) {
                SDL_Rect tempagua = {
                    .x = game->tiles[i][j].x_tiles,
                    .y = game->tiles[i][j].y_tiles,
                    .w = game->tiles[i][j].w_tiles,
                    .h = game->tiles[i][j].h_tiles
                };
                
                if (!es_enemigo) {
                    if (SDL_HasIntersection(player_rect, &tempagua)) {
                        if (!es_enemigo) return 1;
                        game->jugador.velocidad_actual = 2*VELOCIDAD_ENEMIGO1;
                    }
                }
            }

            if (!es_enemigo && game->tiles[i][j].aceite) {
                SDL_Rect tempaceite = {
                    .x = game->tiles[i][j].x_tiles,
                    .y = game->tiles[i][j].y_tiles,
                    .w = game->tiles[i][j].w_tiles - 2*margen,
                    .h = game->tiles[i][j].h_tiles - 2*margen
                };

                if (SDL_HasIntersection(player_rect, &tempaceite)) {
                    game->jugador.velocidad_actual = 100.0f;
                    game->jugador.x -= 0.2;
                }
            }
        }
    }
    
    // RECTANGULO DESTRUIBLE !!!
    for (int i=0; i<tile_filas; i++) {
        for (int j=0; j<tile_cols; j++) {
            if (game->tiles[i][j].objeto2) {
                SDL_Rect temp5 = {
                    game->tiles[i][j].x_tiles,
                    game->tiles[i][j].y_tiles,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };
                
                if (SDL_HasIntersection(player_rect, &temp5)) {
                    if (es_enemigo) {
                        return 1;
                    } else {
                        game->tiles[i][j].objeto2 = false;
                        game->jugador.velocidad_actual *= 0.5f;
                        SDL_Log("caja destruida!");
                        game->jugador.velocidad_actual *= 1.2f;
                        game->jugador.hp += 1;
                    }
                }
            }
        }
    }
    return 0; 
}

int chequea_enemigos(Game *game, SDL_Rect *player_rect)
{
    for (int i = 0; i < max_enemigos; i++) {
        if (game->enemigos[i].activo) {
            SDL_Rect hitbox_enemigo = {
                game->enemigos[i].rect.x + margen,
                game->enemigos[i].rect.y + margen,
                game->enemigos[i].rect.w - (margen*2),
                game->enemigos[i].rect.h - (margen*2)
            };
            
            if (SDL_HasIntersection(player_rect, &hitbox_enemigo)) {
                //printf("Colision player-enemigo N°%d\n", i);
                if (!game->enemigos[i].perseguir) {
                    game->enemigos[i].perseguir = true;
                    game->enemigos[i].sirena = true;
                    printf("Patrulla N°%d Perseguira!!\n", i);
                    game->enemigos[i].velocidad = 0.95*(game->jugador.velocidad);
                    return 1;
                }
            }
        }
    }
    return 0;
}

int chequea_entre_enemigos(Game *game, SDL_Rect *rect, int numero_enemigo)
{
    for (int i=0; i<max_enemigos; i++)
    {
        if (i == numero_enemigo || !game->enemigos[i].activo) continue;
        
        if (SDL_HasIntersection(rect, &game->enemigos[i].rect))
        {
            return 1;
        }
    }
    return 0;
}

void construir_rects(Game *game)
{
    game->jugador.rect = (SDL_Rect) {
        .x = (int)game->jugador.x,
        .y = (int)game->jugador.y,
        .w = game->jugador.lado,
        .h = game->jugador.lado
    };
    game->jugador.rect_colision = (SDL_Rect) {
        .x = game->jugador.x_colision,
        .y = game->jugador.y_colision,
        .w = game->jugador.w_colision,
        .h = game->jugador.h_colision
    };
}

float calcula_direccion(int dir_x_input, int dir_y_input)
{
    if (dir_x_input == 0 && dir_y_input == -1) return 0.0f;
    if (dir_x_input == 1 && dir_y_input == -1) return 45.0f;
    if (dir_x_input == 1 && dir_y_input == 0) return 90.0f;
    if (dir_x_input == 1 && dir_y_input == 1) return 135.0f;
    if (dir_x_input == 0 && dir_y_input == 1) return 180.0f;
    if (dir_x_input == -1 && dir_y_input == 1) return 225.0f; 
    if (dir_x_input == -1 && dir_y_input == 0) return 270.0f;
    if (dir_x_input == -1 && dir_y_input == -1) return 315.0f;
    return 90.0f;
}