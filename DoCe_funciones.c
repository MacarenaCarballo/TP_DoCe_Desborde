#include "comun.h"

#define CANT_REPARTO 3

int aplicarEfecto(int efecto, int* puntajeHum, int* puntajeMaq, unsigned char* nroDeTurno, tCola* informeJuego, const char* jugadorActual)
{
    char mensaje[100];
    char detalleTurno[256];

    switch(efecto)
    {
    case SACAR_DOS:    // -2
    case SACAR_UNO:    // -1
        *puntajeMaq += efecto;  // Resta puntos (efecto es negativo)
        if(*puntajeMaq < 0) *  puntajeMaq = 0;
        snprintf(mensaje, sizeof(mensaje), "%s jugó carta %s. Magutna pierde %d punto%s.",
                 jugadorActual,
                 (efecto == SACAR_UNO) ? "SACAR_UNO" : "SACAR_DOS",
                 -efecto,
                 (efecto == SACAR_UNO) ? "" : "s");
        break;

    case MAS_UNO:     // +1
    case MAS_DOS:     // +2
        *puntajeHum += efecto;
        snprintf(mensaje, sizeof(mensaje), "%s jugó carta %s. ¡Gana %d punto%s!",
                 jugadorActual,
                 (efecto == MAS_UNO) ? "MAS_UNO" : "MAS_DOS",
                 efecto,
                 (efecto == MAS_UNO) ? "" : "s");
        break;

    case REPETIR_TURNO:  // 3
        (*nroDeTurno)--;
        snprintf(mensaje, sizeof(mensaje), "%s jugó carta REPETIR_TURNO. ¡Juega otra vez!", jugadorActual);
        break;

    case ESPEJO:  // 4
        // Devuelve el último efecto aplicado (ejemplo simplificado)
        snprintf(mensaje, sizeof(mensaje), "%s jugó carta ESPEJO. ¡Magutna devuelve el ataque!", jugadorActual);
        // Aquí iría la lógica para invertir el último efecto (depende de tu implementación)
        break;
    }

    // Registra el estado completo del turno
    snprintf(detalleTurno, sizeof(detalleTurno), "[Turno %d] %s\n  Puntos %s: %d\n  Puntos Magutna: %d\n",
             *nroDeTurno,
             mensaje,
             jugadorActual,
             *puntajeHum,
             *puntajeMaq);

    ponerEnCola(informeJuego, detalleTurno, sizeof(256));
    return 0;
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
        printf("|  %3d     |   ", carta);
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
        if (!desapilar(mazo, &carta, sizeof(signed char)))
            return VACIO;
        if (!insVecAlFinal(jugHum, carta))
            return SIN_MEM;

        // Máquina
        if (!desapilar(mazo, &carta, sizeof(signed char)))
            return VACIO;
        if (!insVecAlFinal(jugMaq, carta))
            return SIN_MEM;
    }

    return REALIZADO;
}


signed char elegirCartaHumano(tVectorCartas* cartasHum)
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
    if (elimPorPosVec(cartasHum, opcion - 1, &carta) != REALIZADO)
    {
        printf("Error eliminando la carta del vector.\n");
        return VACIO;
    }

    return carta;
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

    signed char carta,
           temporal;

    int opcion;
    int r;

    crearPila(&mazoJuego);
    crearVector(&cartasHum);
    crearVector(&cartasMaq);
    crearVector(&mazoDescarte);
    crearCola(&informeJuego);

    if(generarMazo(&mazoJuego)!= REALIZADO)
    {
        puts("No se pudo generar el mazo de juego");
        return SIN_MEM;
    }

    repartirCartas(&mazoJuego,&cartasHum,&cartasMaq);

    while( puntajeHum < 12 && puntajeMaq < 12 )
    {
        system("cls");
        mostrarTablero(puntajeHum, puntajeMaq, &cartasHum);


        if((carta = elegirCartaHumano(&cartasHum))==VACIO)
            return VACIO;


        /*
                    desapilar(carta, mazo)...
                    insVecAlFinal(mazoDescarte)...
                    insVecAlFinal(cartasHum)...
        */
        if(puntajeHum < 12)
        {
            if(carta ==SACAR_UNO || carta == SACAR_DOS)
            {
                temporal = carta;
            }


            /*
                hacer
            */
            /**
                1 o 2 aplique los efectos y modificar puntos
                -2 o -1 estas cartas puede tirarme espejo
            */

            // juegaMAquina

        }



    }

    vaciarCola(&informeJuego);
    vaciarPila(&mazoJuego);

    return 0;
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

int modoFACIL( int vecCartas[])
{
    srand(time(NULL));

    int numero = rand() % 3;

    return vecCartas[numero];
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
