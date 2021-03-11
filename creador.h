
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "general.h"
#include "abb.h"

hash_t *construir_sectores(lista_t *doctores);
abb_t *construir_doctores(lista_t *doctores);
hash_t *construir_personas(lista_t *pacientes);
abb_t *construir_atendidos(lista_t *doctores);