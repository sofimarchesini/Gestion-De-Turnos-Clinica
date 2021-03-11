#include "csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estado.h"
#include "creador.h"
#include "strutil.h"

void destruir_paciente(void* paciente_){
    paciente_t* paciente = paciente_;
    if(paciente) paciente_destruir(paciente);
}
void destruir_estado(void *estado){
    estado_destruir((estado_t*)estado,destruir_paciente);
}

void destruir_dato_abb(void* dato){
    if(*(int*)dato == 0) free(dato);
}

hash_t *construir_sectores(lista_t *doctores){
    lista_iter_t *iter = lista_iter_crear(doctores);
    if (!iter) return NULL;
    hash_t *especialidades = hash_crear(destruir_estado);
    if (!especialidades){
        lista_iter_destruir(iter);
        return NULL;
    }
    while(!lista_iter_al_final(iter)){
        char **especialidad = lista_iter_ver_actual(iter);
        if (!hash_obtener(especialidades, especialidad[1])){
            estado_t *estado = crear_estado();
            hash_guardar(especialidades, especialidad[1], estado);
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return especialidades;
}


abb_t *construir_doctores(lista_t *doctores){
    lista_iter_t *iter = lista_iter_crear(doctores);
    if (!iter) return NULL;
    abb_t *doctores_abb = abb_crear(strcmp, NULL);
    if (!doctores_abb){
        lista_iter_destruir(iter);
        return NULL;
    }
    while(!lista_iter_al_final(iter)){
        char **doctor = (char**)lista_iter_ver_actual(iter);
        abb_guardar(doctores_abb, doctor[0], doctor[1]);
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return doctores_abb;
}

hash_t *construir_personas(lista_t *pacientes){
    lista_iter_t *iter = lista_iter_crear(pacientes);
    if (!iter) return NULL;
    hash_t *pacientes_hash = hash_crear(NULL);
    if (!pacientes_hash){
        lista_iter_destruir(iter);
        return NULL;
    }
    while(!lista_iter_al_final(iter)){
        char **paciente = lista_iter_ver_actual(iter);
        hash_guardar(pacientes_hash, paciente[0],paciente[1]);
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return pacientes_hash;
}

abb_t *construir_atendidos(lista_t *doctores){
    lista_iter_t *iter = lista_iter_crear(doctores);
    if (!iter) return NULL;
    abb_t *atendidos = abb_crear(strcmp,free);
    if (!atendidos){
        lista_iter_destruir(iter);
        return NULL;
    }
    while(!lista_iter_al_final(iter)){
        int* atendidos_inicial = malloc(sizeof(int));
        if (!atendidos_inicial) {
            lista_iter_destruir(iter);
            return NULL;
        }
        *atendidos_inicial=0;
        char **doctor = (char**)lista_iter_ver_actual(iter);
        abb_guardar(atendidos, doctor[0], atendidos_inicial);
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return atendidos;
}

