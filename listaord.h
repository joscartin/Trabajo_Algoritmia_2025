#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct listaord{
    double dist;
    int pos;
    struct listaord *sig;
}listaord;

typedef listaord *tipolistaord;

void nuevo(tipolistaord *);

bool esVacio(tipolistaord);

void insertar (tipolistaord *, double, double);

void eliminar(tipolistaord *, double, double);

void recorrer(tipolistaord);

void primero(tipolistaord , double *, int *);

void destruir(tipolistaord *);