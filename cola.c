#include "cola.h"
#include <stdlib.h>
#include <stddef.h>

typedef struct nodo{
	void* dato;
	struct nodo* siguiente;
}nodo_t;

struct cola{
	nodo_t *prim;
	nodo_t *ult;
	size_t cant;
};

nodo_t *crear_nodo_cola(void* dato){
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (nodo==NULL) return NULL;
	nodo->dato = dato;
	nodo->siguiente = NULL;
	return nodo;
}

void destruir_nodo_cola(nodo_t *nodo){
	free(nodo);
}

cola_t *cola_crear(void){
	cola_t *cola = malloc(sizeof(cola_t));

    if (cola== NULL) return NULL;
    cola-> ult = cola->prim = NULL;
	cola->cant = 0;
    return cola;
}

bool cola_esta_vacia(const cola_t *cola){

	return cola->prim == NULL;
}

size_t cola_cantidad(const cola_t* cola){
	return cola->cant;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){

	while (!cola_esta_vacia(cola)){
		void* dato = cola_desencolar(cola);
		if (destruir_dato){
			destruir_dato(dato);
	    }
	}
	free(cola);
}

bool cola_encolar(cola_t *cola, void *valor){

    nodo_t *nodo = crear_nodo_cola(valor);
    if (nodo==NULL) return NULL;
	if (cola_esta_vacia(cola))cola->prim = nodo;
	else{
		cola->ult->siguiente = nodo;
	}
	cola->cant++;
	cola->ult = nodo;
	return true;
}

void *cola_ver_primero(const cola_t *cola){
    
	if (cola_esta_vacia(cola))return NULL;
	return cola->prim->dato;
}

void *cola_desencolar(cola_t *cola){

	if (cola_esta_vacia(cola)) return NULL;

	void* dato = cola->prim->dato;
	nodo_t *eliminado = cola->prim;
	cola->prim = cola->prim->siguiente;
	destruir_nodo_cola(eliminado);
	cola->cant--;
    return dato;
}
 
