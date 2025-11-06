#include <stdbool.h>
void normalizar (double *datos, int ndatos){
    int i;
    float maximo, minimo;
    maximo=datos[0];
    minimo=datos[0];
    for (i=0;i<ndatos;i++){
        if (datos[i]>maximo){
            maximo=datos[i];
        } else if (datos[i]<minimo){
            minimo=datos[i];
        }
    }
    for (i=0;i<ndatos;i++){
        datos[i]=(datos[i]-minimo)/(maximo-minimo);
    }
}

void set_label(dataset datas){
    for (data elemento : datas){
        if (elemento.ventas_g<10000000){
            elemento.exitoso=False;
        } else {
            elemento.exitoso=True;
        }
    }
}

void distancias()

void ENN (){
}

void knn (data datos, dataset dataset){

}