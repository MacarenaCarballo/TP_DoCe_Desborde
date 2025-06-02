#include "comun.h"

#define CANT_REPARTO 3

signed char modoFACIL(tEstadoJuego* estadoDeJuego)
{
    if (!estadoDeJuego || estadoDeJuego->cartas->cantElem == 0)
        return VACIO;


    unsigned pos = rand() % estadoDeJuego->cartas->cantElem;

    signed char carta;
    if (elimPorPosVec(estadoDeJuego->cartas, pos, &carta) != REALIZADO)
        return VACIO;

    return carta;
}

signed char modoMEDIO(tEstadoJuego* estadoDeJuego)
{
    signed char cartaMaq;
    int i;

    if(*(estadoDeJuego->puntajeActual) > CERCA_GANAR)
    {
        for(i=0; i<estadoDeJuego->cartas->cantElem; i++)
        {
            if(verCartaPorPos(estadoDeJuego->cartas,i,&cartaMaq)==REALIZADO)
            {

                if((cartaMaq==MAS_UNO)||(cartaMaq==MAS_DOS))
                {
                    if (elimPorPosVec(estadoDeJuego->cartas, i, &cartaMaq) != REALIZADO)
                        return VACIO;

                    return cartaMaq;
                }

            }
        }
    }
    else if (*(estadoDeJuego->puntajeOponente) ==0)
    {
        for(i=0; i<estadoDeJuego->cartas->cantElem; i++)
        {
            if(verCartaPorPos(estadoDeJuego->cartas,i,&cartaMaq)==REALIZADO)
            {
                if((cartaMaq!=SACAR_UNO)&&(cartaMaq!=SACAR_DOS))
                {
                    if (elimPorPosVec(estadoDeJuego->cartas, i, &cartaMaq) != REALIZADO)
                        return VACIO;

                    return cartaMaq;
                }
            }
        }
    }

    return modoFACIL(estadoDeJuego);
}

signed char modoDIFICIL( tEstadoJuego* estadoDeJuego)
{
    int i,
        posRepetir,
        cartasBuenas=0,
        hayRepetir =0;
    signed char cartaMaq;

    for(i=0; i<estadoDeJuego->cartas->cantElem; i++)
    {
        if(verCartaPorPos(estadoDeJuego->cartas,i,&cartaMaq)==REALIZADO)
        {

            if((*(estadoDeJuego->ultimaCartaOponente)==SACAR_UNO)||(*(estadoDeJuego->ultimaCartaOponente)==SACAR_DOS))
                if(cartaMaq==ESPEJO)
                {
                    if (elimPorPosVec(estadoDeJuego->cartas, i, &cartaMaq) != REALIZADO)
                        return VACIO;

                    return cartaMaq;
                }

            if(*(estadoDeJuego->puntajeOponente) > CERCA_GANAR)
            {
                if((cartaMaq==SACAR_UNO)||(cartaMaq==SACAR_DOS))
                {
                    if (elimPorPosVec(estadoDeJuego->cartas, i, &cartaMaq) != REALIZADO)
                        return VACIO;

                    return cartaMaq;
                }
            }

            if((cartaMaq==MAS_UNO)||(cartaMaq==MAS_DOS))
                cartasBuenas++;
            //guardo posicion de la carta repetir turno
            if(cartaMaq==REPETIR_TURNO)
            {
                hayRepetir=1;
                posRepetir=i;
            }
        }
    }

    if((cartasBuenas>=2)&& hayRepetir)
    {
        if(verCartaPorPos(estadoDeJuego->cartas,posRepetir,&cartaMaq)==REALIZADO)
        {
            if (elimPorPosVec(estadoDeJuego->cartas, posRepetir, &cartaMaq) != REALIZADO)
                return VACIO;

            return cartaMaq;
        }
    }

    return modoMEDIO(estadoDeJuego);
}


