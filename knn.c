#include <stdbool.h>
#include <math.h>

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

void set_label(Dataset datas, int n){
    for (int i=0;i<n;i++){
        if (datas[i].ventas_g<10000000){
            datas[i].exitoso=false;
        } else {
            datas[i].exitoso=true;
        }
    }
}


double distancia_juego(Juego a, Juego b) {
    double suma = 0.0;
    double d; // distancia parcial

    // 1 Consola, Publicador, Desarrollador → categóricos (0 o 1)
    if (a.consola != b.consola)  suma += 1;
    if (a.publicador != b.publicador)  suma += 1;
    if (a.desarrollador != b.desarrollador)  suma += 1;

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

void ordena(int distancias[][2], int size) {
    bool intercambios = true;
    int numPasada = size - 1;
    int temp[2];  // Para almacenar temporalmente una fila (par de números)
    
    while (numPasada > 0 && intercambios) {
        intercambios = false;
        for (int i = 0; i < numPasada; i++) {
            // Comparamos solo el primer valor de cada fila (distancias[i][0])
            if (distancias[i][0] > distancias[i + 1][0]) {
                intercambios = true;
                // Intercambiamos las filas completas (todas las columnas de la fila)
                for (int j = 0; j < 2; j++) {
                    temp[j] = distancias[i][j];
                    distancias[i][j] = distancias[i + 1][j];
                    distancias[i + 1][j] = temp[j];
                }
            }
        }
        numPasada--;
    }
}

bool knn (Juego datos, Dataset dataset, int n, int k){
    Distancias distancias;
    int si, no = 0;
    int x;

    for (int i=0;i<n;i++){//calculo distancias
        distancias[i][0]=distancia_juego(datos, dataset[i]);
        distancias[i][1]=i;
    }

    ordena(distancias);//ordeno distancias

    for (int j=0;j<k;j++){//calculo meida etiquetas de las k primeras distancias
        if (dataset[distancias[0][1]].exitoso){
            si++;
        } else {
            no++;
        }
    }

    if (si>no){//calculo etiquetas datos
        datos.exitoso=true;
    } else if (si<no){
        datos.exitoso=false;
    } else {
        datos.exitoso=dataset[distancias[0][1]].exitoso;
    }
    return datos.exitoso;
}

void ENN (Dataset dataset, int n, int k){
    for (int i=0; i<n; i++){        
        if (knn(dataset[i], dataset, n, k)){
            for (int j=i+1;j<n;j++){
                dataset[j-1]=dataset[j];
            }
        }
    }
}

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


typedef struct juego{
    Consola consola;
    Publicador publicador;
    Desarrollador desarrollador;
    Genero genero;
    Plataforma plataforma;
    fecha fecha_sal;
    fecha ult_update;
    int rating;
    int ventas_g;
    int ventas_EEUU;
    int ventas_EU;
    int ventas_Japon;
    bool exitoso;
}juego;

typedef juego dataset[];
dataset juegos;

    // Juego 0: PS3 Grand Theft Auto V
    juegos[0].consola = 0;  // PS3
    juegos[0].publicador = 0;
    juegos[0].desarrollador = 0;
    juegos[0].genero[0] = 0;  // Acción
    juegos[0].plataforma[6] = 1;  // PS3
    juegos[0].fecha_sal.dia = 17;
    juegos[0].fecha_sal.mes = 9;
    juegos[0].fecha_sal.año = 2013;
    juegos[0].rating = 4.47;
    juegos[0].ventas_g = 20320000;
    juegos[0].ventas_EEUU = 6370000;
    juegos[0].ventas_EU = 9850000;
    juegos[0].ventas_Japon = 990000;
    juegos[0].exitoso = true;

    // Juego 1: PS4 Grand Theft Auto V
    juegos[1].consola = 1;
    juegos[1].publicador = 0;
    juegos[1].desarrollador = 0;
    juegos[1].genero[0] = 0;  //
    juegos[1].plataforma[3] = 1;
    juegos[1].fecha_sal.dia = 18;
    juegos[1].fecha_sal.mes = 11;
    juegos[1].fecha_sal.año = 2014;
    juegos[1].ult_update.dia = 3;
    juegos[1].ult_update.mes = 1;
    juegos[1].ult_update.año = 2018;
    juegos[1].rating = 4.47;
    juegos[1].ventas_g = 19390000;
    juegos[1].ventas_EEUU = 6060000;
    juegos[1].ventas_EU = 9710000;
    juegos[1].ventas_Japon = 600000;
    juegos[1].exitoso = true;

    // Juego 2: PS4 Call of Duty: Black Ops 3
    juegos[2].consola = 1;
    juegos[2].publicador = 1;
    juegos[2].desarrollador = 1;
    juegos[2].genero[1] = 1;
    juegos[2].plataforma[3] = 1;
    juegos[2].fecha_sal.dia = 6;
    juegos[2].fecha_sal.mes = 11;
    juegos[2].fecha_sal.año = 2015;
    juegos[2].rating = 0;  // Aún no se proporciona
    juegos[2].ventas_g = 15090000;
    juegos[2].ventas_EEUU = 6180000;
    juegos[2].ventas_EU = 6050000;
    juegos[2].ventas_Japon = 410000;
    juegos[2].exitoso = false;

    // Juego 3: PS4 Red Dead Redemption 2
    juegos[3].consola = 1;  // PS4
    juegos[3].publicador = 0;
    juegos[3].desarrollador = 0;
    juegos[3].genero[0] = 0;
    juegos[3].plataforma[3] = 1;
    juegos[3].fecha_sal.dia = 26;
    juegos[3].fecha_sal.mes = 10;
    juegos[3].fecha_sal.año = 2018;
    juegos[3].ult_update.dia = 2;
    juegos[3].ult_update.mes = 11;
    juegos[3].ult_update.año = 2018;
    juegos[3].rating = 4.59;
    juegos[3].ventas_g = 13940000;
    juegos[3].ventas_EEUU = 5260000;
    juegos[3].ventas_EU = 6210000;
    juegos[3].ventas_Japon = 210000;
    juegos[3].exitoso = true;

    // Juego 4: PS3 Call of Duty: Black Ops II
    juegos[4].consola = 0;
    juegos[4].publicador = 1;
    juegos[4].desarrollador = 1;
    juegos[4].genero[1] = 1;
    juegos[4].plataforma[6] = 1;
    juegos[4].fecha_sal.dia = 13;
    juegos[4].fecha_sal.mes = 11;
    juegos[4].fecha_sal.año = 2012;
    juegos[4].rating = 3.96;
    juegos[4].ventas_g = 13800000;
    juegos[4].ventas_EEUU = 4990000;
    juegos[4].ventas_EU = 5880000;
    juegos[4].ventas_Japon = 650000;
    juegos[4].exitoso = true;


    // Juego 5: PS3 Call of Duty: Black Ops
    juegos[5].consola = 0;
    juegos[5].publicador = 1;
    juegos[5].desarrollador = 1;
    juegos[5].genero[1] = 1;  // Shooter
    juegos[5].plataforma[6] = 1;  // PS3
    juegos[5].fecha_sal.dia = 9;
    juegos[5].fecha_sal.mes = 11;
    juegos[5].fecha_sal.año = 2010;
    juegos[5].rating = 4.05;
    juegos[5].ventas_g = 12670000;
    juegos[5].ventas_EEUU = 6010000;
    juegos[5].ventas_EU = 4400000;
    juegos[5].ventas_Japon = 480000;
    juegos[5].exitoso = true;

    // Juego 6: PS4 FIFA 18
    juegos[6].consola = 1;
    juegos[6].publicador = 2;  // EA Sports
    juegos[6].desarrollador = 4;  // EA Vancouver
    juegos[6].genero[2] = 2;  // Sports
    juegos[6].plataforma[3] = 1;  // PS4
    juegos[6].fecha_sal.dia = 29;
    juegos[6].fecha_sal.mes = 9;
    juegos[6].fecha_sal.año = 2017;
    juegos[6].ult_update.dia = 2;
    juegos[6].ult_update.mes = 4;
    juegos[6].ult_update.año = 2018;
    juegos[6].rating = 3.36;
    juegos[6].ventas_g = 11800000;
    juegos[6].ventas_EEUU = 1270000;
    juegos[6].ventas_EU = 8640000;
    juegos[6].ventas_Japon = 150000;
    juegos[6].exitoso = true;

     // Juego 7: PS4 FIFA 17
    juegos[7].consola = 1;  // PS4
    juegos[7].publicador = 2;  // Electronic Arts
    juegos[7].desarrollador = 4;  // EA Canada
    juegos[7].genero[2] = 2;  // Sports
    juegos[7].plataforma[3] = 1;  // PS4
    juegos[7].fecha_sal.dia = 27;
    juegos[7].fecha_sal.mes = 9;
    juegos[7].fecha_sal.año = 2016;
    juegos[7].ult_update.dia = 28;
    juegos[7].ult_update.mes = 3;
    juegos[7].ult_update.año = 2018;
    juegos[7].rating = 3.31;
    juegos[7].ventas_g = 10940000;
    juegos[7].ventas_EEUU = 1260000;
    juegos[7].ventas_EU = 7950000;
    juegos[7].ventas_Japon = 120000;
    juegos[7].exitoso = true;

    // Juego 8: PS3 Call of Duty: Modern Warfare 2
    juegos[8].consola = 0;  // PS3
    juegos[8].publicador = 1;  // Activision
    juegos[8].desarrollador = 3;  // Infinity Ward
    juegos[8].genero[1] = 1;  // Shooter
    juegos[8].plataforma[6] = 1;  // PS3
    juegos[8].fecha_sal.dia = 10;
    juegos[8].fecha_sal.mes = 11;
    juegos[8].fecha_sal.año = 2009;
    juegos[8].rating = 4.27;
    juegos[8].ventas_g = 10610000;
    juegos[8].ventas_EEUU = 4990000;
    juegos[8].ventas_EU = 3660000;
    juegos[8].ventas_Japon = 380000;
    juegos[8].exitoso = true;

    // Juego 9: PS3 Grand Theft Auto IV
    juegos[9].consola = 0;  // PS3
    juegos[9].publicador = 0;  // Rockstar Games
    juegos[9].desarrollador = 0;  // Rockstar North
    juegos[9].genero[0] = 0;  // Acción
    juegos[9].plataforma[6] = 1;  // PS3
    juegos[9].fecha_sal.dia = 29;
    juegos[9].fecha_sal.mes = 4;
    juegos[9].fecha_sal.año = 2008;
    juegos[9].rating = 4.26;
    juegos[9].ventas_g = 10570000;
    juegos[9].ventas_EEUU = 4790000;
    juegos[9].ventas_EU = 3730000;
    juegos[9].ventas_Japon = 440000;
    juegos[9].exitoso = true;

    // Juego 10: PS4 Uncharted 4: A Thief's End
    juegos[10].consola = 1;  // PS4
    juegos[10].publicador = 3;  // Sony Interactive Entertainment
    juegos[10].desarrollador = 6;  // Naughty Dog
    juegos[10].genero[5] = 5;  // Aventura
    juegos[10].plataforma[3] = 1;  // PS4
    juegos[10].fecha_sal.dia = 10;
    juegos[10].fecha_sal.mes = 5;
    juegos[10].fecha_sal.año = 2016;
    juegos[10].ult_update.dia = 23;
    juegos[10].ult_update.mes = 9;
    juegos[10].ult_update.año = 2018;
    juegos[10].rating = 4.34;
    juegos[10].ventas_g = 10330000;
    juegos[10].ventas_EEUU = 4490000;
    juegos[10].ventas_EU = 3930000;
    juegos[10].ventas_Japon = 210000;
    juegos[10].exitoso = true;

    // Juego 11: PS3 Call of Duty: Ghosts
    juegos[11].consola = 0;  // PS3
    juegos[11].publicador = 1;  // Activision
    juegos[11].desarrollador = 3;  // Infinity Ward
    juegos[11].genero[1] = 1;  // Shooter
    juegos[11].plataforma[6] = 1;  // PS3
    juegos[11].fecha_sal.dia = 5;
    juegos[11].fecha_sal.mes = 11;
    juegos[11].fecha_sal.año = 2013;
    juegos[11].rating = 3.15;
    juegos[11].ventas_g = 10130000;
    juegos[11].ventas_EEUU = 4110000;
    juegos[11].ventas_EU = 4010000;
    juegos[11].ventas_Japon = 390000;
    juegos[11].exitoso = true;

    // Juego 12: PS4 Call of Duty: Black Ops IIII
    juegos[12].consola = 1;  // PS4
    juegos[12].publicador = 1;  // Activision
    juegos[12].desarrollador = 2;  // Treyarch
    juegos[12].genero[1] = 1;  // Shooter
    juegos[12].plataforma[3] = 1;  // PS4
    juegos[12].fecha_sal.dia = 12;
    juegos[12].fecha_sal.mes = 10;
    juegos[12].fecha_sal.año = 2018;
    juegos[12].ult_update.dia = 7;
    juegos[12].ult_update.mes = 11;
    juegos[12].ult_update.año = 2018;
    juegos[12].rating = 3.77;
    juegos[12].ventas_g = 9320000;
    juegos[12].ventas_EEUU = 4050000;
    juegos[12].ventas_EU = 3280000;
    juegos[12].ventas_Japon = 500000;
    juegos[12].exitoso = true;

        // Juego 13: PS4 FIFA 19
    juegos[13].consola = 1;  // PS4
    juegos[13].publicador = 2;  // Electronic Arts
    juegos[13].desarrollador = 7;  // EA Sports
    juegos[13].genero[2] = 2;  // Sports
    juegos[13].plataforma[3] = 1;  // PS4
    juegos[13].fecha_sal.dia = 28;
    juegos[13].fecha_sal.mes = 9;
    juegos[13].fecha_sal.año = 2018;
    juegos[13].ult_update.dia = 12;
    juegos[13].ult_update.mes = 9;
    juegos[13].ult_update.año = 2018;
    juegos[13].rating = 3.31;
    juegos[13].ventas_g = 9150000;
    juegos[13].ventas_EEUU = 840000;
    juegos[13].ventas_EU = 6870000;
    juegos[13].ventas_Japon = 100000;
    juegos[13].exitoso = true;

    // Juego 14: PS4 Marvel's Spider-Man
    juegos[14].consola = 1;  // PS4
    juegos[14].publicador = 3;  // Sony Interactive Entertainment
    juegos[14].desarrollador = 8;  // Insomniac Games
    juegos[14].genero[0] = 0;  // Acción
    juegos[14].plataforma[3] = 1;  // PS4
    juegos[14].fecha_sal.dia = 7;
    juegos[14].fecha_sal.mes = 9;
    juegos[14].fecha_sal.año = 2018;
    juegos[14].ult_update.dia = 26;
    juegos[14].ult_update.mes = 9;
    juegos[14].ult_update.año = 2018;
    juegos[14].rating = 4.45;
    juegos[14].ventas_g = 8760000;
    juegos[14].ventas_EEUU = 3640000;
    juegos[14].ventas_EU = 3390000;
    juegos[14].ventas_Japon = 320000;
    juegos[14].exitoso = true;

    // Juego 15: PS4 Call of Duty: Infinite Warfare
    juegos[15].consola = 1;  // PS4
    juegos[15].publicador = 1;  // Activision
    juegos[15].desarrollador = 3;  // Infinity Ward
    juegos[15].genero[1] = 1;  // Shooter
    juegos[15].plataforma[3] = 1;  // PS4
    juegos[15].fecha_sal.dia = 4;
    juegos[15].fecha_sal.mes = 11;
    juegos[15].fecha_sal.año = 2016;
    juegos[15].ult_update.dia = 14;
    juegos[15].ult_update.mes = 1;
    juegos[15].ult_update.año = 2018;
    juegos[15].rating = 3.58;
    juegos[15].ventas_g = 8480000;
    juegos[15].ventas_EEUU = 3110000;
    juegos[15].ventas_EU = 3830000;
    juegos[15].ventas_Japon = 190000;
    juegos[15].exitoso = true;

    // Juego 16: PS4 Fallout 4
    juegos[16].consola = 1;  // PS4
    juegos[16].publicador = 4;  // Bethesda Softworks
    juegos[16].desarrollador = 9;  // Bethesda Game Studios
    juegos[16].genero[0] = 0;  // Acción
    juegos[16].plataforma[3] = 1;  // PS4
    juegos[16].fecha_sal.dia = 10;
    juegos[16].fecha_sal.mes = 11;
    juegos[16].fecha_sal.año = 2015;
    juegos[16].ult_update.dia = 24;
    juegos[16].ult_update.mes = 3;
    juegos[16].ult_update.año = 2018;
    juegos[16].rating = 3.81;
    juegos[16].ventas_g = 8480000;
    juegos[16].ventas_EEUU = 2910000;
    juegos[16].ventas_EU = 3970000;
    juegos[16].ventas_Japon = 270000;
    juegos[16].exitoso = true;

    // Juego 17: PS4 FIFA 16
    juegos[17].consola = 1;  // PS4
    juegos[17].publicador = 2;  // EA Sports
    juegos[17].desarrollador = 4;  // EA Canada
    juegos[17].genero[2] = 2;  // Sports
    juegos[17].plataforma[3] = 1;  // PS4
    juegos[17].fecha_sal.dia = 22;
    juegos[17].fecha_sal.mes = 9;
    juegos[17].fecha_sal.año = 2015;
    juegos[17].ult_update.dia = 1;
    juegos[17].ult_update.mes = 4;
    juegos[17].ult_update.año = 2018;
    juegos[17].rating = 3.37;
    juegos[17].ventas_g = 8220000;
    juegos[17].ventas_EEUU = 1150000;
    juegos[17].ventas_EU = 5770000;
    juegos[17].ventas_Japon = 70000;
    juegos[17].exitoso = true;

        // Juego 18: PS4 Star Wars Battlefront (2015)
    juegos[18].consola = 1;  // PS4
    juegos[18].publicador = 2;  // Electronic Arts
    juegos[18].desarrollador = 5;  // EA DICE
    juegos[18].genero[1] = 1;  // Shooter
    juegos[18].plataforma[3] = 1;  // PS4
    juegos[18].fecha_sal.dia = 17;
    juegos[18].fecha_sal.mes = 11;
    juegos[18].fecha_sal.año = 2015;
    juegos[18].ult_update.dia = 24;
    juegos[18].ult_update.mes = 3;
    juegos[18].ult_update.año = 2018;
    juegos[18].rating = 3.53;
    juegos[18].ventas_g = 8030000;
    juegos[18].ventas_EEUU = 3310000;
    juegos[18].ventas_EU = 3190000;
    juegos[18].ventas_Japon = 230000;
    juegos[18].exitoso = true;

    // Juego 19: PS3 FIFA 13
    juegos[19].consola = 0;  // PS3
    juegos[19].publicador = 2;  // EA Sports
    juegos[19].desarrollador = 4;  // EA Canada
    juegos[19].genero[2] = 2;  // Sports
    juegos[19].plataforma[6] = 1;  // PS3
    juegos[19].fecha_sal.dia = 25;
    juegos[19].fecha_sal.mes = 9;
    juegos[19].fecha_sal.año = 2012;
    juegos[19].rating = 3.75;
    juegos[19].ventas_g = 8010000;
    juegos[19].ventas_EEUU = 1070000;
    juegos[19].ventas_EU = 5130000;
    juegos[19].ventas_Japon = 130000;
    juegos[19].exitoso = true;

    // Juego 20: PS4 Call of Duty: Advanced Warfare
    juegos[20].consola = 1;  // PS4
    juegos[20].publicador = 1;  // Activision
    juegos[20].desarrollador = 2;  // Sledgehammer Games
    juegos[20].genero[1] = 1;  // Shooter
    juegos[20].plataforma[3] = 1;  // PS4
    juegos[20].fecha_sal.dia = 4;
    juegos[20].fecha_sal.mes = 11;
    juegos[20].fecha_sal.año = 2014;
    juegos[20].ult_update.dia = 4;
    juegos[20].ult_update.mes = 1;
    juegos[20].ult_update.año = 2018;
    juegos[20].rating = 3.47;
    juegos[20].ventas_g = 7530000;
    juegos[20].ventas_EEUU = 2840000;
    juegos[20].ventas_EU = 3340000;
    juegos[20].ventas_Japon = 140000;
    juegos[20].exitoso = true;

    // Juego 21: PS4 Battlefield 1
    juegos[21].consola = 1;  // PS4
    juegos[21].publicador = 2;  // Electronic Arts
    juegos[21].desarrollador = 5;  // EA DICE
    juegos[21].genero[1] = 1;  // Shooter
    juegos[21].plataforma[3] = 1;  // PS4
    juegos[21].fecha_sal.dia = 21;
    juegos[21].fecha_sal.mes = 10;
    juegos[21].fecha_sal.año = 2016;
    juegos[21].ult_update.dia = 24;
    juegos[21].ult_update.mes = 3;
    juegos[21].ult_update.año = 2018;
    juegos[21].rating = 3.84;
    juegos[21].ventas_g = 7260000;
    juegos[21].ventas_EEUU = 2200000;
    juegos[21].ventas_EU = 3650000;
    juegos[21].ventas_Japon = 290000;
    juegos[21].exitoso = true;

    // Juego 22: PS3 Battlefield 3
    juegos[22].consola = 0;  // PS3
    juegos[22].publicador = 2;  // Electronic Arts
    juegos[22].desarrollador = 5;  // DICE
    juegos[22].genero[1] = 1;  // Shooter
    juegos[22].plataforma[6] = 1;  // PS3
    juegos[22].fecha_sal.dia = 25;
    juegos[22].fecha_sal.mes = 10;
    juegos[22].fecha_sal.año = 2011;
    juegos[22].rating = 4.07;
    juegos[22].ventas_g = 7210000;
    juegos[22].ventas_EEUU = 2860000;
    juegos[22].ventas_EU = 2940000;
    juegos[22].ventas_Japon = 350000;
    juegos[22].exitoso = true;





