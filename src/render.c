#include "headers.h"

int hText = 16;
int wText = 32;
// estandar pixel art 32x32

void render_contbalas(Game *game);
void render_hp(Game *game); // render.c
void render_Cronometro(Game *game); // render.c
void render_nivel(Game *game);

void game_Render(Game *game)
{
    // objeto direccion (d/D)
    float angulo_dir = 0.0f;

    // TEXTO CRONOMETRO
    int tiempo_actual = SDL_GetTicks();
    int transcurrido = tiempo_actual - game->tiempo_inicio;
    int minutos = (transcurrido / 60000); // ms a min
    int segundos = (transcurrido / 1000)%60;
    int centesimas = (transcurrido % 1000)/10;

    // DESFASE CAMARA
    int desfase_x = -(game->pantalla.camara.x % tam);
    int desfase_y = -(game->pantalla.camara.y % tam);

    // limpieza inicial render
    SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->pantalla.renderer);

    // fondo segun nivel
    SDL_Rect origen_fondo;
    if (game->nivel_actual == 2 || game->nivel_actual == 4)
    {
        origen_fondo = (SDL_Rect){ 112, 0, tam, tam };
    } 
    else if (game->nivel_actual == 3)
    {
        origen_fondo = (SDL_Rect){ 448, 112, tam, tam };
    } 
    else
    {
        origen_fondo = (SDL_Rect){ 224, 192, tam, tam };
    }
    
    int x = 0, y = 0;
    for (y = desfase_y - tam; y < game->pantalla.win_h + tam; y = y+tam)
    {
        for (x = desfase_x - tam; x < game->pantalla.win_w + tam; x = x+tam)
        {
            SDL_Rect destino_fondo = { x, y, tam, tam };
            SDL_RenderCopy(game->pantalla.renderer, game->texturaPista, &origen_fondo, &destino_fondo);
        }
    }
    
     // Tilemap dibuja pista desde la textura y sdlrect
    for (int i=0; i < tile_filas; i++)
    {
        for (int j=0; j < (int)tile_cols; j++)
        {
            char tipo = game->tiles[i][j].tipo; //tipo de caracter en txt
            bool dibuja_pista = true;
            
            SDL_Rect destino = {
                game->tiles[i][j].x_tiles - game->pantalla.camara.x,
                game->tiles[i][j].y_tiles - game->pantalla.camara.y,
                game->tiles[i][j].w_tiles,
                game->tiles[i][j].h_tiles
            }; 
            
            // POSICIONES A RECORTAR:
            SDL_Rect origen = { 0,0,tam,tam };
            switch(tipo) 
            {
                // curvas con borde rojo
                case '1':
                    origen.x = 0; origen.y = 0; //(0,0) superior izq
                    break;
                case '2':
                    origen.x = 64; origen.y = 0; //(128,0) superior derecha
                    break;
                case '3':
                    origen.x = 0; origen.y = 64; // (0,128) inferior izq
                    break;
                case '4':
                    origen.x = 64; origen.y = 64; // (128,128) inferior derecha
                    break;
                // curvas sin borde
                case '5':
                    origen.x = 272; origen.y = 112;
                    break;
                case '6':
                    origen.x = 336; origen.y = 112;
                    break;
                case '7':
                    origen.x = 272; origen.y = 176;
                    break;
                case '8':
                    origen.x = 336; origen.y = 176;
                    break;
                // lineas horizontales
                case '-':
                    origen.x = 32; origen.y = 0; //(32,0) recta horiz 
                    break;
                case '+':
                    origen.x = 32; origen.y = 64;
                    break;
                case '=':
                case 'P':
                case 'W':
                    origen.x = 304; origen.y = 112;
                    break;
               // lineas verticales
                case '|':
                    origen.x = 0; origen.y = tam; //recta vertical (0, 64)
                    break;
                case 'I':
                    origen.x = 64; origen.y = tam;
                    break;
                case 'i':
                    origen.x = 272; origen.y = 144;
                    break;
                //asfalto !!!
                case '#': 
                    if (game->nivel_actual == 1){
                        origen.x = 144; origen.y = 192;
                    } else if (game->nivel_actual == 2) {
                        origen.x = 144; origen.y = 32;
                    } else { // asfalto nivel 3
                        origen.x = 512; origen.y = 112;
                    }
                    break;

                case 'C':
                    origen.x = 384; origen.y = 144;
                    break;
                case 'c':
                    origen.x = 272; origen.y = 208;
                    break;
                case 'A':
                case 'a':
                    if (game->nivel_actual == 3){
                        origen.x = 416; origen.y = 144;
                    } else {
                        origen.x = 384; origen.y = 112;
                    }
                    break;
                case 'S':
                case 's':
                    origen.x = 112; origen.y = 32;  
                    break;
                case 'B':
                    origen.x = 384; origen.y = 112;
                    break;
                case 'M':
                    origen.x = 384; origen.y = 208;
                    break;
                case 'N':
                    origen.x = 384; origen.y = 176;
                    break;
                case 'L':
                    origen.x = 272; origen.y = 240;
                    break;
                case 'l':
                    origen.x = 304;; origen.y = 240;
                    break;
                case 'T':
                    if (game->nivel_actual == 2) {
                        origen.x = 336;
                        origen.y = 240;
                    } else {
                        origen.x = 336; origen.y = 208;
                    }
                    break;
                default: // si no es ninguno el booleano se cambia
                    dibuja_pista = false;
                    break;
            }
            
            if (dibuja_pista && game->texturaPista != NULL)
            {
                SDL_RenderCopy(game->pantalla.renderer, game->texturaPista, &origen, &destino);    
            }
        }
    }
    
    // RECTANGULO (interactivo objeto2 txt) destruible!!!!!!
    SDL_SetRenderDrawBlendMode(game->pantalla.renderer, SDL_BLENDMODE_BLEND);
    for (int i=0; i<tile_filas; i++)
    {
        for (int j=0; j<tile_cols; j++)
        {
            // caja
            if (game->tiles[i][j].objeto2)
            {
                SDL_Rect Rectang_Obj2 = {
                    game->tiles[i][j].x_tiles - game->pantalla.camara.x,
                    game->tiles[i][j].y_tiles - game->pantalla.camara.y,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };

                if (game->texturaCaja != NULL)
                {
                    SDL_RenderCopy(game->pantalla.renderer, game->texturaCaja, NULL, &Rectang_Obj2); 
                }
                else
                {
                    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 0, 0, 105);
                    SDL_RenderFillRect(game->pantalla.renderer, &Rectang_Obj2);
                }
            }
            
            if (game->tiles[i][j].objeto3)
            {
                SDL_Rect Rectang_Obj3 = {
                    game->tiles[i][j].x_tiles - game->pantalla.camara.x,
                    game->tiles[i][j].y_tiles - game->pantalla.camara.y,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };
                if (game->texturaCajaBalas != NULL)
                {
                    SDL_RenderCopy(game->pantalla.renderer, game->texturaCajaBalas, NULL, &Rectang_Obj3); 
                } 
                else
                {
                    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 0, 0, 105);
                    SDL_RenderFillRect(game->pantalla.renderer, &Rectang_Obj3);
                }
            }
            
            if (game->tiles[i][j].objetominimapa)
            {
                SDL_Rect Rectang_W = {
                    game->tiles[i][j].x_tiles - game->pantalla.camara.x,
                    game->tiles[i][j].y_tiles - game->pantalla.camara.y,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };

                if (game->texturaMinimapa != NULL)
                {
                    SDL_RenderCopy(game->pantalla.renderer, game->texturaMinimapa, NULL, &Rectang_W);
                }
                else
                {
                    SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 255, 255, 200);
                    SDL_RenderFillRect(game->pantalla.renderer, &Rectang_W);
                }
            }

            if (game->tiles[i][j].meta)
            {
                SDL_Rect Rectang_Meta = {
                    game->tiles[i][j].x_tiles - game->pantalla.camara.x,
                    game->tiles[i][j].y_tiles - game->pantalla.camara.y,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };
                
                if (game->texturaMeta != NULL)
                {
                    SDL_RenderCopy(game->pantalla.renderer, game->texturaMeta, NULL, &Rectang_Meta);
                } 
                else
                {
                    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 0, 0, 105);
                    SDL_RenderFillRect(game->pantalla.renderer, &Rectang_Meta);
                }
            }

            if (game->tiles[i][j].aceite)
            {
                SDL_Rect Rectang_Aceite = {
                    game->tiles[i][j].x_tiles - game->pantalla.camara.x,
                    game->tiles[i][j].y_tiles - game->pantalla.camara.y,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };

                if (game->texturaAceite != NULL)
                {
                    SDL_RenderCopy(game->pantalla.renderer, game->texturaAceite, NULL, &Rectang_Aceite);
                } 
                else
                {
                    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 0, 0, 105);
                    SDL_RenderFillRect(game->pantalla.renderer, &Rectang_Aceite);
                }
            }
            
            // objeto direccion
            if (game->tiles[i][j].direccion == 1 || game->tiles[i][j].direccion == -1) 
            {
                SDL_Rect Rectang_Direccion = {
                    game->tiles[i][j].x_tiles - game->pantalla.camara.x,
                    game->tiles[i][j].y_tiles - game->pantalla.camara.y,
                    game->tiles[i][j].w_tiles,
                    game->tiles[i][j].h_tiles
                };
                
                if (game->tiles[i][j].direccion == 1) angulo_dir = 180.0f;
                
                if (game->texturaDireccion != NULL)
                {
                    SDL_RenderCopyEx(game->pantalla.renderer, game->texturaDireccion, NULL, &Rectang_Direccion, angulo_dir, NULL, SDL_FLIP_NONE);
                } 
                else
                {
                    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 0, 0, 105);
                    SDL_RenderFillRect(game->pantalla.renderer, &Rectang_Direccion);
                }
            }
        }
    }
    
    // CICLO QUE DIBUJA ENEMIGOS
    for (int i = 0; i < max_enemigos; i++)
    {
        if (game->enemigos[i].activo)
        {
            SDL_Rect rect_enm = {
                (int)game->enemigos[i].x - game->pantalla.camara.x,
                (int)game->enemigos[i].y - game->pantalla.camara.y,
                game->enemigos[i].lado,
                game->enemigos[i].lado
            };

            // si no entra al if devuelve patrulla (default)
            SDL_Texture *textura_enemigo = game->texturaEnemigo;
            if (game->enemigos[i].es_camion == true){
                textura_enemigo = game->texturaEnemigo3;
            } else if (game->enemigos[i].es_bote == true) {
                textura_enemigo = game->texturaEnemigo4;
            } else if (game->enemigos[i].es_torreta == true) {
                textura_enemigo = game->texturaEnemigo5;
            }

            if (textura_enemigo != NULL)
            {
                SDL_RenderCopyEx(game->pantalla.renderer, textura_enemigo, NULL, &rect_enm, game->enemigos[i].angulo, NULL, SDL_FLIP_NONE);
            } 
            else
            {
                SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 50, 50, 255);
                SDL_RenderFillRect(game->pantalla.renderer, &rect_enm);
            }
            
            if (!game->enemigos[i].es_bote && !game->enemigos[i].es_camion && !game->enemigos[i].es_torreta && game->enemigos[i].indicador)
            {
                if (game->indicadorEnemigo != NULL)
                {
                    SDL_SetTextureBlendMode(game->indicadorEnemigo, SDL_BLENDMODE_BLEND);
                    SDL_SetTextureAlphaMod(game->indicadorEnemigo, 100);
                    SDL_RenderCopyEx(game->pantalla.renderer, game->indicadorEnemigo, NULL, &rect_enm, game->enemigos[i].angulo, NULL, SDL_FLIP_NONE);
                }
            }        
        }
    }
    
    // DIBUJA PROYECTILES/BALAS DEL ENEMIGO
    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 255, 0, 255);
    for (int i=0; i<max_enemigos; i++)
    {
        for (int p=0; p<MAX_PROYECTILES; p++)
        {
            if (game->enemigos[i].proyectiles[p].activo)
            {
                SDL_Rect rect_bala = {
                    (int)(game->enemigos[i].proyectiles[p].x) - game->pantalla.camara.x,
                    (int)(game->enemigos[i].proyectiles[p].y) - game->pantalla.camara.y,
                    game->enemigos[i].proyectiles[p].lado,
                    game->enemigos[i].proyectiles[p].lado
                };
                SDL_RenderFillRect(game->pantalla.renderer, &rect_bala);
            }
        } 
    }

    // DIBUJA PROYECTILES/BALAS DEL JUGADOR
    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 255, 0, 255);
    for (int p=0; p<MAX_PROYECTILES; p++)
    {
        if (game->jugador.proyectiles[p].activo)
        {
            SDL_Rect rect_bala_jug = {
                (int)(game->jugador.proyectiles[p].x) - game->pantalla.camara.x,
                (int)(game->jugador.proyectiles[p].y) - game->pantalla.camara.y,
                game->jugador.proyectiles[p].lado,
                game->jugador.proyectiles[p].lado
            };
            SDL_RenderFillRect(game->pantalla.renderer, &rect_bala_jug);
        }
    }

    // RECTANGULO DEL JUGADOR !!! acaaa
    SDL_Rect Rectang = { 
        (int)game->jugador.x - game->pantalla.camara.x, 
        (int)game->jugador.y - game->pantalla.camara.y, 
        game->jugador.lado, 
        game->jugador.lado 
    };
    SDL_RenderCopyEx(game->pantalla.renderer, game->jugador.textura, NULL, &Rectang, game->jugador.angulo, NULL, SDL_FLIP_NONE);
   
    // balas del jugador
    for(int p=0; p<MAX_PROYECTILES; p++)
    {
        if(game->jugador.proyectiles[p].activo)
        {
            SDL_Rect rect_bala = {
                (int)(game->jugador.proyectiles[p].x) - game->pantalla.camara.x,
                (int)(game->jugador.proyectiles[p].y) - game->pantalla.camara.y,
                game->jugador.proyectiles[p].lado,
                game->jugador.proyectiles[p].lado
            };
        }
    }

    // EXPLOSIONES
    for (int i=0; i<MAX_EXPLOSIONES; i++)
    {
        if (!game->explosiones[i].activa) continue;

        // numero de fotograma que toca mostrar por tiempo
        int frame = (int)((game->explosiones[i].tiempo / DURACION_EXPLOSION) * FRAMES_EXPLOSION);
        if (frame >= FRAMES_EXPLOSION)
        {
            frame = FRAMES_EXPLOSION - 1;
        }
        
        SDL_Rect rect_explosion = {
            (int)game->explosiones[i].x - game->pantalla.camara.x - (tam),
            (int)game->explosiones[i].y - game->pantalla.camara.y - (tam),
            TAM_EXPLOSION,
            TAM_EXPLOSION,
        };

        if (game->texturaExplosion[frame] != NULL)
        {
            SDL_RenderCopy(game->pantalla.renderer, game->texturaExplosion[frame], NULL, &rect_explosion);
        }
    }

    /*          --- TEXTOS/INTERFAZ ----      */
    // texto que dice teclas awsd
    SDL_Color colorTexto = { 0, 0, 0, 255 }; //Blanco y 255 de opacidad
    
    SDL_Surface *surfaceTexto1 = TTF_RenderText_Solid(game->fuente, "Usa AWSD, Flechas o el pad del mando para moverte. H para bocina y J para disparar", colorTexto);
    if (surfaceTexto1 != NULL)
    {
        game->interfaz.texturaTexto = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTexto1);
        SDL_FreeSurface(surfaceTexto1);
    }
    SDL_QueryTexture(game->interfaz.texturaTexto, NULL, NULL, &wText, &hText);
    SDL_Rect textoRec = { 0, 0, wText/4, hText/4 }; // posicion texto, ancho y alto
    SDL_RenderCopy(game->pantalla.renderer, game->interfaz.texturaTexto, NULL, &textoRec);
    
    // CRONOMETRO
    game->ranking.tiempo = transcurrido / 1000;
    snprintf(game->interfaz.texto_cronometro, sizeof(game->interfaz.texto_cronometro), "Tiempo: %02d:%02d:%02d", minutos, segundos, centesimas);
    // HP
    sprintf(game->interfaz.texto_HP, "HP: %02d", game->jugador.hp);
    // BALAS
    sprintf(game->interfaz.texto_Balas, "Balas restantes: %02d", game->jugador.contador_balas);
    // NIVEL
    sprintf(game->interfaz.texto_Nivel, "Nivel: %01d", game->nivel_actual);
    // camiones
    snprintf(game->interfaz.texto_Camiones, sizeof(game->interfaz.texto_Camiones), "Camiones: %d", game->contador_camiones);
    
    // renderiza interfaz
    render_Cronometro(game);
    render_Minimapa(game);
    render_hp(game);
    render_contbalas(game);
    render_nivel(game);

    // VELOCIMETRO (PASAR A FUNCION)
    // FALTA DESTRUIR LA TEXTURA Y PASARLA A GLOBAL
    char texto_Velocidad[32];
    SDL_Color colorBlanco = {255,255,255,255};
    snprintf(texto_Velocidad, sizeof(texto_Velocidad), "Velocidad: %.2f", game->jugador.velocidad_actual);
    SDL_Surface *s_velocidad = TTF_RenderText_Solid(game->fuente, texto_Velocidad, colorBlanco);
    game->interfaz.t_velocidad = SDL_CreateTextureFromSurface(game->pantalla.renderer, s_velocidad);
    SDL_FreeSurface(s_velocidad);

    SDL_QueryTexture(game->interfaz.t_velocidad, NULL, NULL, &wText, &hText);
    SDL_Rect textoRec3 = { 0, game->pantalla.win_h - 50, (int)wText/2, (int)hText/2 };
    SDL_RenderCopy(game->pantalla.renderer, game->interfaz.t_velocidad, NULL, &textoRec3);

    // ILUMINACION (NIVEL 3)
    if (game->nivel_actual == 3 && game->texturaVignette != NULL)
    {
        SDL_Rect destino_vignette = { 0,0, game->pantalla.win_w, game->pantalla.win_h };
        SDL_RenderCopy(game->pantalla.renderer, game->texturaVignette, NULL, &destino_vignette);
    }

    SDL_RenderPresent(game->pantalla.renderer);
}