int reponerCarta(tPila* mazoJuego, tVectorCartas* mazoDescarte, tVectorCartas* mano)
{
    signed char nuevaCarta;

    // Intentar sacar una carta del mazo

    if(desapilar(mazoJuego, &nuevaCarta, sizeof(nuevaCarta)) == REALIZADO)
        if(insVecAlFinal(mano, nuevaCarta)!=REALIZADO)
            return SIN_MEM;


    if (mano->cantElem < CANT_REPARTO)
    {
        // Mezclar descarte y reponer el mazo
        mezclarVector(mazoDescarte);

        while (sacarUltVec(mazoDescarte, &nuevaCarta) == REALIZADO)
            apilar(mazoJuego, &nuevaCarta, sizeof(nuevaCarta));


        // Intentar de nuevo
        if (desapilar(mazoJuego, &nuevaCarta, sizeof(nuevaCarta)) == VACIO)
        {
            puts("No se pudo recargar el mazo. Juego terminado.");
            return VACIO;
        }

        insVecAlFinal(mano, nuevaCarta);
    }

    // Agregar la nueva carta a la mano del jugador
    return REALIZADO;
}
signed char aplicarEfecto(signed char efecto,tEstadoJuego *estado,tCola *informeJuego)
{
    char mensaje[150];
    char detalleTurno[300];
    signed char resultado = REALIZADO;

    printf("\n\n");

    switch(efecto)
    {
    case SACAR_DOS:
    case SACAR_UNO:
        *(estado->puntajeOponente) += efecto;

        if (*(estado->puntajeOponente) < 0)
            *(estado->puntajeOponente) = 0;

        snprintf(mensaje, sizeof(mensaje),
                 "%s jugo carta %s. %s pierde %d punto%s.",
                 estado->nombreActual,
                 (efecto == SACAR_UNO) ? "SACAR_UNO" : "SACAR_DOS",
                 estado->nombreOponente,
                 -efecto,
                 (efecto == SACAR_UNO) ? "" : "s");

        break;

    case MAS_UNO:
    case MAS_DOS:
        *(estado->puntajeActual) += efecto;

        snprintf(mensaje, sizeof(mensaje),
                 "%s jugo carta %s. Gana %d punto%s!",
                 estado->nombreActual,
                 (efecto == MAS_UNO) ? "MAS_UNO" : "MAS_DOS",
                 efecto,
                 (efecto == MAS_UNO) ? "" : "s");

        if (*(estado->puntajeActual) >= 12)
            resultado = *(estado->esHumano) ? GANO_HUMANO : GANO_MAQUINA;
        break;

    case REPETIR_TURNO:
        snprintf(mensaje, sizeof(mensaje),
                 "%s jugo carta REPETIR_TURNO. Juega otra vez!",
                 estado->nombreActual);

        resultado = REPETIR_TURNO;
        break;

    case ESPEJO:
        if (*(estado->ultimaCartaOponente) == SACAR_UNO || *(estado->ultimaCartaOponente) == SACAR_DOS)
        {
            *(estado->puntajeActual) -= *(estado->ultimaCartaOponente);
            *(estado->puntajeOponente) += *(estado->ultimaCartaOponente);

            if (*(estado->puntajeOponente) < 0)
                *(estado->puntajeOponente) = 0;

            snprintf(mensaje, sizeof(mensaje),
                     "%s jugo ESPEJO. Devuelve el efecto de %d punto%s a %s!",
                     estado->nombreActual,
                     *(estado->ultimaCartaOponente),
                     (*(estado->ultimaCartaOponente) == SACAR_UNO) ? "" : "s",
                     estado->nombreOponente);
        }
        else
        {
            snprintf(mensaje, sizeof(mensaje),
                     "%s jugo ESPEJO, pero no habia efecto para devolver.",
                     estado->nombreActual);
        }
        break;
    }

    snprintf(detalleTurno, sizeof(detalleTurno),
             "[Turno %d] %s\n  Puntos %s: %d\n  Puntos %s: %d\n",
             *(estado->nroDeTurno),
             mensaje,
             estado->nombreActual, *(estado->puntajeActual),
             estado->nombreOponente, *(estado->puntajeOponente));

    puts(mensaje);
    printf("\n\n");

    system("pause");

    ponerEnCola(informeJuego, detalleTurno, strlen(detalleTurno) + 1);

    return resultado;
}
int ejecutarTurno(tVectorCartas *mazoDescarte,
                  tPila *mazoJuego,
                  tCola *informe,
                  tFuncionElegirCarta elegirCarta,
                  tEstadoJuego *estadoJuego)
{
    signed char carta;
    signed char estado;

    // Elegir carta (según IA o humano)
    carta = elegirCarta(estadoJuego);

    // Aplicar efecto de la carta, pasando solo la estructura, la carta y la cola
    estado = aplicarEfecto(carta, estadoJuego, informe);

    // Guardar carta en mazo de descarte
    insVecAlFinal(mazoDescarte, carta);

    *(estadoJuego->ultimaCartaOponente) = carta;
    // Robar una nueva carta
    if (reponerCarta(mazoJuego, mazoDescarte, estadoJuego->cartas) != REALIZADO)
        return VACIO;

    return estado;
}


