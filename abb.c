#define _POSIX_C_SOURCE 200809L 
#include <string.h>
#include <stdlib.h>
#include "abb.h"
#include "pila.h"

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct nodo{
    struct nodo* izq;
    struct nodo* der; 
    char* clave;
    void* dato;
}nodo_t;

struct abb{
    nodo_t* raiz;
	size_t cant;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
};

struct abb_iter{
	const abb_t* abb;
    pila_t* pila;
};

/* ******************************************************************
 *                FUNCIONES AUXILIARES
 * *****************************************************************/

nodo_t *nodo_crear(const char* clave, void* dato){
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;
    char *clave_dup = strdup(clave);
    if (!clave_dup){
        free(nodo);
        return NULL;
    }
    nodo->clave = clave_dup;
    nodo->dato = dato;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

void *destruir_nodo_abb(nodo_t *nodo){
    void *aux = nodo->dato;
    free(nodo->clave);
	free(nodo);
    return aux;
}

nodo_t *buscar_padre(nodo_t *nodo,const char *clave,abb_comparar_clave_t cmp){

    if (cmp(nodo->clave, clave) == 0) return NULL;
    if ((nodo->der && !cmp(nodo->der->clave, clave)) || (nodo->izq && !cmp(nodo->izq->clave, clave))) {
        return nodo;
    }
    if (cmp(nodo->clave, clave) < 0) {
        if (!nodo->der) return nodo;
        return buscar_padre(nodo->der,clave,cmp);
    }else {
        if (!nodo->izq) return nodo;
        return buscar_padre( nodo->izq,clave,cmp);
    }
}

void cambiar_dato(abb_t* abb,nodo_t* raiz,void*dato){
    if(abb->destruir_dato) abb->destruir_dato(raiz->dato);
    raiz->dato = dato;
}

nodo_t *abb_buscar(nodo_t *raiz,const char *clave,abb_comparar_clave_t cmp){
    if (!raiz) return NULL;
    if(!cmp(raiz->clave, clave)) return raiz;
    return cmp(raiz->clave, clave) > 0? abb_buscar(raiz->izq, clave, cmp):abb_buscar(raiz->der, clave, cmp);
} 

bool _abb_guardar(abb_t *abb, nodo_t* raiz, const char* clave, void* dato){
    
    nodo_t* nodo = nodo_crear(clave, dato);
    if (!nodo) return false;
    
    nodo_t* padre = buscar_padre(raiz,clave,abb->cmp);
    if(!padre){
        destruir_nodo_abb(nodo);
        cambiar_dato(abb,raiz,dato);
    }else if (padre->der && !abb->cmp(clave,padre->der->clave)) {
        destruir_nodo_abb(nodo);
        cambiar_dato(abb,padre->der,dato);
    }else if (padre->izq && !abb->cmp(clave,padre->izq->clave)) {
        destruir_nodo_abb(nodo);
        cambiar_dato(abb, padre->izq,dato);
    }else if (abb->cmp(clave,padre->clave) < 0) {
        padre->izq = nodo;
        abb->cant++;
    }else if (abb->cmp(clave,padre->clave) > 0) {
        padre->der = nodo;
        abb->cant++;
    }
    return true;
}

void *borrar0(abb_t *abb, nodo_t *nodo){
    nodo_t *padre = buscar_padre(abb->raiz, nodo->clave, abb->cmp);
    if (!padre) abb->raiz = NULL;
    else if (padre->der && abb->cmp(padre->der->clave, nodo->clave) == 0) padre->der = NULL;
    else if (padre->izq && abb->cmp(padre->izq->clave, nodo->clave) == 0) padre->izq= NULL;
    
    abb->cant--;
    return destruir_nodo_abb(nodo);
}

void *borrar1(abb_t *abb, nodo_t *nodo){
    nodo_t *padre = buscar_padre(abb->raiz, nodo->clave, abb->cmp);
    if (nodo->der) {
        if (!padre) abb->raiz = nodo->der;
        else if (padre->der && abb->cmp(padre->der->clave,nodo->clave) == 0) padre->der = nodo->der;
        else padre->izq = nodo->der;
    }else if(nodo->izq) {
        if(!padre) abb->raiz = nodo->izq;
        else if (padre->der && abb->cmp(padre->der->clave,nodo->clave) == 0) padre->der = nodo->izq;
        else padre->izq = nodo->izq;
    } 
    abb->cant--;
    return destruir_nodo_abb(nodo);
}

nodo_t *buscar_nodo_aux(nodo_t *nodo){
    nodo_t *actual = nodo;
    while(actual->izq) actual = actual->izq;
    return actual;
}

void *borrar2(abb_t *abb, nodo_t *nodo){
    nodo_t *aux = buscar_nodo_aux(nodo->der);
    char *aux_clave = strdup(aux->clave);
    void *aux_dato = abb_borrar(abb, aux->clave);
    void *nodo_dato = nodo->dato;
    nodo->dato = aux_dato;
    free(nodo->clave);
    nodo->clave = aux_clave;
    return nodo_dato;
}


/* ******************************************************************
 *                DEFINICION DE LAS PRIMITIVAS DE ABB
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){

    abb_t* abb = malloc(sizeof(abb_t));
    if(!abb) return NULL;

    abb->cmp= cmp;
    abb->raiz = NULL;
    abb->destruir_dato= destruir_dato;
    abb->cant = 0;

    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    
    if (!arbol->raiz && arbol->cant == 0) {
        nodo_t* nodo_nuevo = nodo_crear(clave, dato);
        if (!nodo_nuevo) return false;
        arbol->raiz = nodo_nuevo;
        arbol->cant++;
        return true;
    }
    return _abb_guardar(arbol,arbol->raiz,clave,dato);
}

void *abb_borrar(abb_t *arbol, const char *clave){
    if (!arbol->raiz) return NULL;
    nodo_t *nodo = abb_buscar(arbol->raiz,clave, arbol->cmp);
    if (!nodo) return NULL;
    if(!nodo->izq && !nodo->der) return borrar0(arbol,nodo);
    if(nodo->der && nodo->izq) return borrar2(arbol,nodo);
    return borrar1(arbol,nodo);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    nodo_t *nodo = abb_buscar(arbol->raiz,clave,arbol->cmp);
    return nodo?nodo->dato:NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    return abb_buscar(arbol->raiz,clave,arbol->cmp);
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cant;
}

void abb_destruir_(abb_t *arbol, nodo_t *raiz){
    if (!raiz) return;
    abb_destruir_ (arbol, raiz->izq);
    abb_destruir_ (arbol, raiz->der);
    void *aux = (destruir_nodo_abb(raiz));
    if (arbol->destruir_dato) arbol->destruir_dato(aux);
}

void abb_destruir(abb_t *arbol){
    abb_destruir_(arbol, arbol->raiz);
    free(arbol);
}


/* ******************************************************************
 *               DEFINICION DE PRIMITIVAS DE ITERADOR INTERNO
 * *****************************************************************/
/*
bool abb_in_order_(nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra,char* inicio,char*fin){
    
    if (!nodo) return true;
    if(!abb_in_order_(nodo->izq,visitar,extra)) return false;
	if (!visitar(nodo->clave,nodo->dato,extra))return false;
	if(!abb_in_order_(nodo->der,visitar,extra)) return false;
    return true;
	
}*/

bool abb_in_order_(abb_comparar_clave_t cmp, nodo_t* nodo, bool visitar(char *, void *, void*,size_t ), void *extra,char* inicio,char*fin,size_t n){

    if (!nodo) return true;
    int minimo = cmp(nodo->clave, inicio);
    int maximo = cmp(nodo->clave, fin);
    
    if (minimo > 0){
        if(!abb_in_order_(cmp, nodo->izq,visitar,extra,inicio,fin,n)) return false; 
    }
    if(minimo>0 || maximo<0){
        if(!visitar(nodo->clave,nodo->dato,extra,n)) return true;
    }
    if (maximo < 0){
        if(!abb_in_order_(cmp, nodo->der,visitar,extra,inicio,fin,n)) return false;
    }
    return true;
}

void abb_in_order(abb_t *arbol, bool visitar(char *, void *, void *,size_t), void *extra,char* inicio,char* fin){ 
    if(!arbol->raiz) return;
    size_t n = 0;
    abb_in_order_(arbol->cmp ,arbol->raiz,visitar, extra,inicio,fin,n);
}



static void _abb_in_rango(nodo_t *actual, bool visitar(const char *, void *, void *,size_t*), char *extra, char *min, char *max, bool *continuar, abb_comparar_clave_t cmp,size_t *n)
{
    if (actual == NULL || !(*continuar))
    {
        return;
    }

    int min_cmp = cmp(actual->clave, min);
    int max_cmp = cmp(actual->clave, max);

    if (min_cmp > 0)
    {
        _abb_in_rango(actual->izq, visitar, extra, min, max, continuar, cmp,n);
    }
    if (*continuar && min_cmp >= 0 && max_cmp <= 0)
    {
        *continuar &= visitar(actual->clave, actual->dato, extra,n);
    }
    if (max_cmp < 0)
    {
        _abb_in_rango(actual->der, visitar, extra, min, max, continuar, cmp,n);
    }
}

void abb_in_rango(abb_t *abb, bool visitar(const char *, void *, void *,size_t*), void *extra, char *min, char *max)
{
    if (abb == NULL)
    {
        return;
    }
    size_t n = 0;
    bool continuar = true;
    _abb_in_rango(abb->raiz, visitar, extra, min, max, &continuar, abb->cmp,&n);
}
/* ******************************************************************
 *               DEFINICION DE PRIMITIVAS DE ITERADOR EXTERNO
 * *****************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if (!iter) return NULL;
    iter->pila = pila_crear();
    if(!iter->pila){
        free(iter);
        return NULL;
    }
    nodo_t* actual = arbol->raiz;
    while (actual){
        pila_apilar(iter->pila,actual);
        actual = actual->izq;
    }
    iter->abb = arbol;
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){

    if (abb_iter_in_al_final(iter)) return false;
    
    nodo_t* desapilado = pila_desapilar(iter->pila);
    if(!desapilado) return false;
    if (desapilado->der){
        pila_apilar(iter->pila,desapilado->der);
        nodo_t* actual = desapilado->der->izq;
        while(actual != NULL){
            pila_apilar(iter->pila,actual);
            actual = actual->izq;
        }
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)) return NULL;
    nodo_t* tope = pila_ver_tope(iter->pila);
    return tope->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}