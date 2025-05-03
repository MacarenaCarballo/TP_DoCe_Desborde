#include "pila_head.h"

void crearPila(tPila* pila)
{
    pila->tope=TAM;
}

int apilar(tPila *pila,const void *dato, unsigned tam)
{
    if(pila->tope < tam + sizeof(unsigned))
    {
        return PILA_LLENA;
    }
    pila->tope-=tam;
    memcpy( pila->pila+pila->tope,dato,tam);
    pila->tope-= sizeof(tam);
    memcpy(pila->pila+pila->tope,&tam,sizeof(tam));

    return SATISFACTORIO;

}

int pilaLlena(const tPila *pila, unsigned tam)
{
    if(pila->tope < tam + sizeof(unsigned))
    {
        return PILA_LLENA;
    }
    return PILA_DISPONIBLE;
}

int pilaVacia(const tPila *pila)
{
    if(pila->tope==TAM)
    {
        return PILA_VACIA;
    }
    return PILA_NO_VACIA;
}

int verTope(const tPila *pila, void *dato, unsigned tam)
{
    unsigned tamInfo;
    if(pila->tope==TAM)
    {
        return PILA_VACIA;
    }

    memcpy(&tamInfo,pila->pila+pila->tope,sizeof(unsigned));
    memcpy(dato,pila->pila+pila->tope+sizeof(unsigned), MINIMO(tam,tamInfo));
    return SATISFACTORIO;
}

void vaciarPila(tPila *pila)
{
    pila->tope=TAM;
}

int desapilar(tPila *pila, void * dato, unsigned tam)
{
    unsigned tamInfo;
    if(pila->tope==TAM)
    {
        return PILA_VACIA;
    }
    memcpy(&tamInfo,pila->pila+pila->tope,sizeof(unsigned));
    pila->tope+=sizeof(unsigned);
    memcpy(dato,pila->pila+pila->tope,MINIMO(tam,tamInfo));
    pila->tope+=tamInfo;
    return SATISFACTORIO;
}
