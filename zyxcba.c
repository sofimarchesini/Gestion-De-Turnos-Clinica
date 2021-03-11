#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "mensajes.h"
#include "hash.h"
#include <ctype.h>
#include "general.h"

#include "strutil.h"
#include "lista.h"
#include "csv.h"



#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

#define COSTANTE_PEDIR_TURNO 3
#define COSTANTE_ATENDER 1
#define COSTANTE_INFORME 2


int cant_parametros(char **parametros){
	int cant = 0;
	for(int i=0;parametros[i];i++) cant++;
	return cant;
}

void procesar_comando(general_t* general,const char* comando,char** parametros) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if(cant_parametros(parametros)!= COSTANTE_PEDIR_TURNO) printf(ENOENT_PARAMS,COMANDO_PEDIR_TURNO);
		else general_encolar_paciente(general,parametros[0],parametros[1],parametros[2]);
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if(cant_parametros(parametros)!= COSTANTE_ATENDER) printf(ENOENT_PARAMS,COMANDO_ATENDER);
		else general_atender_paciente(general,parametros[0]);
	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		if(cant_parametros(parametros)!= COSTANTE_INFORME) printf(ENOENT_PARAMS,COMANDO_INFORME);
		else general_crear_informe(general,parametros[0],parametros[1]);
	} else printf(ENOENT_CMD,comando);
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(general_t* general) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(general,campos[0], parametros);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

void wrapper_free_strv(void *strv){
	free_strv((char**)strv);
}

int main(int argc, char** argv) {

	if(argc != 3) {
		printf(ENOENT_CANT_PARAMS);
		return 1;
	}
	lista_t *doctores = csv_crear_estructura(argv[1], NULL, NULL);
	lista_t *pacientes = csv_crear_estructura(argv[2],NULL, NULL);
	general_t* general = general_crear(doctores, pacientes);
	if(!general) return 1;
	
	procesar_entrada(general);
	general_destruir(general);
	lista_destruir(doctores,wrapper_free_strv);
	lista_destruir(pacientes, wrapper_free_strv);
	return 0;
}