void mostrarCartasEnLinea(const tVectorCartas *cartas)
{
    const char *nombres[] =
    {
        "SACAR 2",  // -2
        "SACAR 1",  // -1
        "MAS UNO",  //  1
        "MAS DOS",  //  2
        "REPETIR",  //  3
        "ESPEJO "   //  4
    };

    int i;
    int cant = cartas->cantElem;
    signed char carta;
    const char *nombre;

    // Línea superior
    for (i = 0; i < cant; i++) printf("+---------+   ");
    printf("\n");

    // Línea vacía
    for (i = 0; i < cant; i++) printf("|         |   ");
    printf("\n");

    // Línea con valor numérico
    for (i = 0; i < cant; i++)
    {
        carta = (signed char)(cartas->datos[i]);
        printf("|   % 3d   |   ", carta);
    }
    printf("\n");

    // Línea con nombre de la carta
    for (i = 0; i < cant; i++)
    {
        carta = (signed char)(cartas->datos[i]);

        switch (carta)
        {
        case -2:
            nombre = nombres[0];
            break;
        case -1:
            nombre = nombres[1];
            break;
        case  1:
            nombre = nombres[2];
            break;
        case  2:
            nombre = nombres[3];
            break;
        case  3:
            nombre = nombres[4];
            break;
        case  4:
            nombre = nombres[5];
            break;
        default:
            nombre = "???????";
            break;
        }

        printf("| %-7s |   ", nombre);
    }
    printf("\n");

    // Línea inferior
    for (i = 0; i < cant; i++) printf("+---------+   ");
    printf("\n");

    // Índices debajo
    for (i = 0; i < cant; i++) printf("    [%d]     ", i + 1);
    printf("\n");
}

void mostrarTablero(unsigned char puntosHum, unsigned char puntosMaq, const tVectorCartas *cartasHum)
{
    puts("\n=========================================");
    printf(" PUNTAJES -> Humano: %2d | Maquina: %2d\n", puntosHum, puntosMaq);
    puts("=========================================");
    puts(" Tus cartas:\n");

    mostrarCartasEnLinea(cartasHum);

    puts("=========================================\n");
}

int repartirCartas(tPila *mazo, tVectorCartas *jugHum, tVectorCartas *jugMaq)
{
    signed char carta,
           i;

    for (i = 0; i < CANT_REPARTO; i++)
    {
        // Jugador humano
        if (desapilar(mazo, &carta, sizeof(signed char))!=REALIZADO)
            return VACIO;

        if (insVecAlFinal(jugHum, carta)!= REALIZADO)
            return SIN_MEM;

        // Máquina
        if (desapilar(mazo, &carta, sizeof(signed char))!=REALIZADO)
            return VACIO;

        if (insVecAlFinal(jugMaq, carta)!=REALIZADO)
            return SIN_MEM;
    }

    return REALIZADO;
}


