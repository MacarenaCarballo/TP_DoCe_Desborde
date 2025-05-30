#include "comun.h"

#define CANT_REPARTO 3

void mostrarCartasEnLinea(const tVectorCartas *cartas) {
    const char *nombres[] = {
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

    // Línea superior
    for (i = 0; i < cant; i++) printf("+---------+   ");
    printf("\n");

    // Línea vacía
    for (i = 0; i < cant; i++) printf("|         |   ");
    printf("\n");

    // Línea con valor numérico
    for (i = 0; i < cant; i++) {
        carta = (signed char)(cartas->datos[i]);
        printf("|  %3d     |   ", carta);
    }
    printf("\n");

    // Línea con nombre de la carta
    for (i = 0; i < cant; i++) {
        carta = (signed char)(cartas->datos[i]);

        const char *nombre;
        switch (carta) {
            case -2: nombre = nombres[0]; break;
            case -1: nombre = nombres[1]; break;
            case  1: nombre = nombres[2]; break;
            case  2: nombre = nombres[3]; break;
            case  3: nombre = nombres[4]; break;
            case  4: nombre = nombres[5]; break;
            default: nombre = "???????"; break;
        }

        printf("| %-8s|   ", nombre);
    }
    printf("\n");

    // Línea inferior
    for (i = 0; i < cant; i++) printf("+---------+   ");
    printf("\n");

    // Índices debajo
    for (i = 0; i < cant; i++) printf("   [%d]       ", i + 1);
    printf("\n");
}

void mostrarTablero(unsigned char puntosHum, unsigned char puntosMaq, const tVectorCartas *cartasHum) {
    puts("\n=========================================");
    printf(" PUNTAJES -> Humano: %2d | Maquina: %2d\n", puntosHum, puntosMaq);
    puts("=========================================");
    puts(" Tus cartas:\n");

    mostrarCartasEnLinea(cartasHum);

    puts("=========================================\n");
}

int repartirCartas(tPila *mazo, tVectorCartas *jugHum, tVectorCartas *jugMaq) {
    signed char carta,
                    i;

    for (i = 0; i < CANT_REPARTO; i++) {
        // Jugador humano
        if (!desapilar(mazo, &carta, sizeof(signed char)))
            return VACIO;
        if (!vector_insertar(jugHum, carta))
            return SIN_MEM;

        // Máquina
        if (!desapilar(mazo, &carta, sizeof(signed char)))
            return VACIO;
        if (!vector_insertar(jugMaq, carta))
            return SIN_MEM;
    }

    return REALIZADO;
}

int jugar(char nombre[], int dificultad)
{
    tPila mazoJuego;

    tVectorCartas cartasHum,
                  cartasMaq,
                  mazoDescarte;

    tCola informeJuego;

    unsigned char nroDeTurno = 1,
                  puntajeHum = 0,
                  puntajeMaq = 0;

    unsigned char opcion;

    crearPila(&mazoJuego);
    crearVector(&cartasHum);
    crearVector(&cartasMaq);
    crearVector(&mazoDescarte);
    crearCola(&informeJuego);

    if(generarMazo(&mazoJuego)!= REALIZADO){
        puts("No se pudo generar el mazo de juego");
        return PILA_LLENA;
    }

    repartirCartas(&mazoJuego,&cartasHum,&cartasMaq);

    while( puntajeHum < 12 && puntajeMaq < 12)
    {
        system("cls"); // LIMPIA PANTALLA en Windows. En Linux usar "clear"
        mostrarTablero(puntajeHum, puntajeMaq, &cartasHum);

        // luego hacés que el jugador elija una carta
        printf("Elegí una carta (1 a %d): ", cartasHum.cantElem);
        scanf("%d", &opcion);
    }

    vaciarCola(&informeJuego);
    vaciarPila(&mazoJuego);
}



int generarMazo(tPila *pMazo)
{
    tVectorCartas vecMazo;
    signed char carta;

    crearVector(&vecMazo);

    if (insVecAlFinal(vecMazo, MAS_DOS, 6) != REALIZADO) return SIN_MEM;
    if (insVecAlFinal(vecMazo, MAS_UNO, 10) != REALIZADO) return SIN_MEM;
    if (insVecAlFinal(vecMazo, SACAR_UNO, 8) != REALIZADO) return SIN_MEM;
    if (insVecAlFinal(vecMazo, SACAR_DOS, 6) != REALIZADO) return SIN_MEM;
    if (insVecAlFinal(vecMazo, REPETIR_TURNO, 6) != REALIZADO) return SIN_MEM;
    if (insVecAlFinal(vecMazo, ESPEJO, 4) != REALIZADO) return SIN_MEM;

    mezclarVector(&vecMazo);

    while(sacarUltVec(&vecMazo,&carta)!= VACIO)
        if(apilar(&pMazo,&carta,sizeof(signed char))!=REALIZADO)
            return SIN_MEM;

    vaciarVector(&vecMazo);

    return REALIZADO;
}

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

const char* decodificarCarta(int valor)
{
    switch (valor)
    {
        case MAS_UNO: return "MAS_UNO";
        case MAS_DOS: return "MAS_DOS";
        case SACAR_UNO: return "SACAR_UNO";
        case SACAR_DOS: return "SACAR_DOS";
        case REPETIR_TURNO: return "REPETIR_TURNO";
        case ESPEJO: return "ESPEJO";
        default: return "CARTA_DESCONOCIDA";
    }
}

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


