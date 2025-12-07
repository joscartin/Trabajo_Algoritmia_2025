#include "listaord.h"


void nuevo (tipolistaord* l){
    *l = NULL;
}

bool esVacio(tipolistaord l){
    return (l==NULL);
}

void insertar (tipolistaord *l, double dist, double pos){
    if (esVacio(*l)){
        listaord *aux=(listaord *)malloc(sizeof(listaord));
        aux->pos=pos;
        aux->dist=dist;
        aux->sig=NULL;
        *l = aux;
    } else {
        listaord *aux=(listaord *)malloc(sizeof(listaord));
        aux->dist=dist;
        aux->pos=pos;
        if ((*l)->dist>aux->dist){
            aux->sig=(*l);
            (*l)=aux;
        } else{
            listaord *aux2=*l;
            while (aux2->dist<aux->dist && aux2->sig!=NULL){
                aux2=aux2->sig;
            }
            if (aux2->sig==NULL){
                aux2->sig=aux;
            } else {
                aux->sig=aux2->sig;
                aux2->sig=aux;
            }

        }
    }
}

void primero(tipolistaord l, double *dist, int *pos){
    *dist=l->dist;
    *pos=l->pos;
}

void eliminar(tipolistaord *l, double dist, double pos){
    if (!esVacio(*l)){
        listaord *aux=*l;
        while(aux->sig!=NULL && aux->sig->dist!=dist && aux->sig->pos!=pos){
            aux=aux->sig;
        }
        if (aux->sig!=NULL){
            listaord *aux2=aux->sig;
            aux->sig=aux2->sig;
            free(aux2);
        }
    }
}

void recorrer(tipolistaord l){
    listaord *aux=l;
    printf("distancia: %f, posicion: %d\n", aux->dist, aux->pos);
    while (aux->sig!=NULL){
        aux=aux->sig;
        printf("distancia: %f, posicion: %d\n", aux->dist, aux->pos);
    }
}


void destruir(tipolistaord *l){
    if (aux!=NULL){
        listaord *aux2=aux->sig;
        while (aux2!=NULL){
            free(aux);
            aux=aux2;
            aux2=aux2->sig;
        }
    }
}