signed char elegirCartaHumano(tEstadoJuego* estado)
{
    unsigned char opcion;
    int r;
    signed char carta;

    do
    {
        printf("Elegi una carta (1 a 3): ");
        r = scanf("%hhu", &opcion);  // %hhu para unsigned char
        while(getchar() != '\n');

        if (r != 1 || opcion < 1 || opcion > 3)
            printf("Entrada invalida. Intente nuevamente.\n");

    }
    while (r != 1 || opcion < 1 || opcion > 3);



    // Eliminar carta de la posición (opcion - 1)

    if (elimPorPosVec(estado->cartas, opcion - 1, &carta) != REALIZADO)
    {
        printf("Error eliminando la carta del vector.\n");

        return VACIO;
    }

    return carta;
}

int generarInforme(tCola *informe, unsigned char ganador, const char *nombreJugador)
{
    char nombreArchivo[200];
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    sprintf(nombreArchivo,
            "informe-juego_%04d-%02d-%02d-%02d-%02d.txt",
            tm_info->tm_year + 1900,
            tm_info->tm_mon + 1,
            tm_info->tm_mday,
            tm_info->tm_hour,
            tm_info->tm_min
           );

    FILE* archivo=fopen(nombreArchivo,"wt");
    if(!archivo)
    {
        printf("ERROR EN LA APERTURA DEL ARCHIVO INFORME\n");
        return ERROR_ARCH;
    }

    fprintf(archivo, "===== INFORME DE PARTIDA =====\n\n");

    if (ganador == GANO_HUMANO)
        fprintf(archivo, "Ganador: %s\n\n", nombreJugador);
    else if (ganador == GANO_MAQUINA)
        fprintf(archivo, "Ganador: Maquina\n\n");
    else
        fprintf(archivo, "Ganador: Desconocido\n\n");

    fprintf(archivo, "Detalle de turnos:\n\n");

    char mensaje[300];
    while (!colaVacia(informe))
    {
        sacarDeCola(informe, mensaje, sizeof(mensaje));
        fprintf(archivo, "%s\n", mensaje);
    }

    fclose(archivo);
    return REALIZADO;
}
unsigned char jugar(const char *nombre, tFuncionElegirCarta dificultadMaq)
{
    tPila mazoJuego;
    tVectorCartas cartasHum, cartasMaq, mazoDescarte;
    tCola informeJuego;

    unsigned char nroDeTurno = 1;
    signed char puntajeHum = 0, puntajeMaq = 0;
    signed char efectoPendHum = 0, efectoPendMaq = 0;
    unsigned char estado;
    tApi config;

    int esHumano = rand() % 2;  // 0 = maquina  1 = humano

    tEstadoJuego estadoDeJuego;
    tFuncionElegirCarta elegirCarta;

    // Inicializo segun quien va a empezar
    if (esHumano)
    {
        estadoDeJuego.puntajeActual = &puntajeHum;
        estadoDeJuego.puntajeOponente = &puntajeMaq;
        estadoDeJuego.ultimaCartaOponente = &efectoPendMaq;
        estadoDeJuego.cartas = &cartasHum;
        estadoDeJuego.nombreActual = nombre;
        estadoDeJuego.nombreOponente = "Maquina";
        elegirCarta = elegirCartaHumano;
    }
    else
    {
        estadoDeJuego.puntajeActual = &puntajeMaq;
        estadoDeJuego.puntajeOponente = &puntajeHum;
        estadoDeJuego.ultimaCartaOponente = &efectoPendHum;
        estadoDeJuego.cartas = &cartasMaq;
        estadoDeJuego.nombreActual = "Maquina";
        estadoDeJuego.nombreOponente = nombre;
        elegirCarta = dificultadMaq;
    }

    estadoDeJuego.esHumano = &esHumano;
    estadoDeJuego.nroDeTurno = &nroDeTurno;


    // Inicializaciones de estructuras auxiliares
    crearPila(&mazoJuego);
    crearVector(&cartasHum);
    crearVector(&cartasMaq);
    crearVector(&mazoDescarte);
    crearCola(&informeJuego);

    if (generarMazo(&mazoJuego) != REALIZADO)
    {
        puts("No se pudo generar el mazo de juego");
        return SIN_MEM;
    }

    repartirCartas(&mazoJuego, &cartasHum, &cartasMaq);

    printf("Empieza: %s\n\n", estadoDeJuego.nombreActual);
    system("pause");

    while (puntajeHum < 12 && puntajeMaq < 12)
    {
        do
        {
            system("cls");
            mostrarTablero(puntajeHum, puntajeMaq, &cartasHum);

            printf("Turno de %s:\n", estadoDeJuego.nombreActual);

            // Ejecutar el turno
            estado = ejecutarTurno(&mazoDescarte,
                                   &mazoJuego,
                                   &informeJuego,
                                   elegirCarta,
                                   &estadoDeJuego);

            if (estado == GANO_HUMANO || estado == GANO_MAQUINA)
                break;

            (*estadoDeJuego.nroDeTurno)++;

        }
        while (estado == REPETIR_TURNO);

        if (*(estadoDeJuego.ultimaCartaOponente) != ESPEJO)
        {
            if (esHumano)
                efectoPendHum = *(estadoDeJuego.ultimaCartaOponente);
            else
                efectoPendMaq = *(estadoDeJuego.ultimaCartaOponente);
        }

        mostrarTablero(puntajeHum, puntajeMaq, &cartasHum);

        // Invertir roles
        esHumano = !esHumano;
        if (esHumano)
        {
            estadoDeJuego.cartas = &cartasHum;
            estadoDeJuego.puntajeActual = &puntajeHum;
            estadoDeJuego.puntajeOponente = &puntajeMaq;
            estadoDeJuego.ultimaCartaOponente = &efectoPendMaq;
            estadoDeJuego.nombreActual = nombre;
            estadoDeJuego.nombreOponente = "Maquina";
            elegirCarta = elegirCartaHumano;
        }
        else
        {
            estadoDeJuego.cartas = &cartasMaq;
            estadoDeJuego.puntajeActual = &puntajeMaq;
            estadoDeJuego.puntajeOponente = &puntajeHum;
            estadoDeJuego.ultimaCartaOponente = &efectoPendHum;
            estadoDeJuego.nombreActual = "Maquina";
            estadoDeJuego.nombreOponente = nombre;
            elegirCarta = dificultadMaq;
        }
    }


    if (generarInforme(&informeJuego, estado, nombre) == REALIZADO)
        printf("Informe generado correctamente.\n");
    else
        printf("No se pudo generar el informe.\n");

    leerConfiguracion(&config);
    if(puntajeHum>=12)
        enviarResultadoAPI(&config,nombre,1);
    else
        enviarResultadoAPI(&config,nombre,0);

    vaciarPila(&mazoJuego);

    return estado;
}


