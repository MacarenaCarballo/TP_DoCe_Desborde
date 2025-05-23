#include "DoCe_head.h"
#include "pila_head.h"
#include "cola_head.h"

int generarMazo(tPila *mazo)
{
    int i,num;
    num=MAS_DOS;
    for (i = 0; i < 6; i++)
        apilar(mazo,&num,sizeof(int));

    num=MAS_UNO;
    for (i = 0; i < 10; i++)
        apilar(mazo,&num,sizeof(int));

    num=SACAR_UNO;
    for (i = 0; i < 8; i++)
        apilar(mazo,&num,sizeof(int));

    num=SACAR_DOS;
    for (i = 0; i < 6; i++)
        apilar(mazo,&num,sizeof(int));

    num=REPETIR_TURNO;
    for (i = 0; i < 6; i++)
        apilar(mazo,&num,sizeof(int));

    num=ESPEJO;
    for (i = 0; i < 4; i++)
        apilar(mazo,&num,sizeof(int));

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
    if(!pf)
    {
        printf("ERROR AL ABRIR EL ARCHIVO\n");
        return -1;

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

    return 0;
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

    return 0;
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

    return 0;
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

