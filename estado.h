#include "heap.h"
#include "hash.h"
#include "cola.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paciente.h"

typedef struct estado estado_t;


estado_t* crear_estado();

void *estado_borrar(estado_t *estado);

bool estado_guardar(estado_t* estado,char* urgencia, paciente_t* paciente);

bool estado_esta_vacio(estado_t *estado);

size_t estado_pacientes_restantes(estado_t* estado);

void estado_destruir(estado_t* estado,void (*destruir_dato)(void *));