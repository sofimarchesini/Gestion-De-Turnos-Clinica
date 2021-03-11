#include "csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mensajes.h"
#include "estado.h"

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct estado{
    heap_t* regulares;
    cola_t* urgentes;
};

int anio_comparacion(const paciente_t* a, const paciente_t* b){
    
    size_t anio_a = paciente_devolver_anio(a);
    size_t anio_b = paciente_devolver_anio(b);
	if ( anio_a == anio_b) return 0;
	if ( anio_a < anio_b) return 1;
	return -1;
}


/* ******************************************************************
 *                DEFINICION DE LAS PRIMITIVAS DE ESTADO
 * *****************************************************************/

estado_t* crear_estado(){
    estado_t* estado = malloc(sizeof(estado_t));
    if(!estado) return NULL;
    heap_t* heap = heap_crear((cmp_func_t)anio_comparacion);
    if (!heap) return NULL;
    cola_t* cola = cola_crear();
    if(!cola){
        heap_destruir(heap,NULL);
        return NULL;
        }
    estado->regulares = heap;
    estado-> urgentes = cola;
    return estado;
}

void *estado_borrar(estado_t *estado){
    if (cola_esta_vacia(estado->urgentes) && heap_esta_vacio(estado->regulares)) return NULL;
    if (!cola_esta_vacia(estado->urgentes)) return cola_desencolar(estado->urgentes);
    return heap_desencolar(estado->regulares);
}   

bool estado_guardar(estado_t* estado,char* urgencia,paciente_t* paciente){
    if(!paciente) return false;
    if (strcmp(urgencia,"URGENTE")==0) return cola_encolar(estado->urgentes,paciente);
    if (strcmp(urgencia,"REGULAR")==0) return heap_encolar(estado->regulares,paciente);
    printf(ENOENT_URGENCIA,urgencia);
    return false;
}

bool estado_esta_vacio(estado_t *estado){
    return (heap_esta_vacio(estado->regulares) && cola_esta_vacia(estado->urgentes));
}

size_t estado_pacientes_restantes(estado_t* estado){
    return (heap_cantidad(estado->regulares)+cola_cantidad(estado->urgentes));
}

void estado_destruir(estado_t* estado,void (*destruir_dato)(void *)){
    heap_destruir(estado->regulares, destruir_dato);
    cola_destruir(estado->urgentes, destruir_dato);
    free(estado);
}

