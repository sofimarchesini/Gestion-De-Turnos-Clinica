#include "lista.h"
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct nodo{
	void* dato;
	struct nodo* siguiente;
}nodo_t;

struct lista{
	nodo_t *prim;
	nodo_t *ult;
	size_t largo;
};

struct lista_iter {
	lista_t* lista;
    nodo_t* anterior;
    nodo_t* actual;
};

/* ******************************************************************
 *               FUNCIONES AUXILIARES
 * *****************************************************************/

nodo_t *crear_nodo(void* dato){
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if(nodo==NULL) return NULL;
	nodo->dato = dato;
	nodo->siguiente = NULL;
	return nodo;
}

void destruir_nodo(nodo_t *nodo){
	free(nodo);
}

/* ******************************************************************
 *                DEFINICION DE LAS PRIMITIVAS DE LISTA
 * *****************************************************************/

lista_t* lista_crear(void){
	lista_t *lista = malloc(sizeof(lista_t));

    if (lista== NULL) return NULL;
     
    lista-> ult = lista->prim = NULL;
    lista->largo = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista){

	return  lista->prim == NULL;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){

	while (!lista_esta_vacia(lista)){
		void* dato = lista_borrar_primero(lista);
		if (destruir_dato){
			destruir_dato(dato);
	    }
	}
	free(lista);
}

bool lista_insertar_primero(lista_t *lista, void *dato){

    nodo_t *nodo = crear_nodo(dato);
    if (nodo==NULL) return false;
	if (lista_esta_vacia(lista)){
		lista->ult = nodo;
	}else{
		nodo->siguiente= lista->prim;
	}
	lista->prim=nodo;
	lista->largo++;
	return true;
}

void *lista_ver_primero(const lista_t *lista){

	if (lista_esta_vacia(lista)) return NULL;
	return lista->prim->dato;
}

void *lista_borrar_primero(lista_t *lista){

	if(lista_esta_vacia(lista)) return NULL;
	
	void* dato = lista->prim->dato;
	nodo_t *eliminado = lista->prim;

	lista->prim = lista->prim->siguiente;
	destruir_nodo(eliminado);
	lista->largo--;
    return dato;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
    
    nodo_t *nodo = crear_nodo(dato);
    if (nodo==NULL) return false;
	if (lista_esta_vacia(lista)){
		lista->prim = nodo;
	}else{
		lista->ult->siguiente = nodo;
	}
	lista->ult=nodo;
	lista->largo++;
	return true;
}

void *lista_ver_ultimo(const lista_t* lista){
	if (lista_esta_vacia(lista)) return NULL;
	return lista->ult->dato;
}

size_t lista_largo(const lista_t *lista){
	return lista->largo;
}

/* ******************************************************************
 *               DEFINICION DE PRIMITIVAS DE ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	if (lista_esta_vacia(lista)) return;

	nodo_t* actual = lista->prim;
	while (actual){
		if (!visitar(actual->dato,extra))return;
		actual = actual->siguiente;
	}
}

/* ******************************************************************
 *               DEFINICION DE PRIMITIVAS DE ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t *iter = malloc(sizeof(lista_iter_t));
	if (iter==NULL) return NULL;

	iter->lista = lista;
	iter->actual = lista->prim;
	iter->anterior = NULL;

	return iter;
}

bool lista_iter_al_final(const lista_iter_t *iter){

	return iter->actual==NULL;
}

bool lista_iter_avanzar(lista_iter_t *iter){

	if(lista_iter_al_final(iter)) return false;
	iter->anterior= iter->actual;
	iter->actual = iter->actual->siguiente;
	
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){

	if (lista_iter_al_final(iter)) return NULL;
	return iter->actual->dato;
}

void lista_iter_destruir(lista_iter_t *iter){

	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){

    nodo_t *nodo = crear_nodo(dato);
    if (nodo==NULL) return false;
    if (lista_iter_al_final(iter)){
    	iter->lista->ult = nodo;
    }
    if (iter->anterior){
		iter->anterior->siguiente = nodo;
	}
    else{
		iter->lista->prim = nodo;
	}
    nodo->siguiente = iter->actual;
	iter->actual = nodo;
	iter->lista->largo++;

	return true;
}

void *lista_iter_borrar(lista_iter_t *iter){

	if(lista_iter_al_final(iter))return NULL;
    void* valor = iter->actual->dato;
    nodo_t *nodo = iter->actual;

    if(iter->lista->prim == iter->actual){
    	iter->actual=iter->actual->siguiente;
        iter->lista->prim = iter->actual;
    }else{
    	if(iter->actual->siguiente ==  NULL){
    		iter->lista->ult = iter->anterior;
    	}
    	iter->anterior->siguiente = iter->actual->siguiente;
    	iter->actual = iter->anterior->siguiente;
    }
    destruir_nodo(nodo);
    iter->lista->largo --;
    return valor;
}  

