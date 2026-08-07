// colisiones direccion y fisicas, etc
#include "headers.h"

#define ACELERACION 110.0f
#define FRICCION 1000.0f
#define REDUCE_COLISION 0.4f
#define DELAY_ARRANQUE 0.11f
#define POTENCIA_FRENO 1100.0f
#define VELOCIDAD_IDLE 10.0f
#define LIMITE 1.0f

int margen = 5;

int chequea_tiles(Game *game, SDL_Rect *player_rect, int es_enemigo);
void construir_rects(Game *game);
int chequea_enemigos(Game *game, SDL_Rect *player_rect);
int chequea_entre_enemigos(Game *game, SDL_Rect *rect, int numero_enemigo);
int enemigos_escapan(Game *game, int numero_enemigo);
int esta_en_agua(Game *game, SDL_Rect *rect);
int bote_agua(Game *game, int numero_enemigo);
void bote_dispara(Game *game, int numero_enemigo);
float calcula_direccion(int dir_x_input, int dir_y_input);
void actualiza_proyectiles(Game *game, Proyectil *proyectiles, bool es_enemigo);
void dispara_jugador(Game *game);

void empuja_camion(Game *game);
void crea_explosion(Game *game, float x, float y);
void dano_explosion(Game *game, float x, float y);
void actualiza_explosiones(Game *game);

int funcion_misiones(Game *game);
void funcion_meta(Game *game);
void jugador_muere(Game *game);
void panel_nombre(Game *game, char *nombre, size_t max_caracteres);
void calcula_puntaje(Game *game, bool completado);
void guarda_puntaje(Game *game, const char *nombre, int puntaje, int nivel, int tiempo);
void endgame(Game *game, bool victoria);

