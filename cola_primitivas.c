#include "cola_head.h"

void crearCola(tCola *cola)
{
    cola->pri=NULL;
    cola->ult=NULL;
}

int ponerEnCola(tCola *cola, const void *dato, unsigned tam)
{
    tNodo *nuevo = (tNodo *)malloc(sizeof(tNodo));
    if (!nuevo)
        return SIN_MEM;

    nuevo->dato = malloc(tam);
    if (!nuevo->dato)
    {
        free(nuevo);
        return SIN_MEM;
    }

    memcpy(nuevo->dato, dato, tam);
    nuevo->tam = tam;
    nuevo->sig = NULL;

    if (cola->ult) // si hay al menos un nodo
        cola->ult->sig = nuevo;
    else
        cola->pri = nuevo;

    cola->ult = nuevo;

    return REALIZADO;
}


int sacarDeCola(tCola *cola,void* dato,unsigned tam)
{
    tNodo *elim;
    if(cola->pri==NULL)
        return VACIO;

    elim=cola->pri;
    memcpy(dato,elim->dato,MINIMO(elim->tam,tam));
    cola->pri=elim->sig;

    if(cola->pri==NULL)
        cola->ult=NULL;

    free(elim->dato);
    free(elim);

    return REALIZADO;
}

int verPrimero(const tCola *cola,void* dato,unsigned tam)
{
    if(cola->pri==NULL)
        return VACIO;

    memcpy(dato,cola->pri->dato,MINIMO(tam,cola->pri->tam));

    return REALIZADO;
}

int colaLlena(const tCola *cola, unsigned tam)
{
    return REALIZADO;
}

int colaVacia(const tCola *cola)
{
    return cola->pri == NULL ? VACIO : REALIZADO;
}

void vaciarCola(tCola *cola)
{
    tNodo *aux;
    while(cola->pri)
    {
        aux=cola->pri;
        cola->pri=aux->sig;
        free(aux->dato);
        free(aux);
    }
    cola->ult=NULL;
}
