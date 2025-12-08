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
            listaord *auxant=aux2;
            while (aux2->dist<aux->dist && aux2->sig!=NULL){
                auxant=aux2;
                aux2=aux2->sig;
            }
            if (aux2->sig==NULL){
                aux2->sig=aux;
                aux->sig=NULL;
            } else {
                aux->sig=aux2;
                auxant->sig=aux;
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
        listaord *aux, *auxant;
        auxant=*l;
        aux=auxant;
        if (aux!=NULL){
            while(aux->sig!=NULL && (aux->dist!=dist || aux->pos!=pos)){
                auxant=aux;
                aux=aux->sig;
            }
            if (aux->sig==NULL && aux->dist==dist && aux->pos==pos){
                free(aux);
                auxant->sig=NULL;
            } else if (aux!=auxant){
                auxant->sig=aux->sig;
                free(aux);
            } else {
                *l=auxant->sig;
                free(auxant);
            }
        } else if(auxant->dist==dist && auxant->pos==pos){
            free(auxant);
            *l=NULL;
        }
    }
}

void recorrer(tipolistaord l){
    if (!esVacio(l)){
        listaord *aux=l;
        printf("distancia: %f, posicion: %d\n", aux->dist, aux->pos);
        while (aux->sig!=NULL){
            aux=aux->sig;
            printf("distancia: %f, posicion: %d\n", aux->dist, aux->pos);
        }
    } else {
        printf("lista vacia\n");
    }
}


void destruir(tipolistaord *l){
    if (!esVacio(*l)){
        listaord *aux=*l;
        listaord *aux2=aux->sig;
        while (aux!=NULL){
            eliminar(l, aux->dist, aux->pos);
            aux=aux2;
            if (aux2!=NULL){
                aux2=aux2->sig;
            }
        }
        *l=NULL;
    }
}
