#include "comun.h"

#define CANT_REPARTO 3

signed char modoFACIL(const tEstadoJuego* estadoDeJuego)
{
    srand(time(NULL));

    int numero = rand() % 3;

    return estadoDeJuego->cartas->datos[numero];
}

signed char modoMEDIO(const tEstadoJuego* estadoDeJuego)
{
    return 0;
}
signed char modoDIFICIL(const tEstadoJuego* estadoDeJuego)
{
    return 0;
}

int reponerCarta(tPila* mazoJuego, tVectorCartas* mazoDescarte, tVectorCartas* mano)
{
    signed char nuevaCarta;

    // Intentar sacar una carta del mazo
    if (desapilar(mazoJuego, &nuevaCarta, sizeof(nuevaCarta)) == VACIO) {
        // Mezclar descarte y reponer el mazo
        mezclarVector(mazoDescarte);

        while (sacarUltVec(mazoDescarte, &nuevaCarta) == REALIZADO) {
            apilar(mazoJuego, &nuevaCarta, sizeof(nuevaCarta));
        }

        // Intentar de nuevo
        if (desapilar(mazoJuego, &nuevaCarta, sizeof(nuevaCarta)) == VACIO) {
            puts("No se pudo recargar el mazo. Juego terminado.");
            return VACIO;
        }
    }

    // Agregar la nueva carta a la mano del jugador
    return insVecAlFinal(mano, nuevaCarta);
}
signed char aplicarEfecto(signed char efecto,
                          unsigned char* puntajeActual,
                          unsigned char* puntajeOponente,
                          unsigned char* nroDeTurno,
                          tCola* informeJuego,
                          const char* nombreJugadorActual,
                          const char* nombreOponente,
                          int esHumano,
                          signed char* ultimoEfectoOponente)
{
    char mensaje[150];
    char detalleTurno[300];
    signed char resultado = REALIZADO;

    switch(efecto)
    {
    case SACAR_DOS:    // -2
    case SACAR_UNO:    // -1
        *puntajeOponente += efecto;
        if (*puntajeOponente < 0)
            *puntajeOponente = 0;

        snprintf(mensaje, sizeof(mensaje),
                 "%s jugó carta %s. %s pierde %d punto%s.",
                 nombreJugadorActual,
                 (efecto == SACAR_UNO) ? "SACAR_UNO" : "SACAR_DOS",
                 nombreOponente,
                 -efecto,
                 (efecto == SACAR_UNO) ? "" : "s");

        *ultimoEfectoOponente = efecto;  // Guardamos por si viene ESPEJO
        break;

    case MAS_UNO:
    case MAS_DOS:
        *puntajeActual += efecto;

        snprintf(mensaje, sizeof(mensaje),
                 "%s jugó carta %s. ¡Gana %d punto%s!",
                 nombreJugadorActual,
                 (efecto == MAS_UNO) ? "MAS_UNO" : "MAS_DOS",
                 efecto,
                 (efecto == MAS_UNO) ? "" : "s");

        if (*puntajeActual >= 12)
            resultado = esHumano ? GANO_HUMANO : GANO_MAQUINA;
        break;

    case REPETIR_TURNO:
        snprintf(mensaje, sizeof(mensaje),
                 "%s jugó carta REPETIR_TURNO. ¡Juega otra vez!",
                 nombreJugadorActual);

        return REPETIR_TURNO;  // Retornar directamente para que el flujo lo repita
        break;

    case ESPEJO:
        if (*ultimoEfectoOponente == SACAR_UNO || *ultimoEfectoOponente == SACAR_DOS)
        {
            efecto = *ultimoEfectoOponente;

            // Deshacer al actual y devolver al oponente
            *puntajeActual -= efecto;
            *puntajeOponente += efecto;

            if (*puntajeActual < 0)
                *puntajeActual = 0;
            if (*puntajeOponente < 0)
                *puntajeOponente = 0;

            snprintf(mensaje, sizeof(mensaje),
                     "%s jugó ESPEJO. ¡Devuelve el efecto de %d punto%s a %s!",
                     nombreJugadorActual,
                     -efecto,
                     (efecto == SACAR_UNO) ? "" : "s",
                     nombreOponente);
        }
        else
        {
            snprintf(mensaje, sizeof(mensaje),
                     "%s jugó ESPEJO, pero no había efecto para devolver.",
                     nombreJugadorActual);
        }
        break;
    }

    snprintf(detalleTurno, sizeof(detalleTurno),
             "[Turno %d] %s\n  Puntos %s: %d\n  Puntos %s: %d\n",
             *nroDeTurno,
             mensaje,
             nombreJugadorActual, *puntajeActual,
             nombreOponente, *puntajeOponente);

    ponerEnCola(informeJuego, detalleTurno, strlen(detalleTurno) + 1);

    return resultado;
}
int ejecutarTurno(tVectorCartas *mazoDescarte,
                  tPila *mazoJuego,
                  int esHumano,
                  tVectorCartas* cartas,
                  signed char* efectoPendOponente,
                  unsigned char * puntajeActual,
                  unsigned char * puntajeOponente,
                  unsigned char* nroDeTurno,
                  tCola* informe,
                  const char* nombreActual,
                  const char* nombreOponente,
                  tFuncionElegirCarta elegirCarta,
                  tEstadoJuego* estadoJuego)
{
    signed char carta;
    static signed char ultimoEfectoHumano = 0;
    static signed char ultimoEfectoMaquina = 0;
    signed char* ultimoEfectoOponente = esHumano ? &ultimoEfectoMaquina : &ultimoEfectoHumano;
    signed char estado;
    // Elegir carta (humano o IA)
    carta = elegirCarta(estadoJuego);

    // Aplicar efecto
        estado = aplicarEfecto(carta,
                               puntajeActual,
                               puntajeOponente,
                               nroDeTurno,
                               informe,
                               nombreActual,
                               nombreOponente,
                               esHumano,
                               ultimoEfectoOponente);

    // Guardar efecto aplicado, si corresponde
    if (carta == SACAR_UNO || carta == SACAR_DOS)
        *efectoPendOponente = carta;

    //  Reposición de carta
    if (reponerCarta(mazoJuego, mazoDescarte, cartas) != REALIZADO)
        return VACIO;

    // Si la carta fue REPETIR_TURNO, repetir este mismo jugador (no alternar)
    if (carta == REPETIR_TURNO)
        (*nroDeTurno)--;

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


signed char elegirCartaHumano(const tEstadoJuego* estado)
{
    unsigned char opcion;   // Solo puede valer 1, 2 o 3
    int r;
    signed char carta;

    do
    {
        printf("Elegí una carta (1 a 3): ");
        r = scanf("%hhu", &opcion);  // %hhu para unsigned char
        while(getchar() != '\n');    // Limpiar buffer

        if (r != 1 || opcion < 1 || opcion > 3)
            printf("Entrada inválida. Intente nuevamente.\n");

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

unsigned char jugar(const char *nombre, tFuncionElegirCarta dificultadMaq) {
    tPila mazoJuego;
    tVectorCartas cartasHum,
                  cartasMaq,
                  mazoDescarte;
    tCola informeJuego;

    unsigned char nroDeTurno = 1;
    unsigned char puntajeHum = 0, puntajeMaq = 0;
    signed char efectoPendHum = 0, efectoPendMaq = 0;
   // signed char carta;
    int estado;

    tEstadoJuego estadoDeJuego;
    // Turno dinámico
    int esHumano = 1;
    tVectorCartas* manoActual = &cartasHum;
    tVectorCartas* manoOponente = &cartasMaq;
    signed char* efectoOponente = &efectoPendMaq;
    unsigned char* puntajeActual = &puntajeHum;
    unsigned char* puntajeOponente = &puntajeMaq;
    const char* nombreActual = nombre;
    const char* nombreOponente = "Maquina";
    signed char (*elegirCarta)(const tEstadoJuego*) = elegirCartaHumano;

    // Inicializaciones
    crearPila(&mazoJuego);
    crearVector(&cartasHum);
    crearVector(&cartasMaq);
    crearVector(&mazoDescarte);
    crearCola(&informeJuego);

    if (generarMazo(&mazoJuego) != REALIZADO) {
        puts("No se pudo generar el mazo de juego");
        return SIN_MEM;
    }

    repartirCartas(&mazoJuego, &cartasHum, &cartasMaq);


    while (puntajeHum < 12 && puntajeMaq < 12) {
        system("cls");
        mostrarTablero(puntajeHum, puntajeMaq, &cartasHum);

        printf("Turno de %s:\n", nombreActual);

        estado = ejecutarTurno(&mazoDescarte,
                               &mazoJuego,
                               esHumano,
                               manoActual,
                               efectoOponente,
                               puntajeActual,
                               puntajeOponente,
                               &nroDeTurno,
                               &informeJuego,
                               nombreActual,
                               nombreOponente,
                               elegirCarta,
                               &estadoDeJuego);
        if (estado != REALIZADO)
            break;

        nroDeTurno++;

        // Invertir roles
        esHumano = !esHumano;
        if (esHumano) {
            manoActual = &cartasHum;
            manoOponente = &cartasMaq;
            efectoOponente = &efectoPendMaq;
            puntajeActual = &puntajeHum;
            puntajeOponente = &puntajeMaq;
            nombreActual = nombre;
            nombreOponente = "Maquina";
            elegirCarta = elegirCartaHumano;
        } else {
            manoActual = &cartasMaq;
            manoOponente = &cartasHum;
            efectoOponente = &efectoPendHum;
            puntajeActual = &puntajeMaq;
            puntajeOponente = &puntajeHum;
            nombreActual = "Maquina";
            nombreOponente = nombre;
            elegirCarta = dificultadMaq; // puntero a función según modo
        }
    }

    // Finalizar cola (mostrar o guardar)
    printf("\n--- Informe de juego ---\n");
   // mostrarYVaciarCola(&informeJuego); // o guardarColaEnArchivo() si querés persistencia

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

    // Suponiendo que cargar en la pila es:
    for(int i = 0; i < vecMazo.cantElem; i++)
    {
        if(apilar(pMazo, &vecMazo.datos[i], sizeof(signed char)) != REALIZADO)
            return SIN_MEM;
    }


    return REALIZADO;
}

/*
signed char modoFACIL(tVectorCartas *vecCartas, unsigned char *pjeMaquina, unsigned char *pjeHumano, signed char *jugadaHumano)
{
    srand(time(NULL));

    int numero = rand() % vecCartas->cantElem;
    signed char carta;

    verCartaPorPos(vecCartas,numero, &carta)

    return carta;
}

signed char modoMEDIO(tVectorCartas *vecCartas, unsigned char *pjeMaquina, unsigned char *pjeHumano, signed char *jugadaHumano)
{
    signed char cartaMaq;
    int i;

    if(*pjeMaquina>CERCA_GANAR)
    {
        for(i=0; i<vecCartas->cantElem; i++)
        {
            if(verCartaPorPos(vecCartas,i,&cartaMaq)==REALIZADO)
            {

                if((cartaMaq==MAS_UNO)||(cartaMaq==MAS_DOS))
                    return cartaMaq;
            }
        }
    }
    else if (*pjeHumano==0)
    {
        for(i=0; i<vecCartas->cantElem; i++)
        {
            if(verCartaPorPos(vecCartas,i,&cartaMaq)==REALIZADO)
            {
                if((cartaMaq!=SACAR_UNO)&&(cartaMaq!=SACAR_DOS))
                    return cartaMaq;
            }
        }
    }

    return modoFACIL(vec,pjeMaquina,pjeHumano,jugadaHumano);
}

signed char modoDIFICIL(tVectorCartas *vecCartas, unsigned char *pjeMaquina, unsigned char *pjeHumano, signed char *jugadaHumano)
{
    int i,
        posRepetir,
        cartasBuenas=0,
        hayRepetir =0;
    signed char cartaMaq;

    for(i=0; i<vecCartas->cantElem; i++)
    {
        if(verCartaPorPos(vecCartas,i,&cartaMaq)==REALIZADO)
        {

            if((*jugadaHumano==SACAR_UNO)||(*jugadaHumano==SACAR_DOS))
                if(cartaMaq==ESPEJO)
                    return cartaMaq;

            if(*pjeHumano > CERCA_GANAR)
            {
                if((cartaMaq==SACAR_UNO)||(cartaMaq==SACAR_DOS))
                    return cartaMaq;
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
        if(verCartaPorPos(vecCartas,posRepetir,&cartaMaq)==REALIZADO)
            return cartaMaq;
    }

    return modoMEDIO(vecCartas,pjeMaquina,pjeHumano,jugadaHumano);
}
*/

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
/*
int generarInforme(tCola *informe, int ganador, char *nombreJugador)
{
    tInforme jugada;
    char nombreArchivo[100];
    char *maq="MAQUINA";
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t); //ESTO ES UNA FUNCION PARA TENER EL DIA ACTUAL CON FECHA Y HORA
    //SALE DE LA LIBRERIA <TIME.H>

    sprintf(nombreArchivo,
        "informe-juego_%04d-%02d-%02d-%02d-%02d.txt",
        tm_info->tm_year + 1900,
        tm_info->tm_mon + 1,
        tm_info->tm_mday,
        tm_info->tm_hour,
        tm_info->tm_min
    );

    FILE* pInforme=fopen(nombreArchivo,"wt");
    if(!pInforme)
    {
        printf("ERROR EN LA APERTURA DEL ARCHIVO INFORME\n");
        return -1;
    }

    fprintf(pInforme,"INFORME DEL JUEGO\n\n");
    while(colaVacia(informe)!=COLA_VACIA)
    {
        sacarDeCola(informe,&jugada,sizeof(tInforme));
        fprintf(pInforme,"NUMERO DE TURNO:%d\n",jugada.numTurno);
        fprintf(pInforme,"Carta Jugada por %s: %s\n",nombreJugador,jugada.cartaJugador);
        fprintf(pInforme,"Carta Jugada por %s: %s\n",maq,jugada.cartaMaquina);
        fprintf(pInforme,"Puntos Acumulados por %s: %d\n",nombreJugador,jugada.puntosJugador);
        fprintf(pInforme,"Puntos Acumulados por %s: %d\n\n",maq,jugada.puntosMaquina);


    }
    fprintf(pInforme,"EL GANADOR DEL JUEGO ES: %s\n", ganador==5? nombreJugador: "MAQUINA");

    fclose(pInforme);
    return REALIZADO;
}

int leerConfiguracion(tApi* configuracion)
{
    FILE *pf=fopen("configuracionesApi.txt", "r+t");
    char linea[100];
    char *act;
    if(!pf){
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

    if (curl) {

        curl_easy_setopt(curl, CURLOPT_URL, config->urlAPi);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);


        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // SOLICITUD DE POST
        res = curl_easy_perform(curl);

        //VERIFICO ERROR
        if (res != CURLE_OK) {
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
    sprintf(urlCompleto, "%s/%s",config->urlAPi,config->codGrupo);
    printf("%s\n",urlCompleto);
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // CARGO EL URL DE GET
        curl_easy_setopt(curl, CURLOPT_URL, urlCompleto);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        //MUESTRA LA RESPUESTA
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // SOLICITO GET
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "Error en la solicitud: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

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

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_URL,urlCompleto);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "Error al hacer DELETE: %s\n", curl_easy_strerror(res));
        } else {
            printf("Ranking del grupo '%s' eliminado correctamente.\n", config->codGrupo);
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}


*/
