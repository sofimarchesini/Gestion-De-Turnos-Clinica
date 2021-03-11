
#include "strutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct paciente paciente_t;


paciente_t* paciente_crear(char* nombre,size_t anio);

char* paciente_devolver_nombre(const paciente_t* paciente);
size_t paciente_devolver_anio(const paciente_t* paciente);

void paciente_destruir(paciente_t* paciente);