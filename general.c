
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mensajes.h"
#include "estado.h"
#include <ctype.h>
#include "general.h"
#include "creador.h"

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct general{
    hash_t* especialidades;
    hash_t* personas;
    abb_t* doctores;
    abb_t* atendidos;
};

/* ******************************************************************
 *                FUNCIONES AUXILIARES
 * *****************************************************************/

void imprimir_salida_atender_paciente(char* persona,char* especialidad,general_t* general){
    printf(PACIENTE_ATENDIDO,persona);
    size_t restantes = estado_pacientes_restantes((estado_t*)hash_obtener(general->especialidades,especialidad));
    printf(CANT_PACIENTES_ENCOLADOS,restantes,especialidad);
}

void imprimir_salida_encolar_paciente(char* persona,char* especialidad,general_t* general){
    printf(PACIENTE_ENCOLADO,persona);
    size_t restantes = estado_pacientes_restantes((estado_t*)hash_obtener(general->especialidades,especialidad));
    printf(CANT_PACIENTES_ENCOLADOS,restantes,especialidad);
}

bool imprimir_informe(const char* clave,void* dato,void* general,size_t* n){
    general_t* general_ = general;
    int atendidos_ = *(int*)abb_obtener(general_->atendidos,clave);
    *n+=1;
    printf(INFORME_DOCTOR,*n,clave,(char*)dato,(size_t)atendidos_);  
    return true;
}

bool contar_doctores(const char* clave,void* dato,void* extra,size_t* extra_2){
    size_t *cant_doctores = extra;
    *cant_doctores = *cant_doctores+1;
    return true;
}

/* ******************************************************************
 *                DEFINICION DE LAS PRIMITIVAS DE GENERAL
 * *****************************************************************/

general_t *general_crear(lista_t* doctores, lista_t *pacientes){
    general_t* general = malloc(sizeof(general_t));
    if(!general) return NULL;
    
    hash_t* especialidades = construir_sectores(doctores);
    if(!especialidades) return NULL;
	abb_t* abb_doctores = construir_doctores(doctores);
    if(!abb_doctores) return NULL;
	hash_t* personas = construir_personas(pacientes);
    if(!personas) return NULL;
	abb_t* atendidos = construir_atendidos(doctores);
    if (!atendidos) return NULL;
    
    general->doctores = abb_doctores;
    general->personas = personas;
    general->especialidades = especialidades;
    general->atendidos = atendidos;

    return general;
}

void general_encolar_paciente(general_t* general,char* nombre,char* especialidad,char* urgencia){
    if(!hash_pertenece(general->especialidades,especialidad)) printf(ENOENT_ESPECIALIDAD,especialidad);
    else if (!hash_pertenece(general->personas,nombre)) printf(ENOENT_PACIENTE,nombre);
    else{
        char* anio_ = hash_obtener(general->personas,nombre);
        int anio = atoi(anio_);

        paciente_t* info_paciente = paciente_crear(nombre,(size_t )anio);
        if(!info_paciente) return;
        estado_t* estado = (estado_t*)(hash_obtener(general->especialidades,especialidad));
        if(!estado_guardar(estado,urgencia,info_paciente)) printf(ENOENT_NO_ENCOLADO,nombre);
        else imprimir_salida_encolar_paciente(nombre, especialidad, general);
        
    }
}

void general_atender_paciente(general_t* general,char* doctor){
    //void* valor = (void*)1;
    if(!abb_pertenece(general->doctores,doctor)) printf(ENOENT_DOCTOR,doctor);
    else{
        char* especialidad = (char*)abb_obtener(general->doctores,doctor);
        paciente_t* paciente = (paciente_t*)(estado_borrar((estado_t*)(hash_obtener(general->especialidades,especialidad))));
        if(paciente) {
            char* nombre = paciente_devolver_nombre(paciente);
            //if(abb_obtener(general->atendidos,doctor)==0){
                //abb_guardar(general->atendidos,doctor,valor);
                //int* cant = malloc(sizeof(int));
            (*(int*)abb_obtener(general->atendidos,doctor))++;
                //*cant+=1;
                //abb_guardar(general->atendidos,doctor,cant);
            imprimir_salida_atender_paciente(nombre,especialidad,general);
            paciente_destruir(paciente);
        }
        else printf(SIN_PACIENTES);
    }
}


void general_crear_informe(general_t* general,char* inicio,char* fin){
    bool inicio_ = false;
    bool fin_ = false;
    if(!strcmp(inicio,"")) {
        inicio_ = true;
        inicio = substr("A",2);
    }
    if(!strcmp(fin,"")){
        fin = substr("z",2);
        fin_ = true;
    }
    size_t cant_doctores = 0;
    abb_in_rango(general->doctores, contar_doctores,&cant_doctores, inicio, fin);
    printf(DOCTORES_SISTEMA,cant_doctores);
    abb_in_rango(general->doctores, imprimir_informe,( void*)general,inicio, fin);
    if(inicio_)free(inicio);
    if(fin_)free(fin);
}

void general_destruir(general_t* general){
    abb_destruir(general->doctores);
    hash_destruir(general->personas);
    abb_destruir(general->atendidos);
    hash_destruir(general->especialidades);
    free(general);

}