void render_Cronometro(Game *game)
{
    // PREVIENE FUGAS DE MEMORIA (ya paso :v)
    if (game->interfaz.texturaTexto2 != NULL) 
    {
        SDL_DestroyTexture(game->interfaz.texturaTexto2);
        game->interfaz.texturaTexto2 = NULL;
    }
    
    SDL_Color colorBlanco = {255,255,255,200}; //rgb y transparencia
    SDL_Surface *surfaceTemporal = TTF_RenderText_Solid(game->fuente, game->interfaz.texto_cronometro, colorBlanco);
    
    if (surfaceTemporal != NULL)
    {
        game->interfaz.texturaTexto2 = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTemporal);
        SDL_FreeSurface(surfaceTemporal);
    }
    
    if (game->interfaz.texturaTexto2 != NULL)
    {
        SDL_QueryTexture(game->interfaz.texturaTexto2, NULL, NULL, &wText, &hText);
        SDL_Rect textoRec2 = { game->pantalla.win_w - wText/2, game->pantalla.win_h - 30, (int)wText/2, (int)hText/2 };
        SDL_RenderCopy(game->pantalla.renderer, game->interfaz.texturaTexto2, NULL, &textoRec2);
    }
}

void render_hp(Game *game)
{
    if (game->interfaz.texturaHP != NULL)
    {
        SDL_DestroyTexture(game->interfaz.texturaHP);
        game->interfaz.texturaHP = NULL;
    }

    SDL_Color colorBlanco = {255,255,255,255}; //rgb y transparencia
    SDL_Surface *surfaceTemporal = TTF_RenderText_Solid(game->fuente, game->interfaz.texto_HP, colorBlanco);

    if (surfaceTemporal != NULL)
    {
        game->interfaz.texturaHP = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTemporal);
        SDL_FreeSurface(surfaceTemporal);
    }

    if (game->interfaz.texturaHP != NULL)
    {
        SDL_QueryTexture(game->interfaz.texturaHP, NULL, NULL, &wText, &hText);
        SDL_Rect textoRec2 = { 0, game->pantalla.win_h - hText, wText/2, hText/2 };
        SDL_RenderCopy(game->pantalla.renderer, game->interfaz.texturaHP, NULL, &textoRec2);
    }
}