int agregarCartas(tVectorCartas* vec, signed char carta, unsigned char cantidad)
{
    for(int i = 0; i < cantidad; i++)
    {
        if(insVecAlFinal(vec, carta) != REALIZADO)
            return SIN_MEM;
    }
    return REALIZADO;
}

int generarMazo(tPila *pMazo)
{
    tVectorCartas vecMazo;
    crearVector(&vecMazo);

    if (agregarCartas(&vecMazo, MAS_DOS, 6) != REALIZADO) return SIN_MEM;
    if (agregarCartas(&vecMazo, MAS_UNO, 10) != REALIZADO) return SIN_MEM;
    if (agregarCartas(&vecMazo, SACAR_UNO, 8) != REALIZADO) return SIN_MEM;
    if (agregarCartas(&vecMazo, SACAR_DOS, 6) != REALIZADO) return SIN_MEM;
    if (agregarCartas(&vecMazo, REPETIR_TURNO, 6) != REALIZADO) return SIN_MEM;
    if (agregarCartas(&vecMazo, ESPEJO, 4) != REALIZADO) return SIN_MEM;

    mezclarVector(&vecMazo);

    for(int i = 0; i < vecMazo.cantElem; i++)
    {
        if(apilar(pMazo, &vecMazo.datos[i], sizeof(signed char)) != REALIZADO)
            return SIN_MEM;
    }


    return REALIZADO;
}



