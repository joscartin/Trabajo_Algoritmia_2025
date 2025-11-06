#include <stdbool.h>
/*¿cómo hacer diccionarios?*/
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
}juego;

typedef juego dataset[];