void render_contbalas(Game *game)
{
    if (game->interfaz.texturaBalas != NULL)
    {
        SDL_DestroyTexture(game->interfaz.texturaBalas);
        game->interfaz.texturaBalas = NULL;
    }
    
    SDL_Color colorBlanco = {255,255,255,255}; //rgb y transparencia
    SDL_Surface *surfaceTemporal = TTF_RenderText_Solid(game->fuente, game->interfaz.texto_Balas, colorBlanco);
    
    if (surfaceTemporal != NULL)
    {
        game->interfaz.texturaBalas = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTemporal);
        SDL_FreeSurface(surfaceTemporal);
    }
    
    if (game->interfaz.texturaBalas != NULL)
    {
        SDL_QueryTexture(game->interfaz.texturaBalas, NULL, NULL, &wText, &hText);
        SDL_Rect textoRec = { 0, game->pantalla.win_h - 2*hText, wText/2, hText/2 };
        SDL_RenderCopy(game->pantalla.renderer, game->interfaz.texturaBalas, NULL, &textoRec);
    }
}

void render_nivel(Game *game)
{
    // reseteo
    if (game->interfaz.texturaNivel != NULL)
    {
        SDL_DestroyTexture(game->interfaz.texturaNivel);
        game->interfaz.texturaNivel = NULL;
    }

    if (game->interfaz.texturaTexto3 != NULL)
    {
        SDL_DestroyTexture(game->interfaz.texturaTexto3);
        game->interfaz.texturaTexto3 = NULL;
    }

    if (game->interfaz.texturaCamiones != NULL)
    {
        SDL_DestroyTexture(game->interfaz.texturaCamiones);
        game->interfaz.texturaCamiones = NULL;
    }
    
    SDL_Color colorBlanco = {255,255,255,255};
    SDL_Surface *surfaceTemporal = TTF_RenderText_Solid(game->fuente, game->interfaz.texto_Nivel, colorBlanco);
    SDL_Surface *surfaceTexto3 = TTF_RenderText_Solid(game->fuente, game->interfaz.texto_Mision, colorBlanco);
    SDL_Surface *surfaceTextoCamiones = TTF_RenderText_Solid(game->fuente, game->interfaz.texto_Camiones, colorBlanco);

    if (surfaceTemporal != NULL)
    {
        game->interfaz.texturaNivel = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTemporal);
        SDL_FreeSurface(surfaceTemporal);
    }

    if (surfaceTexto3 != NULL)
    {
        game->interfaz.texturaTexto3 = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTexto3);
        SDL_FreeSurface(surfaceTexto3);
    }

    if (surfaceTextoCamiones != NULL)
    {
        game->interfaz.texturaCamiones = SDL_CreateTextureFromSurface(game->pantalla.renderer, surfaceTextoCamiones);
        SDL_FreeSurface(surfaceTextoCamiones);
    }
    
    if (game->interfaz.texturaNivel != NULL)
    {
        SDL_QueryTexture(game->interfaz.texturaNivel, NULL, NULL, &wText, &hText);
        SDL_Rect textoRec = { game->pantalla.win_w - (wText/2) - 10, 10, wText/2, hText/2 };
        SDL_RenderCopy(game->pantalla.renderer, game->interfaz.texturaNivel, NULL, &textoRec);
    }

    if (game->interfaz.texturaCamiones != NULL && game->nivel_actual != 1)
    {
        SDL_QueryTexture(game->interfaz.texturaCamiones, NULL, NULL, &wText, &hText);
        SDL_Rect textoC = { game->pantalla.win_w - (wText/2) - 10, 40, wText/2, hText/2 };
        SDL_RenderCopy(game->pantalla.renderer, game->interfaz.texturaCamiones, NULL, &textoC);
    }

    if (game->interfaz.texturaTexto3 != NULL)
    {
        SDL_QueryTexture(game->interfaz.texturaTexto3, NULL, NULL, &wText, &hText);

        // cuadradito gris
        SDL_SetRenderDrawBlendMode(game->pantalla.renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 0, 0, 150);
        SDL_Rect fondo_mision = {
            5,
            25,
            (wText/4)+10,
            (hText/4)+6
        };
        SDL_RenderFillRect(game->pantalla.renderer, &fondo_mision);

        SDL_Rect textoRec2 = { 5, 25, (wText/4), (hText/4) };
        SDL_RenderCopy(game->pantalla.renderer, game->interfaz.texturaTexto3, NULL, &textoRec2);
    }
}