const char* decodificarCarta(int valor)
{
    switch (valor)
    {
    case MAS_UNO:
        return "MAS_UNO";
    case MAS_DOS:
        return "MAS_DOS";
    case SACAR_UNO:
        return "SACAR_UNO";
    case SACAR_DOS:
        return "SACAR_DOS";
    case REPETIR_TURNO:
        return "REPETIR_TURNO";
    case ESPEJO:
        return "ESPEJO";
    default:
        return "CARTA_DESCONOCIDA";
    }
}


int leerConfiguracion(tApi* configuracion)
{
    FILE *pf = fopen("configuracionesApi.txt", "r+t");
    char linea[100];
    char *act;
    if(!pf)
    {
        printf("ERROR AL ABRIR EL ARCHIVO\n");
        return ERROR_ARCH;
    }

    fgets(linea,100,pf);
    fclose(pf);
    act = strchr(linea, '\n');
    if (act)
        *act = '\0';

    act=strrchr(linea,'|');
    strcpy(configuracion->codGrupo,act+1);
    *act='\0';
    strcpy(configuracion->urlAPi,linea);
    return REALIZADO;
}

int  enviarResultadoAPI(tApi* config, const char* nombre, int gano)
{
    CURL* curl;
    CURLcode res;
    char json[256];

    //JSON PARA LA API
    sprintf(json,
            "{\"codigoGrupo\":\"%s\",\"jugador\":{\"nombre\":\"%s\",\"vencedor\":%d}}",
            config->codGrupo, nombre, gano
           );


    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl)
    {

        curl_easy_setopt(curl, CURLOPT_URL, config->urlAPi);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);


        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // SOLICITUD DE POST
        res = curl_easy_perform(curl);

        //VERIFICO ERROR
        if (res != CURLE_OK)
        {
            fprintf(stderr, "Error al enviar POST: %s\n", curl_easy_strerror(res));
        }


        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return REALIZADO;
}

int obtenerRanking(tApi *config)
{
    CURL *curl;
    CURLcode res;
    char urlCompleto[256];
    bufferRanking[0]='\0';
    sprintf(urlCompleto, "%s/%s",config->urlAPi,config->codGrupo);
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        // CARGO EL URL DE GET
        curl_easy_setopt(curl, CURLOPT_URL, urlCompleto);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        //MUESTRA LA RESPUESTA
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, guardarRespuesta);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, bufferRanking);

        // SOLICITO GET
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            fprintf(stderr, "Error en la solicitud: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    mostrarRankingSimple(bufferRanking);

    return REALIZADO;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    printf("%.*s", (int)realsize, (char *)contents);
    return realsize;
}

void eliminarRanking(tApi* config)
{
    CURL* curl;
    CURLcode res;
    char urlCompleto[256];
    sprintf(urlCompleto, "%s/%s",config->urlAPi,config->codGrupo);
    printf("%s\n",urlCompleto);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_URL,urlCompleto);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            fprintf(stderr, "Error al hacer DELETE: %s\n", curl_easy_strerror(res));
        }
        else
        {
            printf("Ranking del grupo '%s' eliminado correctamente.\n", config->codGrupo);
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void mostrarRankingSimple(char* json) {
    char* p = json;
    int pos = 1;

    while ((p = strstr(p, "\"nombreJugador\"")) != NULL) {
        char nombre[100] = "";
        int ganadas = 0;

        p = strchr(p, ':');
        if (!p) break;
        p += 2;


        sscanf(p, "%[^\"]", nombre);
        p = strstr(p, "\"cantidadPartidasGanadas\"");
        if (!p) break;
        p = strchr(p, ':');
        if (!p) break;
        p += 1;

        sscanf(p, "%d", &ganadas);

        printf("%d. %s - %d partidas ganadas\n", pos++, nombre, ganadas);

        p++;
    }
    if (pos == 1) {
        printf("No se encontraron jugadores.\n");
    }
}

char bufferRanking[TAM_BUFFER] = "";

size_t guardarRespuesta(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t tamReal = size * nmemb;
    strncat((char*)userp, (char*)contents, tamReal);
    return tamReal;
}
