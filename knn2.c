#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "fragmenta.h"
#include "fragmenta2.h"
#include "listaord.h"

typedef struct fecha{
    int dia;
    int mes;
    int año;
}fecha;


typedef struct Juego{
    char *consola;
    char *publicador;
    char *desarrollador;
    char *genero;
    char *plataforma;
    float fecha_sal;
    float rating;
    float ventas_g;
    float ventas_EEUU;
    float ventas_EU;
    float ventas_Japon;
    bool exitoso;
    bool eliminar;
}Juego;



void normalizar (float *datos, int ndatos){
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

void set_label(Juego datas[], int n, int media){
    int si, no;
    si=0;
    no=0;
    for (int i=0;i<n;i++){
        if (datas[i].ventas_g<media){
            datas[i].exitoso=false;
            no++;
        } else {
            datas[i].exitoso=true;
            si++;
        }
    }
}


double distancia_juego(Juego a, Juego b) {
    float suma = 0.0;
    double d; // distancia parcial

    // 1 Consola, Publicador, Desarrollador → categóricos (0 o 1)
    if (a.consola!=b.consola)  suma += 1;
    if (a.publicador!=b.publicador)  suma += 1;
    if (a.desarrollador!=b.desarrollador)  suma += 1;

    // 2 Genero (Hamming)
    if (a.genero!=b.genero) suma += 1;
    // 3 Plataforma (Hamming)
    if (a.plataforma!=b.plataforma) suma += 1;
    // 4 Fechas
    if (a.fecha_sal!=b.fecha_sal){
        suma+=(a.fecha_sal-b.fecha_sal)*(a.fecha_sal-b.fecha_sal);
    }

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
    return sqrtf(suma);
}

bool knn (Juego datos, Juego dataset[], bool exitos[], int n, int k, int y){
    double distancia;
    int si, no, pos, posmin;
    si=0;
    no=0;
    tipolistaord l;
    nuevo(&l);
    for (int i=0;i<n;i++){//calculo distancias
        distancia=distancia_juego(datos, dataset[i]);
        insertar(&l, distancia, i+y);
    }
    for (int j=0;j<k;j++){//calculo meidas etiquetas de las k primeras distancias
        primero(l, &distancia, &pos);
        eliminar(&l, distancia, pos);
        if (j==0){
            posmin=pos;
        }
        if (dataset[pos].exitoso){
            si++;
        } else {
            no++;
        }
    }
    destruir(&l);
    free(l);


    if (si>no){//calculo etiquetas datos
        return true;
    } else if (si<no){
        return false;
    } else {
    return dataset[posmin].exitoso;
    }
}


/*
void ENN (Juego dataset[], int n, int k){
    for (int i=0; i<n; i++){
        if (knn(dataset[i], dataset, n, k, 0)!=dataset[i].exitoso || dataset[i].ventas_g==0.0){
            dataset[i].eliminar=true;
            printf("%d--", i+1);
        }
    }
    printf("fin\n");
}
*/


void main(int argc, char **argv){
	Juego dataset[1426], control[250], datas[1176];
    int posiciones[250];
	float ventas_g, ventas_EEUU, ventas_EU, ventas_Japon, rating, ventas_gs[2191], ventas_EEUUs[2191], ventas_EUs[2191], ventas_Japons[2191], ratings[2191], fechas[1426];
	char consola[100], publicador[100], desarrollador[100], salida[100], last[100], generos[100], plataformas[100], **fecha;
    int i=0;
    int j;
    double aciertos;
    bool predicciones[2191];
    char **cadena, *insert;


	FILE * archivo = fopen(argv[1], "r");

    while(fscanf(archivo, "%99[^;];%99[^;];%99[^;];%f;%f;%f;%f;%99[^;];%f;%99[^;];%99[^\n]\n", consola, publicador, desarrollador, &(ventas_g), &(ventas_EEUU), &(ventas_EU), &(ventas_Japon), salida, &(rating), generos, plataformas)!=EOF && i<1418){//como comprobar EOF
        
        ventas_gs[i]=ventas_g;
        ventas_EEUUs[i]=ventas_EEUU;
        ventas_EUs[i]=ventas_EU;
        ventas_Japons[i]=ventas_Japon;
        ratings[i]=rating;
        fecha=fragmenta2(salida);
        fechas[i]=atoi(fecha[0])*365+atoi(fecha[1])*30+atoi(fecha[2]);


        dataset[i].consola=consola;
        dataset[i].publicador=publicador;
        dataset[i].desarrollador=desarrollador;
        dataset[i].genero=generos;
        dataset[i].plataforma=plataformas;

        i++;
    }
    normalizar(ventas_gs, 1418);
    normalizar(ventas_EEUUs, 1418);
    normalizar(ventas_EUs, 1418);
    normalizar(ventas_Japons, 1418);
    normalizar(ratings, 1418);
    normalizar(fechas, 1418);
    
    

    for (int i=0; i < 1418; i++){
        dataset[i].ventas_EEUU=ventas_EEUUs[i];
        dataset[i].ventas_g=ventas_gs[i];
        dataset[i].ventas_EU=ventas_EUs[i];
        dataset[i].ventas_Japon=ventas_Japons[i];
        dataset[i].rating=ratings[i];
        dataset[i].fecha_sal=fechas[i];
    }

    for (int i=0; i < 250; i++){
        posiciones[i]=rand()%1245;
    }

    for (int i=0; i<250; i++){
        j=posiciones[i];
        control[i]=dataset[j];
        dataset[j].ventas_g=0.0;
    }

    for (int i=0; i<1418; i++){
        if (dataset[i].ventas_g!=0) datas[i]=dataset[i];
    }


    float mediaventas=0;
    for (i=0; i<1418; i++){
        mediaventas+=dataset[i].ventas_g;
    }
    mediaventas/=1418;

    set_label(dataset, 1418, mediaventas);

    //ENN(dataset, 1426, 1426);//aplicar ENN (ya hecho)
    
    for (int i=1; i<=atoi(argv[2]); i++){
        aciertos=0;
        for (int j=0; j<250; j++){
            predicciones[j]=knn(control[j], datas, predicciones, 1168, i, 250);
        }
        for (int j=0; j<250; j++){
            if (predicciones[j]==control[j].exitoso){
                aciertos++;
            }
        }
        printf("proporción aciertos: %f, k=%d\n", aciertos/250, i);
    }
}
