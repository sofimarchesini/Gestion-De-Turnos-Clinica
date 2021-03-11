
#include "paciente.h"

struct paciente{
    char* nombre;
    size_t anio;
};

paciente_t* paciente_crear(char* nombre,size_t anio){
    paciente_t* paciente = malloc(sizeof(paciente_t));
    if (!paciente) return NULL;
    
    char *nombre_copiado = malloc(strlen(nombre) + 1);
    if (nombre_copiado == NULL) return NULL;
    strcpy(nombre_copiado, nombre);
    paciente->nombre = nombre_copiado;
    paciente->anio = anio;

    return paciente;
}


char* paciente_devolver_nombre(const paciente_t* paciente){
    if (!paciente) return NULL;
    return paciente->nombre;
}

size_t paciente_devolver_anio(const paciente_t* paciente){
    return paciente->anio;
}

void paciente_destruir(paciente_t* paciente){
    free(paciente->nombre);
    free(paciente);
}