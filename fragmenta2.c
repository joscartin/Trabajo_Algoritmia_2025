#include "fragmenta2.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char **fragmenta2(char *cadena){
	char *copia1, *copia2, *cad, **lista;
	int cont;
	
	copia1 = (char *)malloc((strlen(cadena) + 1)*sizeof(char));
	strcpy(copia1, cadena);
	
	copia2 = (char *)malloc((strlen(cadena) + 1)*sizeof(char));
	strcpy(copia2, cadena);
	
	cont = 1;
	cad = strtok((char *)copia1, "-");
	while(cad != NULL){
		if(strlen(cad) > 0){
			cont++;
		}
		
		cad = strtok(NULL, "-");
	} 
	
	lista = (char **)malloc(cont*sizeof(char *));
	
	cont = 0;
	cad = strtok((char *)copia2, "-");
	while(cad != NULL){
		if(strlen(cad) > 0){
			lista[cont] = (char *)malloc((strlen(cad) + 1)*sizeof(char));
			strcpy(lista[cont], cad);
			cont++;
		}
		
		cad = strtok(NULL, "-");
	} 
	free(copia1);
	free(copia2);
	return lista;
}

void borrarg(char **arg){
	int i = 0;
	
	while(arg[i] != NULL){
		free(arg[i]);
		i++;
	}
	free(arg);
}























