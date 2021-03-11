#include "pila.h"

#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
// ...

#define CAPACIDAD_INICIAL 10
#define CANTIDAD_INICIAL 0
#define FACTOR_REDIMENSION 2

pila_t *pila_crear(void){

	pila_t *pila = malloc(sizeof(pila_t));

    if (pila == NULL) return NULL;

    pila->capacidad = CAPACIDAD_INICIAL;
    pila-> cantidad = CANTIDAD_INICIAL;
    pila->datos = malloc(pila->capacidad*sizeof(void*));

    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    return pila;
}

void pila_destruir(pila_t *pila){

	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
 
    return (pila->cantidad == 0);
}

bool redimensionar(pila_t *pila,size_t nueva_capacidad){

    void** datos_aux = realloc(pila->datos, nueva_capacidad * sizeof(void*));
    
    if (datos_aux == NULL) return false;
    pila->datos = datos_aux;
    pila->capacidad = nueva_capacidad;

    return true;
}

bool pila_apilar(pila_t *pila, void *valor){

    if (pila->capacidad == pila->cantidad){
        redimensionar(pila,pila->capacidad*FACTOR_REDIMENSION);
	} 

    pila->cantidad++;
    
	pila->datos[pila->cantidad-1]= valor;
	return true;
}   

void *pila_ver_tope(const pila_t *pila){
   
    if (pila_esta_vacia(pila))return NULL;
    return (pila->datos[pila->cantidad-1]);  
}

void *pila_desapilar(pila_t *pila){

	if (pila_esta_vacia(pila)) return NULL;
    
    void* tope = pila_ver_tope(pila);
    
    if (pila->capacidad == pila->cantidad*4){
        redimensionar(pila,pila->capacidad/FACTOR_REDIMENSION);
	} 
    pila->cantidad --;
    return tope;
}