void render_Minimapa(Game *game)
{
    if (!game->jugador.minimapa_activo) return;
    
    int escala = 2; // 2x2 px
    int ancho_mapa = tile_cols * escala;
    int alto_mapa = tile_filas * escala;

    int mapa_x = game->pantalla.win_w - ancho_mapa - 20;
    int mapa_y = 60;

    // caja gris transparente
    SDL_SetRenderDrawBlendMode(game->pantalla.renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 0, 0, 160);
    SDL_Rect fondo_minimapa = {
        mapa_x - 5,
        mapa_y - 5,
        ancho_mapa + 10,
        alto_mapa + 10
    };
    SDL_RenderFillRect(game->pantalla.renderer, &fondo_minimapa);

    // renderizado mapa
    for (int i=0; i<tile_filas; i++)
    {
        for (int j=0; j<tile_cols; j++)
        {
            char t = game->tiles[i][j].tipo;
            SDL_Rect pixel = {
                mapa_x + (j*escala),
                mapa_y + (i*escala),
                escala,
                escala
            };
            // solo si es cemento o curva
            if (t == '1' || t == '2' || t == '3' || t == '4' || t == '5' || t == '6' || t == '7' || t == '8' || t == '-' || t == '+' || t == '=' || t == '|' || t == 'I' || t == 'i')
            {
                SDL_SetRenderDrawColor(game->pantalla.renderer, 150, 150, 150, 255);
                SDL_RenderFillRect(game->pantalla.renderer, &pixel);
            }
            else if (t == 'F')
            {
                SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 255, 0, 255);
                SDL_RenderFillRect(game->pantalla.renderer, &pixel);
            }
        }
    }

    // dibujar rectnagulo jugador en el mapa
    int x = (int)(game->jugador.x / tam);
    int y = (int)(game->jugador.y / tam);

    SDL_SetRenderDrawColor(game->pantalla.renderer, 0, 255, 0, 255);
    SDL_Rect pixel_jugador = {
        mapa_x + (x*escala) - 1,
        mapa_y + (y*escala) - 1,
        escala * 2,
        escala * 2
    };
    SDL_RenderFillRect(game->pantalla.renderer, &pixel_jugador);

    // dibuja camiones de gas en el mapa
    SDL_SetRenderDrawColor(game->pantalla.renderer, 255, 0, 0, 255);
    for (int i=0; i<max_enemigos; i++)
    {
        if (game->enemigos[i].activo && game->enemigos[i].es_camion)
        {
            int ex = (int)(game->enemigos[i].x / tam);
            int ey = (int)(game->enemigos[i].y / tam);

            SDL_Rect pixel_camion = {
                mapa_x + (ex*escala) - 1,
                mapa_y + (ey*escala) - 1,
                2 * escala,
                2 * escala
            };
            SDL_RenderFillRect(game->pantalla.renderer, &pixel_camion);
        }
    }
}