void game_Update(Game *game)
{
    // tope: importante para evitar bugs de cronometro y fisicas
    if (game->delta_time > 0.05f)
    {
        game->delta_time = 0.05f;
    }

    // funciones resolucion e input
    int ancho_act = game->pantalla.nivel_w;
    int alto_act = game->pantalla.nivel_h;
    int dir_x_input = 0;
    int dir_y_input = 0;
    int margen_camion = 0;
   
    // funciones inicializadoras de hitboxes
    construir_rects(game);
    empuja_camion(game);

    SDL_Rect temp1 = game->jugador.rect;
    SDL_Rect temp2 = game->jugador.rect_colision;
   
   /* direccion solicitada x input
       0=nada 1=derecha/abajo -1=izq/arriba */
    if (game->jugador.right == 1) 
    {
        dir_x_input = 1;
    } 
    else if (game->jugador.left == 1) 
    {
        dir_x_input = -1;
    }
    
    if (game->jugador.down == 1) 
    {
        dir_y_input = 1;
    } 
    else if (game->jugador.up == 1) 
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
    if (game->jugador.freno == 1)
    {
        game->jugador.velocidad_actual -= POTENCIA_FRENO * game->delta_time;

        if (game->jugador.velocidad_actual <= LIMITE)
        {
            game->jugador.velocidad_actual = LIMITE;
            game->jugador.dir_x = 0;
            game->jugador.dir_y = 0;
            game->jugador.tiempo_arranque = 0.0f;
        }
        paso = (game->delta_time) * (game->jugador.velocidad_actual);
       
    } 
    else if (invirtiendo) 
    {
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
    } 
    else if (movimiento) 
    {
        game->jugador.dir_x = dir_x_input;
        game->jugador.dir_y = dir_y_input;
        
        // ANGULOS: 0 45 90 135 180 225 270 315
        // RECORDATORIO CAMBIAR ESTO POR LA FUNCION float calcula_direccion
        if (dir_x_input == 0 && dir_y_input == -1) game->jugador.angulo = 0.0;
        else if (dir_x_input == 1 && dir_y_input == -1) game->jugador.angulo = 45.0;
        else if (dir_x_input == 1 && dir_y_input == 0) game->jugador.angulo = 90.0;
        else if (dir_x_input == 1 && dir_y_input == 1) game->jugador.angulo = 135.0;
        else if (dir_x_input == 0 && dir_y_input == 1) game->jugador.angulo = 180.0;
        else if (dir_x_input == -1 && dir_y_input == 1) game->jugador.angulo = 225.0; 
        else if (dir_x_input == -1 && dir_y_input == 0) game->jugador.angulo = 270.0;
        else if (dir_x_input == -1 && dir_y_input == -1) game->jugador.angulo = 315.0;
       
        // aceleracion
        if (game->jugador.velocidad_actual <= 0.0f)
        {
            game->jugador.tiempo_arranque += game->delta_time;
            if (game->jugador.tiempo_arranque >= DELAY_ARRANQUE) 
            {
                game->jugador.velocidad_actual += ACELERACION * game->delta_time;
            }
        } 
        else 
        {
            game->jugador.velocidad_actual += ACELERACION * game->delta_time;
        }
        
        if (game->jugador.velocidad_actual > game->jugador.velocidad)
        {
            game->jugador.velocidad_actual = game->jugador.velocidad;
        }
        paso = (game->delta_time) * (game->jugador.velocidad_actual);
    } 
    else 
    {
        // caso idle / quieto
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
   
    /* manejo al presionar teclas: hitbox con tiles & enemigos, velocidades, colisiones
    casos en y, casos en x
    CASOS X */
    if (game->jugador.right == 1)
    {
        temp1.x = (int)(game->jugador.x + paso);
        if(!chequea_tiles(game, &temp1, 0) && !chequea_enemigos(game, &temp1) && temp1.x <= ancho_act - game->jugador.lado) 
        {
            game->jugador.x += paso;
            game->jugador.colisionando = 0;
        } 
        else if (!game->jugador.colisionando) 
        {
            game->jugador.velocidad_actual *= REDUCE_COLISION;
            game->jugador.colisionando = 1;
        }    
    } 
    else if (game->jugador.left == 1)
    {
        temp1.x = (int)(game->jugador.x - paso);
        if(!chequea_tiles(game, &temp1, 0) && !chequea_enemigos(game, &temp1) && temp1.x >= 0) 
        {
            game->jugador.x -= paso;
            game->jugador.colisionando = 0;
        } 
        else if (!game->jugador.colisionando)
        {
            game->jugador.velocidad_actual *= REDUCE_COLISION;
            game->jugador.colisionando = 1;
        }
    }
   
    // reinicio temp x
    temp1.x = (int)game->jugador.x;
   
    // CASOS Y
    if (game->jugador.down == 1)
    {
        temp1.y = (int)(game->jugador.y + paso);
        if(!chequea_tiles(game, &temp1, 0) && !chequea_enemigos(game, &temp1) && temp1.y <= alto_act - game->jugador.lado) 
        {
            game->jugador.y += paso;
            game->jugador.colisionando = 0;
        } 
        else if (!game->jugador.colisionando)
        {
            game->jugador.velocidad_actual *= REDUCE_COLISION;
            game->jugador.colisionando = 1;
        }
    } 
    else if (game->jugador.up == 1)
    {
        temp1.y = (int)(game->jugador.y - paso);
        if(!chequea_tiles(game, &temp1, 0) && !chequea_enemigos(game, &temp1) && temp1.y >= 0) 
        {
            game->jugador.y -= paso;
            game->jugador.colisionando = 0;
        } 
        else if (!game->jugador.colisionando)
        {
            game->jugador.velocidad_actual *= REDUCE_COLISION;
            game->jugador.colisionando = 1;
        }
    }
   
    // IDLE / QUIETO
    if (game->jugador.up == 0 && game->jugador.down == 0 && game->jugador.left == 0 && game->jugador.right == 0)
    {
        float rad_idle = game->jugador.angulo * PI / 180.0f;
        float paso_idle = VELOCIDAD_IDLE * game->delta_time;

        SDL_Rect temp_idle = temp1;
        temp_idle.x = (int)(game->jugador.x + sinf(rad_idle) * paso_idle);
        temp_idle.y = (int)(game->jugador.y + (-cosf(rad_idle)) * paso_idle);

        //vemos que nada interfiera su camino
        if (!chequea_tiles(game, &temp_idle, 0) && !chequea_enemigos(game, &temp_idle))
        {
            bool dentromapa = temp_idle.x >= 0 && temp_idle.x <= ancho_act - game->jugador.lado && temp_idle.y >= 0 && temp_idle.y <= ancho_act - game->jugador.lado;
            if (dentromapa)
            {
                    game->jugador.x += sinf(rad_idle) * paso_idle;
                    game->jugador.y += -cosf(rad_idle) * paso_idle;
            }
        }
        //game->jugador.x -= 0.05*(paso);
        //game->jugador.velocidad_actual *= 0.97f;
    }
   
    // - CAMARA
    //          definiendo el centro
    game->pantalla.camara.x = (int)game->jugador.x + (game->jugador.lado/2) - (game->pantalla.win_w/2);
    game->pantalla.camara.y = (int)game->jugador.y + (game->jugador.lado/2) - (game->pantalla.win_h/2);
    
    //          centrando camara
    if (game->pantalla.camara.x < 0) 
        game->pantalla.camara.x = 0;
    if (game->pantalla.camara.y < 0) 
        game->pantalla.camara.y = 0;
    
    if (game->pantalla.camara.x > game->pantalla.nivel_w - game->pantalla.camara.w)
    {
        game->pantalla.camara.x = game->pantalla.nivel_w - game->pantalla.camara.w;
    }
    
    if (game->pantalla.camara.y > game->pantalla.nivel_h - game->pantalla.camara.h)
    {
        game->pantalla.camara.y = game->pantalla.nivel_h - game->pantalla.camara.h;
    }
   
    //         Mecanica de los  enemigos
    for (int i = 0; i < max_enemigos; i++)
    {
        if (game->enemigos[i].activo)
        {
            float paso_enm = game->enemigos[i].velocidad * game->delta_time;
            
            // si no esta en persecucion la comprobamos
            if (!game->enemigos[i].perseguir) 
            {
                if (game->enemigos[i].es_bote)
                {
                    if (esta_en_agua(game, &game->enemigos[i].rect))
                    {
                        if(!game->enemigos[i].escapando)
                        {
                            bote_agua(game, i); // fcion bote agua
                            game->enemigos[i].escapando = 1;
                        }
                    } 
                    else 
                    {
                        game->enemigos[i].escapando = 0;
                    }
                    bote_dispara(game, i); // dispara si es bote
                } 
                else if (game->enemigos[i].es_torreta) 
                {
                    // CASO SI ES TORRETA
                    game->enemigos[i].escapando = 0;
                    bote_dispara(game, i);
                } 
                else
                {
                    // si no es bote ni torreta es enemigo en tierra
                    if (esta_en_agua(game, &game->enemigos[i].rect))
                    {
                        if(!game->enemigos[i].escapando) 
                        {
                            enemigos_escapan(game, i); // funcion patrullas
                            game->enemigos[i].escapando = 1;
                        }
                    } 
                    else 
                    {
                        game->enemigos[i].escapando = 0;
                    }
                }
                game->enemigos[i].x += game->enemigos[i].dir_x * paso_enm;
                game->enemigos[i].y += game->enemigos[i].dir_y * paso_enm;

                
                if (game->enemigos[i].es_camion)
                {
                    margen_camion = 4;
                }

                SDL_Rect rect_enm1 = {
                    (int)(game->enemigos[i].x) + margen_camion,
                    (int)(game->enemigos[i].y) + margen_camion,
                    game->enemigos[i].lado - (2*margen_camion),
                    game->enemigos[i].lado - (2*margen_camion)
                };
                
                int chocar = chequea_tiles(game, &rect_enm1, 1) || rect_enm1.x >= game->pantalla.nivel_w - game->enemigos[i].lado || rect_enm1.x <= 0;
                
                if (!chocar && game->enemigos[i].es_bote && !esta_en_agua(game, &rect_enm1)) chocar = 1;
                if (chocar) 
                {
                    game->enemigos[i].x -= game->enemigos[i].dir_x*(paso_enm);
                    game->enemigos[i].y -= game->enemigos[i].dir_y*(paso_enm);

                    game->enemigos[i].dir_x *= -1;
                    game->enemigos[i].dir_y *= -1;
                }
            } 
            else 
            {

                float dx = game->jugador.x - game->enemigos[i].x;
                float dy = game->jugador.y - game->enemigos[i].y;
                float dist = sqrtf(dx*dx + dy*dy);
                
                if (dist > RADIO_PERDIDO)
                {
                    game->enemigos[i].perseguir = false;
                    game->enemigos[i].sirena = false;  
                } 
                else 
                {
                    if (game->enemigos[i].cooldown_disparo > 0.0f)
                    {
                    game->enemigos[i].cooldown_disparo -= game->delta_time;
                    }
    
                    if (game->enemigos[i].cooldown_disparo <= 0.0f && dist > 0.0f) 
                    {
                        for (int p = 0; p<MAX_PROYECTILES; p++) 
                        {
                            if(!game->enemigos[i].proyectiles[p].activo) 
                            {
                                game->enemigos[i].proyectiles[p].activo = true;
                                game->enemigos[i].proyectiles[p].x = game->enemigos[i].x + (game->enemigos[i].lado/2.0f);
                                game->enemigos[i].proyectiles[p].y = game->enemigos[i].y + (game->enemigos[i].lado/2.0f);
        
                                game->enemigos[i].proyectiles[p].dir_x = dx/dist;
                                game->enemigos[i].proyectiles[p].dir_y = dy/dist;
        
                                game->enemigos[i].proyectiles[p].velocidad = VELOCIDAD_DISPARO;
                                game->enemigos[i].proyectiles[p].lado = 6;
                                game->enemigos[i].cooldown_disparo = COOLDOWN_DISPARO;
                                game->enemigos[i].proyectiles[p].sonido = true;
                                if (game->bala != NULL && dist < RADIO_PERDIDO) 
                                {
                                    printf("Sonido bala!\n");
                                    Mix_PlayChannel(-1, game->bala, 0);
                                }
                                
                                break; //necesario romper el ciclo para que no continue
                            }
                        }       
                    }
                }   
                
                if (dist > RADIO_EJ)
                {
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
                        if (paso_x >= 0) 
                        {
                            game->enemigos[i].dir_x = 1;
                        } 
                        else 
                        {
                            game->enemigos[i].dir_x = -1;
                        }
                    }
                    
                    rect_enm2.x = (int)game->enemigos[i].x;
                    rect_enm2.y = (int)(game->enemigos[i].y + paso_y);
                    
                    if (!chequea_tiles(game, &rect_enm2, 1) && !SDL_HasIntersection(&rect_enm2, &game->jugador.rect) && !chequea_entre_enemigos(game, &rect_enm2, i) && rect_enm2.y >= 0 && rect_enm2.y <= game->pantalla.nivel_h - game->enemigos[i].lado)
                    {
                        game->enemigos[i].y += paso_y;
                        if (paso_y >= 0)
                        {
                            game->enemigos[i].dir_x = 1;
                        } 
                        else 
                        {
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

    // inicio resta cooldown por tiempo (delta_time)
    if (game->jugador.cooldown_agua > 0.0f) 
    {
        game->jugador.cooldown_agua -= game->delta_time;
    }
    if (game->jugador.cooldown_choque > 0.0f) 
    {
        game->jugador.cooldown_choque -= game->delta_time;
    }

    dispara_jugador(game); //mecanica para que mi jugador dispare hacia una direccion en rads
    
    for (int i=0; i<max_enemigos; i++)
    {
        actualiza_proyectiles(game, game->enemigos[i].proyectiles, true);
    }

    // actualiza reloj de proyectiles y explosiones
    actualiza_proyectiles(game, game->jugador.proyectiles, false);
    actualiza_explosiones(game);
}

// chequea proyectiles del enemigo y jugador:
void actualiza_proyectiles(Game *game, Proyectil *proyectiles, bool es_enemigo)
{
    for (int p=0; p<MAX_PROYECTILES; p++) 
    {
        if(!proyectiles[p].activo) continue;
        
        proyectiles[p].x += proyectiles[p].dir_x * proyectiles[p].velocidad * game->delta_time;
        proyectiles[p].y += proyectiles[p].dir_y * proyectiles[p].velocidad * game->delta_time;
    
        SDL_Rect rect_bala = {
            .x = (int)proyectiles[p].x,
            .y = (int)proyectiles[p].y,
            .w = proyectiles[p].lado,
            .h = proyectiles[p].lado
        };
    
        if (rect_bala.x < 0 || rect_bala.y < 0 || chequea_tiles(game, &rect_bala, 2))
        {
           proyectiles[p].activo = false;
           continue;
        }
        
        if (es_enemigo)
        {
            // caso bala hacia jugador
            if (SDL_HasIntersection(&rect_bala, &game->jugador.rect)) 
            {
                proyectiles[p].activo = false;
                game->jugador.hp -= 1;
                printf("El jugador recibio disparo !! -1 de HP : \n--HP: %d --velocidad_actual: %.2f\n", game->jugador.hp, game->jugador.velocidad_actual);
                
                if (game->jugador.hp <= 0) 
                {
                    printf("El jugador murio!!!!\n");
                    jugador_muere(game);
                }
            } 
        } 
        else 
        { // caso enemigo recibe bala
            for (int i=0; i<max_enemigos; i++)
            {
                if (game->enemigos[i].activo && SDL_HasIntersection(&rect_bala, &game->enemigos[i].rect)) 
                {
                    proyectiles[p].activo = false;
                    if (game->enemigos[i].es_camion || game->enemigos[i].es_bote || game->enemigos[i].es_torreta) 
                    {
                        printf("Es camion / bote / torreta!!!! falta agrandar\n");
                    } 
                    else 
                    {
                        game->enemigos[i].perseguir = true;
                        game->enemigos[i].indicador = true;
                    }
                    game->enemigos[i].hp--;
                    
                    printf("Enemigo n°%d recibio disparo! y ahora tiene HP: %d\n", i, game->enemigos[i].hp);
                    if (game->enemigos[i].hp <= 0)
                    {
                        if (game->enemigos[i].es_camion == true)
                        {
                            SDL_Log("BOOM!!!!\n");
                            if (game->explosion != NULL)
                            {
                                Mix_PlayChannel(-1, game->explosion, 0);
                            }
                            game->contador_camiones--;
                            game->ranking.cuenta_camiones++; //cuenta de kills para pje

                            if (game->contador_camiones < 0)
                            {
                                game->ranking.cuenta_camiones_extra++;
                            }
                            crea_explosion(game, game->enemigos[i].x, game->enemigos[i].y);
                        }
                        else
                        {
                            game->ranking.cuenta_enemigos++; //cuenta kills de enemigos para puntaje
                        }
                        game->enemigos[i].activo = false;
                        printf("Enemigo n%d debería desaparecer\n", i);
                    }
                    break;
                }
            }
        }
    }
}

int chequea_tiles(Game *game, SDL_Rect *player_rect, int es_enemigo)
{
    // LO MISMO Q EN RENDER.C
    for (int i = 0; i < tile_filas; i++)
    {
        for (int j = 0; j < (int)tile_cols; j++)
        {
            if (game->tiles[i][j].activo)
            {
                SDL_Rect temp4 = {
                    .x = game->tiles[i][j].x_tiles,
                    .y = game->tiles[i][j].y_tiles,
                    .w = game->tiles[i][j].w_tiles,
                    .h = game->tiles[i][j].h_tiles
                };
                
                // comprueba si chocaste con cualquier tile
                if (SDL_HasIntersection(player_rect, &temp4))
                {
                    return 1;
                }
            }

            if (game->tiles[i][j].direccion == 1 || game->tiles[i][j].direccion == -1)
            {
                SDL_Rect tempdir = {
                    .x = game->tiles[i][j].x_tiles,
                    .y = game->tiles[i][j].y_tiles,
                    .w = game->tiles[i][j].w_tiles,
                    .h = game->tiles[i][j].h_tiles
                };
                
                // comprueba si chocaste con cualquier tile
                if (SDL_HasIntersection(player_rect, &tempdir))
                {
                    return 1;
                }
            }
            
            if (game->tiles[i][j].casa)
            {
                    SDL_Rect tempcasa = {
                        .x = game->tiles[i][j].x_tiles,
                        .y = game->tiles[i][j].y_tiles,
                        .w = game->tiles[i][j].w_tiles,
                        .h = game->tiles[i][j].h_tiles
                    };
                
                    if (SDL_HasIntersection(player_rect, &tempcasa))
                    {
                        // si es jugador e impacta
                        if (!es_enemigo && game->jugador.cooldown_choque <= 0.0f)
                        {

                            if (game->jugador.velocidad_actual >= 150.0f)
                            {
                                game->jugador.hp -= 2;
                                game->jugador.cooldown_choque = 3.0f; // 3 segundo
                                printf("Choque fuerte... HP: %d\n", game->jugador.hp);
                                if (game->jugador.hp <= 0)
                                {
                                    jugador_muere(game);
                                }
                            } 
                            else if (game->jugador.velocidad_actual > 80.0f)
                            {
                                game->jugador.hp -= 1;
                                game->jugador.cooldown_choque = 3.0f;
                                printf("Choque suave... HP: %d\n", game->jugador.hp);
                                if (game->jugador.hp <= 0)
                                {
                                    jugador_muere(game);
                                }
                            }
                        }

                        return 1;
                    }
            }

            // MECANICA DEL .AGUA
            if (game->tiles[i][j].agua)
            {
                SDL_Rect tempagua = {
                    .x = game->tiles[i][j].x_tiles,
                    .y = game->tiles[i][j].y_tiles,
                    .w = game->tiles[i][j].w_tiles,
                    .h = game->tiles[i][j].h_tiles
                };
                
                if (!es_enemigo)
                {
                    // lo alineamos al centro del tile para que no nos haga dano solo rozando
                    int centro_x = player_rect->x + (player_rect->w / 2);
                    int centro_y = player_rect->y + (player_rect->h / 2);
                    
                    if(centro_x >= tempagua.x && centro_x <= tempagua.x + tempagua.w)
                    {
                        if (centro_y >= tempagua.y && centro_y <= tempagua.y + tempagua.h)
                        {
                            game->jugador.velocidad_actual *= 0.8f;

                            if (game->jugador.cooldown_agua <= 0.0f)
                            {
                                game->jugador.hp -= 1;
                                game->jugador.cooldown_agua = 0.8f;
                                printf("Dano x agua!. hp actual jugador: %d\n", game->jugador.hp);
                                if (game->jugador.hp <= 0)
                                {
                                    jugador_muere(game);
                                }
                            }
                        }
                    }
                    /*if (SDL_HasIntersection(player_rect, &tempagua)) {
                        if (!es_enemigo) return 1;
                    }*/
                }
            }
            
            if (game->tiles[i][j].movediza >= 1)
            {
                SDL_Rect temparena = {
                    .x = game->tiles[i][j].x_tiles + margen,
                    .y = game->tiles[i][j].y_tiles + margen,
                    .w = game->tiles[i][j].w_tiles - margen,
                    .h = game->tiles[i][j].h_tiles - margen
                };
                
                if (!es_enemigo)
                {
                    if (SDL_HasIntersection(player_rect, &temparena))
                    {
                        if (game->tiles[i][j].movediza == 2)
                        {
                            game->jugador.velocidad_actual -= POTENCIA_FRENO * game->delta_time;
                            if (game->jugador.velocidad_actual <= LIMITE)
                            {
                                game->jugador.velocidad_actual = LIMITE;
                                game->jugador.dir_x = 0;
                                game->jugador.dir_y = 0;
                                game->jugador.tiempo_arranque = 0.0f;
                            }
                            game->jugador.velocidad_actual = (game->delta_time) * (game->jugador.velocidad_actual);
                        }
                        else
                        {
                            game->jugador.velocidad_actual *= 0.5;
                        }
                    }
                }
            }
            
            if (game->tiles[i][j].meta)
            {
                SDL_Rect tempmeta = {
                    .x = game->tiles[i][j].x_tiles,
                    .y = game->tiles[i][j].y_tiles,
                    .w = game->tiles[i][j].w_tiles,
                    .h = game->tiles[i][j].h_tiles - 2*margen
                };
                
                if (SDL_HasIntersection(player_rect, &tempmeta))
                {
                    if (es_enemigo) return 1; // enemigo choca con meta
                    
                    if (game->contador_camiones > 0 && game->nivel_actual != 1)
                    {
                        printf("Aun te faltan %d camiones!\n", game->contador_camiones);
                        return 1;
                    }

                    Uint32 tiempo_actual = SDL_GetTicks();
                    Uint32 cooldown_meta = 20000; // 20 segundos
                    if (tiempo_actual - game->ultimo_tiempo_meta >= cooldown_meta)
                    {
                        game->llego_meta = true;
                        if (game->llego_meta == true && tiempo_actual - game->ultimo_tiempo_meta >= cooldown_meta)
                        {
                            funcion_meta(game);
                        }
                    }
                }
            }

            if (!es_enemigo && game->tiles[i][j].aceite)
            {
                SDL_Rect tempaceite = {
                    .x = game->tiles[i][j].x_tiles,
                    .y = game->tiles[i][j].y_tiles,
                    .w = game->tiles[i][j].w_tiles - 2*margen,
                    .h = game->tiles[i][j].h_tiles - 2*margen
                };

                if (SDL_HasIntersection(player_rect, &tempaceite))
                {
                    game->jugador.velocidad_actual = 100.0f;
                    game->jugador.x -= 0.2;
                }
            }
        }
    }
    
    // RECTANGULO DESTRUIBLE !!!
    for (int i=0; i<tile_filas; i++)
    {
        for (int j=0; j<tile_cols; j++)
        {
            if (game->tiles[i][j].objeto2)
            {
                SDL_Rect temp5 = {
                    game->tiles[i][j].x_tiles,
                    game->tiles[i][j].y_tiles,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };
                
                if (SDL_HasIntersection(player_rect, &temp5))
                {
                    if (es_enemigo)
                    {
                        return 1;
                    } 
                    else 
                    {
                        game->tiles[i][j].objeto2 = false;
                        game->jugador.velocidad_actual *= 0.5f;
                        SDL_Log("caja destruida!");
                        game->jugador.velocidad_actual *= 1.2f;
                        if (game->jugador.hp <= HP_MAX)
                        {
                            game->jugador.hp += 1;
                            if (game->powerup != NULL)
                            {
                                Mix_PlayChannel(-1, game->powerup, 0);
                            }
                        }
                    }
                }
            }
            
            if (game->tiles[i][j].objetominimapa)
            {
                SDL_Rect tempW = {
                    game->tiles[i][j].x_tiles,
                    game->tiles[i][j].y_tiles,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };
                if (SDL_HasIntersection(player_rect, &tempW))
                {
                    if (es_enemigo) return 1;
                    
                    game->tiles[i][j].objetominimapa = false;
                    game->jugador.minimapa_activo = true;
                    SDL_Log("Minimapa activado!!!!!!!!!!!!!!!");
                    if (game->powerup != NULL)
                    {
                        Mix_PlayChannel(-1, game->powerup, 0);
                    }
                }
            }

            if (game->tiles[i][j].objeto3)
            {
                SDL_Rect temp6 = {
                    game->tiles[i][j].x_tiles,
                    game->tiles[i][j].y_tiles,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };
                
                if (SDL_HasIntersection(player_rect, &temp6)) 
                {
                    if (es_enemigo) 
                    {
                        return 1;
                    } 
                    else 
                    {
                        game->tiles[i][j].objeto3 = false;
                        game->jugador.velocidad_actual *= 0.6f;
                        SDL_Log("caja de 5 balas destruida!!");
                        game->jugador.velocidad_actual *= 1.2f;
                        game->jugador.contador_balas += 5;
                        if (game->powerup != NULL)
                        {
                            Mix_PlayChannel(-1, game->powerup, 0);
                        }
                    }
                }
            }
        }
    }
    return 0; 
}

int chequea_enemigos(Game *game, SDL_Rect *player_rect)
{
    for (int i = 0; i < max_enemigos; i++)
    {
        if (game->enemigos[i].activo && !game->enemigos[i].es_camion && !game->enemigos[i].es_bote && !game->enemigos[i].es_torreta)
        {
            SDL_Rect hitbox_enemigo = {
                game->enemigos[i].rect.x + margen,
                game->enemigos[i].rect.y + margen,
                game->enemigos[i].rect.w - (margen*2),
                game->enemigos[i].rect.h - (margen*2)
            };
            
            if (SDL_HasIntersection(player_rect, &hitbox_enemigo))
            {
                if (!game->enemigos[i].perseguir)
                {
                    game->enemigos[i].perseguir = true;
                    game->enemigos[i].sirena = true;
                    game->enemigos[i].indicador = true;
                    printf("Patrulla N°%d Perseguira!!\n", i);
                    game->enemigos[i].velocidad = 0.95 * (game->jugador.velocidad);

                    if (game->enemigos[i].sirena == true && game->enemigos[i].perseguir == true)
                    {
                        if (game->sirena1 != NULL)
                        {
                            printf("pato\n");
                            Mix_PlayChannel(-1, game->sirena1, 0);
                            break;
                        }
                        game->enemigos[i].sirena == false;
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}

int enemigos_escapan(Game *game, int numero_enemigo)
{
    // game->enemigos[numero_enemigo].OBJETO
    SDL_Rect rect_enemigo = {
        (int)game->enemigos[numero_enemigo].x,
        (int)game->enemigos[numero_enemigo].y,
        (int)game->enemigos[numero_enemigo].lado,
        (int)game->enemigos[numero_enemigo].lado
    };
    
    for (int i=0; i<tile_filas; i++)
    {
        for (int j=0; j<tile_cols; j++)
        {
            if (!game->tiles[i][j].agua) continue;
            // rect temporal ubicacion agua
            SDL_Rect temp_agua = {
                .x = game->tiles[i][j].x_tiles,
                .y = game->tiles[i][j].y_tiles,
                .w = game->tiles[i][j].w_tiles,
                .h = game->tiles[i][j].h_tiles
            };
            
            if (!SDL_HasIntersection(&rect_enemigo, &temp_agua)) continue;

            SDL_Rect rprueba; // rect vacio
            printf("enemigo %d buscando escapatoria. Tamano lado enemigo: %d\n", numero_enemigo, game->enemigos[numero_enemigo].lado);
            printf("Dir: %d %d\n", game->enemigos[numero_enemigo].dir_x, game->enemigos[numero_enemigo].dir_y);

            // caso derecha >
            rprueba = rect_enemigo;
            rprueba.x += game->enemigos[numero_enemigo].lado;
            if (!esta_en_agua(game, &rprueba))
            {
                game->enemigos[numero_enemigo].dir_x = 1;
                game->enemigos[numero_enemigo].dir_y = 0;
                printf("Derecha\n");
                return 1;
            }

            // caso izquierda <
            rprueba = rect_enemigo;
            rprueba.x -= game->enemigos[numero_enemigo].lado;
            if (!esta_en_agua(game, &rprueba))
            {
                game->enemigos[numero_enemigo].dir_x = -1;
                game->enemigos[numero_enemigo].dir_y = 0;
                printf("Izquierda\n");
                return 1;
            }
            
            // caso abajo
            rprueba = rect_enemigo;
            rprueba.y += game->enemigos[numero_enemigo].lado;
            if (!esta_en_agua(game, &rprueba))
            {
                game->enemigos[numero_enemigo].dir_x = 0;
                game->enemigos[numero_enemigo].dir_y = 1;
                printf("Abajo\n");
                return 1;
            }

            // caso arriba
            rprueba = rect_enemigo;
            rprueba.y -= game->enemigos[numero_enemigo].lado;
            if (!esta_en_agua(game, &rprueba))
            {
                game->enemigos[numero_enemigo].dir_x = 0;
                game->enemigos[numero_enemigo].dir_y = -1;
                printf("Arriba\n");
                return 1;
            }
            printf("Posicion(x,y): %.2f %.2f\n", game->enemigos[numero_enemigo].x, game->enemigos[numero_enemigo].y);
            
            return 1; // funcion entero
        }
    }
    return 0;
}

int bote_agua(Game *game, int numero_enemigo) // SOLO FUE COPIAR LA FUNCION DE ARRIBA Y SACARLE EL !
{
    SDL_Rect rect_enemigo = {
        (int)game->enemigos[numero_enemigo].x,
        (int)game->enemigos[numero_enemigo].y,
        (int)game->enemigos[numero_enemigo].lado,
        (int)game->enemigos[numero_enemigo].lado 
    };
    SDL_Rect rprueba;
    
    // !enemigos_escapan
    // caso derecha >
    rprueba = rect_enemigo;
    rprueba.x += game->enemigos[numero_enemigo].lado;
    if (esta_en_agua(game, &rprueba))
    {
        game->enemigos[numero_enemigo].dir_x = 1;
        game->enemigos[numero_enemigo].dir_y = 0;
        return 1;
    }

    // caso izquierda <
    rprueba = rect_enemigo;
    rprueba.x -= game->enemigos[numero_enemigo].lado;
    if (esta_en_agua(game, &rprueba))
    {
        game->enemigos[numero_enemigo].dir_x = -1;
        game->enemigos[numero_enemigo].dir_y = 0;
        return 1;
    }
    
    // caso abajo
    rprueba = rect_enemigo;
    rprueba.y += game->enemigos[numero_enemigo].lado;
    if (esta_en_agua(game, &rprueba))
    {
        game->enemigos[numero_enemigo].dir_x = 0;
        game->enemigos[numero_enemigo].dir_y = 1;
        return 1;
    }

    // caso arriba
    rprueba = rect_enemigo;
    rprueba.y -= game->enemigos[numero_enemigo].lado;
    if (esta_en_agua(game, &rprueba))
    {
        game->enemigos[numero_enemigo].dir_x = 0;
        game->enemigos[numero_enemigo].dir_y = -1;
        return 1;
    }
    return 0; // no encontro agua
}

// y torretas
void bote_dispara(Game *game, int numero_enemigo)
{
    if (game->enemigos[numero_enemigo].cooldown_disparo > 0.0f)
    {
        game->enemigos[numero_enemigo].cooldown_disparo -= game->delta_time;
    }

    float dx = game->jugador.x - game->enemigos[numero_enemigo].x;
    float dy = game->jugador.y - game->enemigos[numero_enemigo].y;
    float dist = sqrtf(dx*dx + dy*dy);

    if (dist <= RADIO_PERDIDO) 
    {
        if (!game->enemigos[numero_enemigo].es_camion && game->enemigos[numero_enemigo].cooldown_disparo <= 0.0f && dist > 0.0f) 
        {
            for (int p = 0; p<MAX_PROYECTILES; p++) 
            {
                if(!game->enemigos[numero_enemigo].proyectiles[p].activo) 
                {
                    game->enemigos[numero_enemigo].proyectiles[p].activo = true;

                    game->enemigos[numero_enemigo].proyectiles[p].x = game->enemigos[numero_enemigo].x + (game->enemigos[numero_enemigo].lado/2.0f);
                    game->enemigos[numero_enemigo].proyectiles[p].y = game->enemigos[numero_enemigo].y + (game->enemigos[numero_enemigo].lado/2.0f);

                    game->enemigos[numero_enemigo].proyectiles[p].dir_x = dx/dist;
                    game->enemigos[numero_enemigo].proyectiles[p].dir_y = dy/dist;

                    game->enemigos[numero_enemigo].proyectiles[p].velocidad = VELOCIDAD_DISPARO;
                    game->enemigos[numero_enemigo].proyectiles[p].lado = 6;
                    game->enemigos[numero_enemigo].cooldown_disparo = COOLDOWN_DISPARO;
                    game->enemigos[numero_enemigo].proyectiles[p].sonido = true;
                    
                    if (game->bala != NULL && dist < RADIO_PERDIDO)
                    {
                        Mix_PlayChannel(-1, game->bala, 0);
                    }
                    break; //necesario romper el ciclo para que no continue
                }
            }       
        }
    }
    
}

void empuja_camion(Game *game)
{
    SDL_Rect rect_jugador = {
        (int)game->jugador.x,
        (int)game->jugador.y,
        game->jugador.lado,
        game->jugador.lado
    };

    for (int i=0; i<max_enemigos; i++)
    {
        if(!game->enemigos[i].activo || !game->enemigos[i].es_camion) continue;
        SDL_Rect rect_camion = {
            (int)game->enemigos[i].x,
            (int)game->enemigos[i].y,
            game->enemigos[i].lado,
            game->enemigos[i].lado
        };

        if (SDL_HasIntersection(&rect_jugador, &rect_camion)) 
        {
            float rad = game->jugador.angulo * PI / 180.0f; //ANGULO A RADIANES PARA FUNC MATH.H
            float empuje = EMPUJE_CAMION * game->delta_time;
            game->jugador.x += sinf(rad) * empuje;
            game->jugador.y += -cosf(rad) * empuje;

            // para q no se salga del mapa mi jugador (limites)
            if (game->jugador.x < 0) game->jugador.x = 0;
            if (game->jugador.y < 0) game->jugador.y = 0;
            if (game->jugador.x > game->pantalla.nivel_w - game->jugador.lado) 
                game->jugador.x = game->pantalla.nivel_w - game->jugador.lado;
            if (game->jugador.y > game->pantalla.nivel_h - game->jugador.lado) 
                game->jugador.y = game->pantalla.nivel_h - game->jugador.lado;

            return;
        }
    }
}

void dispara_jugador(Game *game)
{
    // chequea el cooldown del disparo y le restamos x tiempo
    if (game->jugador.cooldown_disparo > 0.0f)
    {
        game->jugador.cooldown_disparo -= game->delta_time;
    }
    // si jugador dispara y el cooldown termino
    if (game->jugador.disparo && game->jugador.cooldown_disparo <= 0.0f) 
    {
        // ° a radianes
        float rad = game->jugador.angulo * PI / 180.0f;
        float dir_x = sinf(rad);
        float dir_y = -cosf(rad);

        for (int p=0; p<MAX_PROYECTILES; p++) 
        {
            if (!game->jugador.proyectiles[p].activo && game->jugador.contador_balas > 0)
            {
                game->jugador.proyectiles[p].activo = true;
                game->jugador.proyectiles[p].x = game->jugador.x + (game->jugador.lado/2.0f);
                game->jugador.proyectiles[p].y = game->jugador.y + (game->jugador.lado/2.0f);
                
                game->jugador.proyectiles[p].dir_x = dir_x;
                game->jugador.proyectiles[p].dir_y = dir_y;

                game->jugador.proyectiles[p].velocidad = VELOCIDAD_DISPARO;
                game->jugador.proyectiles[p].lado = 6;
                game->jugador.proyectiles[p].es_enemigo = false;
                game->jugador.cooldown_disparo = COOLDOWN_DISPARO_JUGADOR;
                game->jugador.proyectiles[p].sonido = true;
                game->jugador.contador_balas--; //inicializado en MAX_PROYECTILES
                printf("Numero balas: %d\n", game->jugador.contador_balas);
               
                if (game->bala != NULL)
                {
                    printf("Sonido bala! (jugador)\n");
                    Mix_PlayChannel(-1, game->bala, 0);
                }
                break;
            }
        }
    }
}

int esta_en_agua(Game *game, SDL_Rect *rect)
{
    for (int i=0; i<tile_filas; i++)
    {
        for (int j=0; j<tile_cols; j++)
        {
            if (game->tiles[i][j].agua)
            {
                SDL_Rect agua = {
                    game->tiles[i][j].x_tiles,
                    game->tiles[i][j].y_tiles,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };
                
                if (SDL_HasIntersection(rect, &agua)) return 1;
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

void funcion_meta(Game *game) //cambiar a int
{
    // aca ira lo que tengo arriba respecto a la meta la gracia de esto es la escritura archivo
    printf("Felicidades has llegado a la meta en %s!! Vuelta Num %d\n", game->interfaz.texto_cronometro, game->vueltas);
    funcion_misiones(game);
    
    game->vueltas++;
    if (game->nivel_actual == 3)
    {
        int tiempo_transcurrido = SDL_GetTicks() - game->tiempo_inicio;
        game->ranking.tiempo = tiempo_transcurrido / 1000;
        // puntaje
        calcula_puntaje(game, true);
        char nombre[20];
        panel_nombre(game, nombre, sizeof(nombre));
    
        guarda_puntaje(game, nombre, game->ranking.puntaje, game->nivel_actual, game->ranking.tiempo);
        endgame(game, true);

        game_Menu(game);
        if (!game->quit)
        {
            game->nivel_actual = 1;
            game->vueltas = 1;
            game->jugador.hp = HP_INICIAL;
            game->jugador.contador_balas = MAX_PROYECTILES;
            
            game->jugador.velocidad_actual = 0.0f;
            game->jugador.dir_x = 0;
            game->jugador.dir_y = 0;
            game->jugador.up = 0; game->jugador.down = 0;
            game->jugador.left = 0; game->jugador.right = 0;
            game->jugador.disparo = 0; game->jugador.freno = 0;
            
            game->ranking.cuenta_camiones = 0;
            game->ranking.cuenta_enemigos = 0;
            game->ranking.puntaje = 0;
            game->ranking.cuenta_camiones_extra = 0;
            
            carga_Nivel(game, game->nivel_actual);
            
            SDL_Event limpia_queue;
            while (SDL_PollEvent(&limpia_queue)) { }
            
            game->tiempo_inicio = SDL_GetTicks();
            game->ultimo_tiempo_meta = SDL_GetTicks();
        }
    }
    else
    {
        siguiente_Nivel(game);
    }

    /*
    funcion guarda_puntaje
    fscanf(archi,"%s", ranking[i].nombre);
    fscanf(archi,"%f", ranking[i].tiempo);
    fscanf(archi,"%d", ranking[i].nivel);
    */ 
}

void jugador_muere(Game *game)
{
    printf("GAME OVER - jugador murio nivel %d", game->nivel_actual);

    int tiempo_transcurrido = SDL_GetTicks() - game->tiempo_inicio;
    game->ranking.tiempo = tiempo_transcurrido/1000;

    // calcula puntaje y pide nombre si se muere jugador
    calcula_puntaje(game, false);
    char nombre[20];
    panel_nombre(game, nombre, sizeof(nombre));
    guarda_puntaje(game, nombre, game->ranking.puntaje, game->nivel_actual, game->ranking.tiempo);

    // redirecciona ranking y menu al morir
    endgame(game, false);
    game_Menu(game);
    if (!game->quit)
    {
        // limpieza y reset antes de ir al menu importante !!!
        game->nivel_actual = 1;
        game->vueltas = 1;
        game->jugador.hp = HP_INICIAL;
        game->jugador.contador_balas = MAX_PROYECTILES;
        
        // reinicio teclas y auto
        game->jugador.velocidad_actual = 0.0f;
        game->jugador.dir_x = 0;
        game->jugador.dir_y = 0;
        game->jugador.up = 0; game->jugador.down = 0;
        game->jugador.left = 0; game->jugador.right = 0;
        game->jugador.disparo = 0; game->jugador.freno = 0;
        
        // reinicio puntajes y nivel
        game->ranking.cuenta_camiones = 0;
        game->ranking.cuenta_enemigos = 0;
        game->ranking.puntaje = 0;
        game->ranking.cuenta_camiones_extra = 0;
        carga_Nivel(game, game->nivel_actual);
        
        // vacia entrada, input y cola eventos
        SDL_Event limpia_queue;
        while (SDL_PollEvent(&limpia_queue)) { }
        
        // reinicia cronometro delta time
        game->tiempo_inicio = SDL_GetTicks();
        game->ultimo_tiempo_meta = SDL_GetTicks();
    }
}

void panel_nombre(Game *game, char *nombre, size_t max_caracteres)
{
    // iniciando
    bool ingresando = true;
    SDL_Event evento;
    int longitud = 0;
    nombre[0] = '\0';

    SDL_StartTextInput();
    SDL_Color colorTexto = {255,255,255,255};
    SDL_Color colorFondo = {40,40,40,230};

    while (ingresando && !game->quit)
    {
        while (SDL_PollEvent(&evento))
        {
            if (evento.type == SDL_QUIT)
            {
                game->quit = true;
                ingresando = false;
            }
            else if (evento.type == SDL_KEYDOWN)
            {
                if (evento.key.keysym.sym == SDLK_RETURN)
                {
                    Mix_PlayChannel(-1, game->sel_menu, 0);
                    if (longitud > 0)
                    {
                        ingresando = false;
                    }
                }
                else if (evento.key.keysym.sym == SDLK_BACKSPACE)
                {
                    Mix_PlayChannel(-1, game->sel_menu, 0);
                    if (longitud > 0)
                    {
                        longitud--;
                        nombre[longitud] = '\0';
                    }
                }
            }
            // entrada texto
            else if (evento.type == SDL_TEXTINPUT)
            {
                Mix_PlayChannel(-1, game->sel_menu, 0);
                if (longitud + strlen(evento.text.text) < max_caracteres)
                {
                    strcat(nombre, evento.text.text);
                    longitud += strlen(evento.text.text);
                }
            }
        }

        // RENDER PANEL
        SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->pantalla.renderer);
        if (game->texturaImg != NULL)
        {
            SDL_RenderCopy(game->pantalla.renderer, game->texturaImg, NULL, NULL);
        }

        // caja gris
        SDL_SetRenderDrawBlendMode(game->pantalla.renderer, SDL_BLENDMODE_BLEND);
        SDL_Rect rect_fondo = {
            5,
            5,
            game->pantalla.win_w - 10,
            game->pantalla.win_h - 10
        };
        SDL_SetRenderDrawColor(game->pantalla.renderer, 40, 40, 40, 230);
        SDL_RenderFillRect(game->pantalla.renderer, &rect_fondo);

        // TITULO
        SDL_Surface *s_titulo = TTF_RenderText_Solid(game->fuente, "NUEVO RECORD! ingresa nombre:", colorTexto);
        if (s_titulo)
        {
            SDL_Texture *tex_titulo = SDL_CreateTextureFromSurface(game->pantalla.renderer, s_titulo);
            SDL_Rect rect_titulo = {
                rect_fondo.x + (rect_fondo.w - s_titulo->w)/2,
                rect_fondo.y + 20,
                s_titulo->w,
                s_titulo->h
            };
            SDL_RenderCopy(game->pantalla.renderer, tex_titulo, NULL, &rect_titulo);
            SDL_FreeSurface(s_titulo);
            SDL_DestroyTexture(tex_titulo);
        }

        if (longitud > 0)
        {
            SDL_Surface *s_nombre = TTF_RenderText_Solid(game->fuente, nombre, colorTexto);
            if (s_nombre)
            {
                SDL_Texture *tex_nombre = SDL_CreateTextureFromSurface(game->pantalla.renderer, s_nombre);
                SDL_Rect rect_nombre = {
                    rect_fondo.x + (rect_fondo.w - s_nombre->w)/2,
                    rect_fondo.y + 80,
                    s_nombre->w,
                    s_nombre->h
                };
                SDL_RenderCopy(game->pantalla.renderer, tex_nombre, NULL, &rect_nombre);
                SDL_FreeSurface(s_nombre);
                SDL_DestroyTexture(tex_nombre);
            }
        }
        else
        {
            // GUION QUE PARPADEA CON EL TIEMPO _
            if ((SDL_GetTicks()/500) % 2 == 0)
            {
                SDL_Surface *surf_cursor = TTF_RenderText_Solid(game->fuente, "_", colorTexto);
                SDL_Texture *tex_cursor = SDL_CreateTextureFromSurface(game->pantalla.renderer, surf_cursor);
                SDL_Rect rect_cursor = { rect_fondo.x + rect_fondo.w / 2 - 10, rect_fondo.y + 80, surf_cursor->w, surf_cursor->h };
                SDL_RenderCopy(game->pantalla.renderer, tex_cursor, NULL, &rect_cursor);
                SDL_FreeSurface(surf_cursor);
                SDL_DestroyTexture(tex_cursor);
            }
        }
        // actualiza pantalla
        SDL_RenderPresent(game->pantalla.renderer);
        SDL_Delay(16);
    }
    SDL_StopTextInput();
}

void calcula_puntaje(Game *game, bool completado) 
{
    // puntaje por bajas
    int puntaje = (300 * game->ranking.cuenta_camiones) + (50*game->ranking.cuenta_enemigos);
    
    // bono por camiones extra
    puntaje += (500 * game->ranking.cuenta_camiones_extra);

    // bono por mando conectado :)
    if (game->mando) puntaje += 150;

    // bono por llegar al ultimo nivel
    if (game->nivel_actual >= MAX_NIVELES)
    {
        puntaje += (150*game->nivel_actual);
    }
    
    // bono por completar nivel
    if (completado)
    {
        puntaje += 100 + (1000 - game->ranking.tiempo);
        printf("%d tiempo:\n", game->ranking.tiempo);
    }

    // si se tarda como 15 minutos
    if (puntaje < 0) puntaje = 0;
    game->ranking.puntaje = puntaje;
}

void guarda_puntaje(Game *game, const char *nombre, int puntaje, int nivel, int tiempo)
{
    EntradaRanking score[MAX_RANKING];
    int c_lineas = 0;

    // iniciandolo para que no haya basura
    for (int i=0; i<MAX_RANKING; i++)
    {
        strcpy(score[i].nombre, "PepitoDefault");
        score[i].puntaje = 0;
        score[i].tiempo = 0;
        score[i].nivel = 0;
    }

    // archi en modo lectura
    FILE *archi = fopen("./data/score.txt", "r");
    if (archi != NULL)
    {
        for (int i=0; i<MAX_RANKING; i++)
        {
            // 19s para que no haya basura ni colapse
            if (fscanf(archi, "%19s %d %d %d", score[i].nombre, &score[i].puntaje, &score[i].tiempo, &score[i].nivel) == 4)
            {
                c_lineas++;
            } 
            else
            {
                break;
            }
        }
        fclose(archi);
    }

    // posicion donde se insertara score si lo supera
    int pos = -1;

    for (int i=0; i<MAX_RANKING; i++)
    {
        if (puntaje > score[i].puntaje)
        {
            pos = i;
            break;
        }
    }

    if (pos != -1)
    {
        // empezamos de abajo hacia arriba 9,8,7,...
        for (int i=MAX_RANKING - 1; i > pos; i--)
        {
            // para que no colapse :v
            if (i-1 >= 0) 
            {
                score[i] = score[i-1];
            }
        }

        // metemos los datos que enviamos x funcion
        strcpy(score[pos].nombre, nombre);
        score[pos].puntaje = puntaje;
        score[pos].nivel = nivel;
        score[pos].tiempo = tiempo;

        // ahora escribiremos en archi
        archi = fopen("./data/score.txt", "w");
        if (archi != NULL)
        {
            for (int i=0; i<MAX_RANKING; i++)
            {
                // si el puntaje existe
                if (score[i].puntaje > 0)
                {
                    fprintf(archi, "%s %d %d %d\n", score[i].nombre, score[i].puntaje, score[i].tiempo, score[i].nivel);
                }
            }
            fclose(archi);
        }
    }

}

int funcion_misiones(Game *game)
{
    printf("Funcion misiones nivel %d\n", game->nivel_actual);
    // caso misiones true : requisitos para avanzar de nivel
    // nivel 2 = 3 camiones
    // nivel 3 = todos los camiones
    // nivel 4 = sobrevivir
    //wipppppppp
    return 1;
}

void siguiente_Nivel(Game *game)
{
    // necesario el unsigned int (+) por la funcion GetTicks()
    Uint32 tiempo_transicion = SDL_GetTicks();
    Uint32 cooldown = 6000; // 6seg a ms
    bool en_transicion = true;

    // TEXTO RESUMEN - STATS DURANTE TRANSICION/COOLDOWN
    char texto_stats[120];
    snprintf(texto_stats, sizeof(texto_stats), "Nivel: %d | HP: %d | Balas: %d | Camiones: %d", game->nivel_actual, game->jugador.hp, game->jugador.contador_balas, game->contador_camiones+game->ranking.cuenta_camiones_extra);

    SDL_Color blanco = {255,255,255,255};
    SDL_Surface *s_stats = TTF_RenderText_Solid(game->fuente, texto_stats, blanco);
    SDL_Texture *t_stats = NULL;
    int sta_w = 0;
    int sta_h = 0;

    if (s_stats != NULL)
    {
        t_stats = SDL_CreateTextureFromSurface(game->pantalla.renderer, s_stats);
        sta_w = s_stats->w;
        sta_h = s_stats->h;
        SDL_FreeSurface(s_stats);
    }

    while (en_transicion && !game->quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                game->quit = true;
                en_transicion = false;
            }
        }

        // comprobacion tiempo: actual - inicial
        Uint32 tiempo_actual = SDL_GetTicks();
        if (tiempo_actual - tiempo_transicion >= cooldown)
        {
            en_transicion = false;
        }

        // reseteo del renderer
        SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->pantalla.renderer);

        // imagen del fondo bg.png
        if (game->texturaImg != NULL)
        {
            SDL_RenderCopy(game->pantalla.renderer, game->texturaImg, NULL, NULL);
        }

        // caja texto stats o resumen
        if (t_stats != NULL)
        {
            SDL_SetRenderDrawBlendMode(game->pantalla.renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(game->pantalla.renderer, 0,0,0,200);
            SDL_Rect fondo_tex = {5, 5, game->pantalla.win_w - 10, game->pantalla.win_h - 10};
            SDL_RenderFillRect(game->pantalla.renderer, &fondo_tex);

            SDL_Rect r_stats = {
                (int)(game->pantalla.win_w - sta_w)/2,
                (int)(game->pantalla.win_h - sta_h)/2,
                sta_w/2,
                sta_h/2
            };
            SDL_RenderCopy(game->pantalla.renderer, t_stats, NULL, &r_stats);
        }

        SDL_RenderPresent(game->pantalla.renderer);
        SDL_Delay(16);
    }

    if (t_stats != NULL) SDL_DestroyTexture(t_stats);

    if (!game->quit)
    {
        int siguiente = game->nivel_actual + 1;
        if (siguiente > MAX_NIVELES)
        {
            siguiente = 1;
        }
        
        game->jugador.up = 0;
        game->jugador.down = 0;
        game->jugador.left = 0;
        game->jugador.right = 0;
        game->jugador.freno = 0;
        game->jugador.disparo = 0;
        game->jugador.velocidad_actual = 0.0f;
        game->jugador.dir_x = 0;
        game->jugador.dir_y = 0;

        // funcion que carga y limpia tiles nivel
        carga_Nivel(game, siguiente);

        SDL_Event limpia_queue;
        while (SDL_PollEvent(&limpia_queue))
        {
            // reseteo cola/queue eventos
        }

        // reinicio cronometro
        game->tiempo_inicio = SDL_GetTicks();
        game->ultimo_tiempo_meta = SDL_GetTicks();
        game->vueltas = 1;
        game->llego_meta = false;
    }
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

void dano_explosion(Game *game, float x, float y)
{
    // centro explosion (para dp aplicar el radio) le sumamos 16
    float centro_x = x + (tam/2.0f);
    float centro_y = y + (tam/2.0f);

    // chequea contra jugador
    float dx = (game->jugador.x + game->jugador.lado/2.0f) - centro_x;
    float dy = (game->jugador.y + game->jugador.lado/2.0f) - centro_y;
    float dist = sqrtf(dx*dx + dy*dy);

    if (dist <= RADIO_EXPLOSION)
    {
        game->jugador.hp -= DANO_EXPLOSION;
        if (game->jugador.hp <= 0) {
            jugador_muere(game);
        }
    }

    for (int i=0; i<max_enemigos; i++)
    {
        if (!game->enemigos[i].activo) continue;
        // chequea contra cada enemigo del mapa
        float dx_e = (game->enemigos[i].x + game->enemigos[i].lado/2.0f) - centro_x;
        float dy_e = (game->enemigos[i].y + game->enemigos[i].lado/2.0f) - centro_y;
        float dist_e = sqrtf(dx*dx + dy*dy);
        
        if (dist_e <= RADIO_EXPLOSION) {
            game->enemigos[i].hp -= DANO_EXPLOSION;
            if (game->enemigos[i].hp <= 0) {
                game->enemigos[i].activo = false;
                // mata enemigos
            }
        }
    }
}

void crea_explosion(Game *game, float x, float y)
{
    for (int i=0; i<MAX_EXPLOSIONES; i++)
    {
        if (!game->explosiones[i].activa)
        {
            game->explosiones[i].x = x;
            game->explosiones[i].y = y;
            game->explosiones[i].tiempo = 0.0f;
            game->explosiones[i].activa = true;
            break;
        }
    }
    dano_explosion(game, x, y);
}

void actualiza_explosiones(Game *game)
{
    for (int i=0; i<MAX_EXPLOSIONES; i++)
    {
        if (!game->explosiones[i].activa) continue;
        game->explosiones[i].tiempo += game->delta_time;

        if (game->explosiones[i].tiempo >= DURACION_EXPLOSION)
        {
            game->explosiones[i].activa = false;
        }
    }
}

void endgame(Game *game, bool victoria)
{
    Mix_HaltMusic(); //saca la musica de fondo
    bool espera = true;
    SDL_Event evento;

    SDL_Color colorTexto = {255,255,255,255};
    SDL_Color colorTitulo;
    char texto_titulo[64];

    if (victoria)
    {
        colorTitulo = (SDL_Color){0,255,0,255};
        strcpy(texto_titulo, "NIVEL COMPLETADO!");
    }
    else
    {
        colorTitulo = (SDL_Color){255,50,50,255};
        strcpy(texto_titulo, "GAME OVER");
    }

    // resultados: pje y tiempo
    char texto_puntaje[64];
    snprintf(texto_puntaje, sizeof(texto_puntaje), "Puntaje final: $%d", game->ranking.puntaje);

    int minutos = game->ranking.tiempo / 60;
    int segundos = game->ranking.tiempo % 60;
    char texto_tiempo[64];
    snprintf(texto_tiempo, sizeof(texto_tiempo), "Tiempo: %02d:%02d", minutos, segundos);

    // reproduce audios victoria / game over
    if (game->audio.endgame != NULL && victoria == true)
    {
        SDL_Log("Audio endgame");
        Mix_PlayChannel(-1, game->audio.endgame, 0);
    }
    else if (game->audio.fail != NULL && victoria == false)
    {
        SDL_Log("Audio fail");
        Mix_PlayChannel(-1, game->audio.fail, 0);
    }

    // limpieza eventos
    while(SDL_PollEvent(&evento)) { }

    while (espera && !game->quit)
    {
        while(SDL_PollEvent(&evento))
        {
            if (evento.type == SDL_QUIT)
            {
                game->quit = true;
                espera = false;
            }
            else if (evento.type == SDL_KEYDOWN || evento.type == SDL_MOUSEBUTTONDOWN || evento.type == SDL_CONTROLLERBUTTONDOWN)
            {
                espera = false;
            }
        }

        SDL_SetRenderDrawColor(game->pantalla.renderer, 0,0,0,255);
        SDL_RenderClear(game->pantalla.renderer);
        if (game->texturaImg != NULL)
        {
            SDL_RenderCopy(game->pantalla.renderer, game->texturaImg, NULL, NULL);
        }

        // caja gris transparente
        SDL_SetRenderDrawBlendMode(game->pantalla.renderer, SDL_BLENDMODE_BLEND);
        SDL_Rect rect_fondo = {
            game->pantalla.win_w / 4, 
            game->pantalla.win_h / 4,
            game->pantalla.win_w / 2,
            game->pantalla.win_h / 2
        };
        SDL_SetRenderDrawColor(game->pantalla.renderer, 30, 30, 30, 200);
        SDL_RenderFillRect(game->pantalla.renderer, &rect_fondo);

        // renderizar texto
        SDL_Surface *s_titulo = TTF_RenderText_Solid(game->fuente, texto_titulo, colorTitulo);
        if (s_titulo)
        {
            SDL_Texture *t_titulo = SDL_CreateTextureFromSurface(game->pantalla.renderer, s_titulo);
            SDL_Rect r_titulo = {
                rect_fondo.x + (rect_fondo.w - s_titulo->w)/2, 
                rect_fondo.y + 40, 
                s_titulo->w, 
                s_titulo->h
            };
            SDL_RenderCopy(game->pantalla.renderer, t_titulo, NULL, &r_titulo);
            SDL_FreeSurface(s_titulo); SDL_DestroyTexture(t_titulo);
        }

        // puntajes
        SDL_Surface *s_pts = TTF_RenderText_Solid(game->fuente, texto_puntaje, colorTexto);
        if (s_pts) {
            SDL_Texture *t_pts = SDL_CreateTextureFromSurface(game->pantalla.renderer, s_pts);
            SDL_Rect r_pts = { rect_fondo.x + (rect_fondo.w - s_pts->w)/2, rect_fondo.y + 120, s_pts->w, s_pts->h };
            SDL_RenderCopy(game->pantalla.renderer, t_pts, NULL, &r_pts);
            SDL_FreeSurface(s_pts); SDL_DestroyTexture(t_pts);
        }

        // tiempo
        SDL_Surface *s_tmp = TTF_RenderText_Solid(game->fuente, texto_tiempo, colorTexto);
        if (s_tmp) {
            SDL_Texture *t_tmp = SDL_CreateTextureFromSurface(game->pantalla.renderer, s_tmp);
            SDL_Rect r_tmp = { rect_fondo.x + (rect_fondo.w - s_tmp->w)/2, rect_fondo.y + 180, s_tmp->w, s_tmp->h };
            SDL_RenderCopy(game->pantalla.renderer, t_tmp, NULL, &r_tmp);
            SDL_FreeSurface(s_tmp); SDL_DestroyTexture(t_tmp);
        }

        SDL_RenderPresent(game->pantalla.renderer);
        SDL_Delay(16);
    }
}