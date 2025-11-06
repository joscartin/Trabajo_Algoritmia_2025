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

#include <math.h>
#include <stdbool.h>

double distancia_juego(juego a, juego b) {
    double suma = 0.0;
    double d; // distancia parcial

    // 1 Consola, Publicador, Desarrollador → categóricos (0 o 1)
    if (a.consola != b.consola)  suma += d * d;
    if (a.publicador != b.publicador)  suma += d * d;
    if (a.desarrollador != b.desarrollador)  suma += d * d;

    // 2 Genero (Hamming)
    int dif_gen = 0;
    for (int i = 0; i < 7; i++)
        if (a.genero[i] != b.genero[i]) dif_gen++;
    d = (double)dif_gen / 7.0;
    suma += d * d;

    // 3 Plataforma (Hamming)
    int dif_plat = 0;
    for (int i = 0; i < 11; i++)
        if (a.plataforma[i] != b.plataforma[i]) dif_plat++;
    d = (double)dif_plat / 11.0;
    suma += d * d;

    // 4 Fechas
    int dias_a_sal = a.fecha_sal.año * 365 + a.fecha_sal.mes * 30 + a.fecha_sal.dia;
    int dias_b_sal = b.fecha_sal.año * 365 + b.fecha_sal.mes * 30 + b.fecha_sal.dia;
    d = fabs(dias_a_sal - dias_b_sal);
    suma += d * d;

    int dias_a_upd = a.ult_update.año * 365 + a.ult_update.mes * 30 + a.ult_update.dia;
    int dias_b_upd = b.ult_update.año * 365 + b.ult_update.mes * 30 + b.ult_update.dia;
    d = fabs(dias_a_upd - dias_b_upd);
    suma += d * d;

    // 5 Numéricos
    d = a.rating - b.rating;
    suma += d * d;

    d = (a.ventas_g - b.ventas_g);
    suma += d * d;

    d = (a.ventas_EEUU - b.ventas_EEUU);
    suma += d * d;

    d = (a.ventas_EU - b.ventas_EU);
    suma += d * d;

    d = (a.ventas_Japon - b.ventas_Japon);
    suma += d * d;

    // 7 Raíz cuadrada final
    return sqrt(suma);
}



void ENN (){
}

void knn (data datos, dataset dataset){

}
