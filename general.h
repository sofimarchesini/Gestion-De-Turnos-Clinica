#ifndef GENERAL_H
#define GENERAL_H

#include "heap.h"
#include "hash.h"
#include "cola.h"
#include "csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"



typedef struct general general_t;

general_t *general_crear(lista_t *doctores, lista_t *pacientes);

void general_encolar_paciente(general_t* general,char* nombre,char* especialidad,char* urgencia);

void general_atender_paciente(general_t* general,char* doctor);

void general_crear_informe_(general_t* general,char* inicio, char* fin);

void general_crear_informe(general_t* general,char* inicio,char* fin);

void general_destruir(general_t* general);

#endif