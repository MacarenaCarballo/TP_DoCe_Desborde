#include "comun.h"

void crearPila(tPila* pila)
{
    pila->tope=TAM;
}

int apilar(tPila *pila,const void *dato, unsigned tam)
{
    if(pila->tope < tam + sizeof(unsigned))
        return SIN_MEM;

    pila->tope-=tam;
    memcpy( pila->pila+pila->tope,dato,tam);
    pila->tope-= sizeof(tam);
    memcpy(pila->pila+pila->tope,&tam,sizeof(tam));

    return REALIZADO;

}

int pilaLlena(const tPila *pila, unsigned tam)
{
    if(pila->tope < tam + sizeof(unsigned))
        return SIN_MEM;

    return REALIZADO;
}

int pilaVacia(const tPila *pila)
{
    if(pila->tope==TAM)
        return VACIO;

    return REALIZADO;
}

int verTope(const tPila *pila, void *dato, unsigned tam)
{
    unsigned tamInfo;
    if(pila->tope==TAM)
        return VACIO;


    memcpy(&tamInfo,pila->pila+pila->tope,sizeof(unsigned));
    memcpy(dato,pila->pila+pila->tope+sizeof(unsigned), MINIMO(tam,tamInfo));

    return REALIZADO;
}

void vaciarPila(tPila *pila)
{
    pila->tope=TAM;
}

int desapilar(tPila *pila, void * dato, unsigned tam)
{
    unsigned tamInfo;

    if(pila->tope==TAM)
        return VACIO;

    memcpy(&tamInfo,pila->pila+pila->tope,sizeof(unsigned));
    pila->tope+=sizeof(unsigned);
    memcpy(dato,pila->pila+pila->tope,MINIMO(tam,tamInfo));
    pila->tope+=tamInfo;

    return REALIZADO;
}
