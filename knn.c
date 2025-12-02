#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef struct fecha{
    int dia;
    int mes;
    int año;
}fecha;

typedef int Consola;
    /*0=ps3, 1=ps4, 2=ps5, 3=otro*/


typedef int Publicador;
    /*0=Rockstar, 1=Activision, 2=EA, 3=Sony, 4=Bethesda, 5=otro*/


typedef int Desarrollador;
    /*0=Rockstar, 1=Treyarch, 2=Sledgehamer, 3=InfinityWard, 4=EA vancouver, 5=EA Canada, 6=Naughty Dog, 7=EA Sports, 8=Insomniac Games, 9=Bethesda*/


typedef int Genero[7];
    /*0=accion, 1=shooter, 2=Sports, 3=simulacion, 4=rpg, 5=aventura, 6=otro*/

typedef int Plataforma[11];
/*0=Pc, 1=ps5, 2=xbox one, 3=ps4, 4=xbox series s/x, 5=xbox 360, 6=play3, 7=Wii U, 8=Wii, 9=ios, 10=macOS, 11=swich*/


typedef struct Juego{
    char *consola;
    char *publicador;
    char *desarrollador;
    char *genero;
    char *plataforma;
    char *fecha_sal;
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

void set_label(Juego datas[], int n){
    for (int i=0;i<n;i++){
        if (datas[i].ventas_g<0.5){
            datas[i].exitoso=false;
        } else {
            datas[i].exitoso=true;
        }
    }
}


double distancia_juego(Juego a, Juego b) {
    float suma = 0.0;
    double d; // distancia parcial

    // 1 Consola, Publicador, Desarrollador → categóricos (0 o 1)
    if (strcmp(a.consola, b.consola)!=0)  suma += 1;
    if (strcmp(a.publicador, b.publicador)!=0)  suma += 1;
    if (strcmp(a.desarrollador, b.desarrollador)!=0)  suma += 1;

    // 2 Genero (Hamming)
    if (strcmp(a.genero, b.genero)!=0) suma += 1;
    /*int dif_gen = 0;
    for (int i = 0; i < 7; i++)
        if (a.genero[i] != b.genero[i]) dif_gen++;
    d = (double)dif_gen / 7.0;
    suma += d * d;
*/
    // 3 Plataforma (Hamming)
    if (strcmp(a.plataforma, b.plataforma)!=0) suma += 1;
    /*int dif_plat = 0;
    for (int i = 0; i < 11; i++)
        if (a.plataforma[i] != b.plataforma[i]) dif_plat++;
    d = (double)dif_plat / 11.0;
    suma += d * d;
*/
    // 4 Fechas
    if (strcmp(a.fecha_sal, b.fecha_sal)!=0)  suma += 1;
    /*
    int dias_a_sal = a.fecha_sal.año * 365 + a.fecha_sal.mes * 30 + a.fecha_sal.dia;
    int dias_b_sal = b.fecha_sal.año * 365 + b.fecha_sal.mes * 30 + b.fecha_sal.dia;
    d = fabs(dias_a_sal - dias_b_sal);
    suma += d * d;
    */

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

void ordena(double *(distancias[][2]), int size) {
    bool intercambios = true;
    int numPasada = size - 1;
    double temp[2];  // Para almacenar temporalmente una fila (par de números)
    
    while (numPasada > 0 && intercambios) {
        intercambios = false;
        for (int i = 0; i < numPasada; i++) {
            // Comparamos solo el primer valor de cada fila (distancias[i][0])
            if (*distancias[i][0] > *distancias[i + 1][0]) {
                intercambios = true;
                // Intercambiamos las filas completas (todas las columnas de la fila)
                for (int j = 0; j < 2; j++) {
                    temp[j] = *distancias[i][j];
                    *distancias[i][j] = *distancias[i + 1][j];
                    *distancias[i + 1][j] = temp[j];
                }
            }
        }
        numPasada--;
    }
}

bool knn (Juego datos, Juego dataset[], int n, int k, int y){
    double distancias[n][2];
    int si, no = 0;
    int x;

    for (int i=y;i<n;i++){//calculo distancias
        distancias[i][0]=distancia_juego(datos, dataset[i]);
        distancias[i][1]=i;
    }

    ordena(&(distancias), n);//ordeno distancias
    /*for (int i=0; i<20; i++){
        printf("distancia %d: %f; pos: %f\n", i, distancias[i][0], distancias[i][1]);
    }*/

    for (int j=0;j<k;j++){//calculo meidas etiquetas de las k primeras distancias
        if (dataset[(int)distancias[j][1]].exitoso){
            si++;
        } else {
            no++;
        }
    }

    printf("si: %d, no: %d\n", si, no);

    if (si>no){//calculo etiquetas datos
        datos.exitoso=true;
    } else if (si<no){
        datos.exitoso=false;
    } else {
        datos.exitoso=dataset[(int)distancias[0][1]].exitoso;
    }
    return datos.exitoso;
}

void ENN (Juego dataset[], int n, int k){
    for (int i=0; i<n; i++){
        if (knn(dataset[i], dataset, n, k, 0)!=dataset[i].exitoso){
            dataset[i].eliminar=true;
            printf("%d--", i+1);
        }
    }
    printf("fin\n");
}



void main(int argc, char **argv){
	Juego dataset[2218];
	float ventas_g, ventas_EEUU, ventas_EU, ventas_Japon, rating, ventas_gs[2191], ventas_EEUUs[2191], ventas_EUs[2191], ventas_Japons[2191], ratings[2191];
	char consola[100], publicador[100], desarrollador[100], salida[100], last[100], generos[100], plataformas[100];
    int i=0;
    int aciertos;
    bool predicciones[2191];


	FILE * archivo = fopen(argv[1], "r");

    while(fscanf(archivo, "%99[^;];%99[^;];%99[^;];%f;%f;%f;%f;%99[^;];%f;%99[^;];%99[^\n]\n", consola, publicador, desarrollador, &(ventas_g), &(ventas_EEUU), &(ventas_EU), &(ventas_Japon), salida, &(rating), generos, plataformas)!=EOF && i<2191){//como comprobar EOF
        
        ventas_gs[i]=ventas_g;
        ventas_EEUUs[i]=ventas_EEUU;
        ventas_EUs[i]=ventas_EU;
        ventas_Japons[i]=ventas_Japon;
        ratings[i]=rating; 

        
        
        dataset[i].consola=consola;
        dataset[i].publicador=publicador;
        dataset[i].desarrollador=desarrollador;
        dataset[i].fecha_sal=salida;
        dataset[i].genero=generos;
        dataset[i].plataforma=plataformas;


        //printf("consola: %s\npublicador: %s\ndesarrollador: %s\nventas: %f-%f-%f-%f\nsalida: %s\nrating%f\ngeneros: %s\nplataformas: %s\n%d\n", dataset[i].consola, dataset[i].publicador, dataset[i].desarrollador, (dataset[i].ventas_g), (dataset[i].ventas_EEUU), (dataset[i].ventas_EU), (dataset[i].ventas_Japon), dataset[i].fecha_sal, (dataset[i].rating), dataset[i].genero, dataset[i].plataforma, i);
        i++;
    }
    normalizar(ventas_gs, 2191);
    normalizar(ventas_EEUUs, 2191);
    normalizar(ventas_EUs, 2191);
    normalizar(ventas_Japons, 2191);
    normalizar(ratings, 2191);
    
    

    for (int i=0; i < 2191; i++){
        dataset[i].ventas_EEUU=ventas_EEUUs[i];
        dataset[i].ventas_g=ventas_gs[i];
        dataset[i].ventas_EU=ventas_EUs[i];
        dataset[i].ventas_Japon=ventas_Japons[i];
        dataset[i].rating=ratings[i];
    }

    set_label(dataset, 2191);
    knn(dataset[0], dataset, 2191, 20, 548);

    //ENN(dataset, 2191, 2191);//aplicar ENN (ya hecho)

    /*for (int i=0; i<=atoi(argv[2]); i++){
        aciertos=0;
        for (int j=0; j<548; j++){
            predicciones[j]=knn(dataset[j], dataset, 2191, i, 548);//knn devuelve todo el rato 1-->error!!!!!!!
        }
        for (int j=0; j<548; j++){
            if (predicciones[j]==dataset[j].exitoso){
                aciertos++;
            }
        }
        printf("proporción aciertos: %d, k=%d\n", aciertos/548, i);
    }*/

}
