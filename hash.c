#define _POSIX_C_SOURCE 200809L 
#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#define CAPACIDAD_INICIAL 10
#define FACTOR_AUMENTAR 10
#define INDICE_RED_ACHICAR 0.5
#define INDICE_RED_AGRANDAR 10
#define FACTOR_DISMINUIR 0.5


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct nodo_hash{
	void* dato;
    char* clave;
	//struct nodo* prox;
}nodo_hash_t;

struct hash {
    lista_t** listas;
    size_t capacidad;
    size_t cantidad;
    hash_destruir_dato_t destruir_dato; 
};

struct hash_iter{
    const hash_t *hash;
    size_t indice;
    lista_iter_t *lista_iter;
};

/* ******************************************************************
 *                DEFINICION DE LA FUNCION HASH
 * *****************************************************************/

size_t funcion_hash(const char *str, size_t cantidad) { //Utiliza el algoritmo 'djb2'
    size_t hash = 5381;
    int c;
    
    while ((c = *str++))
        hash = ((hash << 5) + hash) + (unsigned long)c; 
    
    return hash%cantidad;
}
/* ******************************************************************
 *                FUNCIONES DEL NODO
 * *****************************************************************/

nodo_hash_t *nodo_hash_crear(char* clave, void* dato){
    nodo_hash_t *nodo = malloc(sizeof(nodo_hash_t));
    if (!nodo) return NULL;
    nodo->clave = clave;
    nodo->dato = dato;
    return nodo;
}

/* ******************************************************************
 *                FUNCIONES AUXILIARES
 * *****************************************************************/

bool es_primo(size_t num){
	bool numero_primo = true;
	for(size_t i = 2;i<num && numero_primo;i++){
		if(num % i == 0){
			numero_primo = false;
		}
	}
	return numero_primo;
}
void hash_listas_crear(hash_t* hash){
    for (size_t i = 0; i < hash->capacidad;i++){
        hash->listas[i] = lista_crear();
        if (!hash->listas[i]) hash_destruir(hash);
    }
}

bool hash_redimensionar(hash_t *hash, float indice_redimension){

    size_t cap_nueva = (size_t)(indice_redimension*((float)hash->capacidad));
    while(!es_primo(cap_nueva)) cap_nueva++;
    lista_t **nuevas_listas = malloc(sizeof(lista_t*) * cap_nueva);
    if (!nuevas_listas) return false;
    for (size_t i = 0; i<cap_nueva; i++){
        nuevas_listas[i] = lista_crear();
        if (!nuevas_listas[i]){ 
            for (size_t j =0; j<i; j++) lista_destruir(nuevas_listas[j], NULL);
            free(nuevas_listas);
            return false;
        }
    }
    for(size_t i = 0; i < hash->capacidad; i++){
        while (!lista_esta_vacia(hash->listas[i])){
            nodo_hash_t *aux = lista_borrar_primero(hash->listas[i]);
            size_t indice = funcion_hash(aux->clave,cap_nueva);
            lista_t *lista = nuevas_listas[indice];
            lista_insertar_primero(lista, aux);
        } 
    }
    for (size_t i = 0; i < hash->capacidad; i++) lista_destruir(hash->listas[i],NULL);
    free(hash->listas);
    hash->listas = nuevas_listas;
    hash->capacidad = cap_nueva;
    return true;
}

nodo_hash_t* buscar_clave(const char* clave,lista_iter_t* iter){
    
    while(!lista_iter_al_final(iter)){
        nodo_hash_t* nodo = lista_iter_ver_actual(iter);
        if (strcmp(nodo->clave,clave) == 0){
            return nodo;
        }
        lista_iter_avanzar(iter);
    }
    return NULL;
}

/* ******************************************************************
 *                DEFINICION DE LAS PRIMITIVAS DE HASH
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t *hash = malloc(sizeof(hash_t));
    if(!hash) return NULL;
    hash->listas = malloc(sizeof(lista_t*) * CAPACIDAD_INICIAL);
	if((!hash->listas)){
		free(hash);
		return NULL;
	}
    hash->cantidad = 0;
    hash->capacidad = CAPACIDAD_INICIAL;
    hash->destruir_dato = destruir_dato;
    hash_listas_crear(hash);
    return hash;
}
bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    if((hash->cantidad)/hash->capacidad >= FACTOR_AUMENTAR) hash_redimensionar(hash,INDICE_RED_AGRANDAR);

    lista_t *lista = hash->listas[funcion_hash(clave, hash->capacidad)];
    lista_iter_t* iter = lista_iter_crear(lista);
    nodo_hash_t* nodo;
    if(buscar_clave(clave,iter)){
        nodo = buscar_clave(clave,iter);
        if(hash->destruir_dato){
            hash->destruir_dato(nodo->dato);
        }
        nodo->dato = dato;
        lista_iter_destruir(iter);
        return true;
    }
    nodo = malloc(sizeof(nodo_hash_t));
    if(!nodo) return false;
    nodo->clave = strdup(clave);
    nodo-> dato = dato;
    if(!lista_iter_insertar(iter,nodo)){
        free(nodo->clave);
        free(nodo);
        lista_iter_destruir(iter);
        return false;
    }
    hash->cantidad++;
    lista_iter_destruir(iter);
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
    if (((float)(hash->cantidad)/(float)(hash->capacidad)) < FACTOR_DISMINUIR  && (((float)hash->capacidad)* INDICE_RED_ACHICAR>= CAPACIDAD_INICIAL)){
        hash_redimensionar(hash,(float)INDICE_RED_ACHICAR);
    }
    lista_t *lista = hash->listas[funcion_hash(clave, hash->capacidad)];
    lista_iter_t* iter = lista_iter_crear(lista);
    
    if(buscar_clave(clave,iter)){
        nodo_hash_t* nodo = buscar_clave(clave,iter);
        lista_iter_borrar(iter);
        lista_iter_destruir(iter);
        free(nodo->clave);
        void *dato = nodo->dato;
        hash->cantidad--;
        free(nodo);
        return dato;
    }
    lista_iter_destruir(iter);
    return NULL;
}

void *hash_obtener(const hash_t *hash, const char *clave){
    lista_t *lista = hash->listas[funcion_hash(clave,hash->capacidad)];
    lista_iter_t* iter = lista_iter_crear(lista);

    if(buscar_clave(clave,iter)){
        nodo_hash_t* nodo = buscar_clave(clave,iter);
        lista_iter_destruir(iter);
        return nodo->dato;
    }
    lista_iter_destruir(iter);
    return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    lista_t *lista = hash->listas[funcion_hash(clave,hash->capacidad)];
    lista_iter_t* iter = lista_iter_crear(lista);
    
    if(buscar_clave(clave,iter)){
        lista_iter_destruir(iter);
        return true;
    }
    lista_iter_destruir(iter);
    return false;    
}

size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad;
}

void hash_destruir(hash_t *hash){
    for (size_t i = 0; i<hash->capacidad; i++){
        while(!lista_esta_vacia(hash->listas[i])){
            nodo_hash_t *aux = lista_borrar_primero(hash->listas[i]);
            if(hash->destruir_dato){
                hash->destruir_dato(aux->dato);
            }
            free(aux->clave);
            free(aux);
        }
        lista_destruir(hash->listas[i], NULL);
    }
    free(hash->listas);
    free(hash);
}

/* ******************************************************************
 *               DEFINICION DE PRIMITIVAS DE ITERADOR EXTERNO
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash){
    if(!hash) return NULL;
    hash_iter_t *iter = malloc(sizeof(hash_iter_t));
    if (!iter) return NULL;
    iter->hash = hash;
    iter->indice = 0;
 
    iter->lista_iter = lista_iter_crear(iter->hash->listas[iter->indice]);
    
    if(lista_iter_al_final(iter->lista_iter)){
        hash_iter_avanzar(iter);
    }
    return iter;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    return (iter->indice ==iter->hash->capacidad);
}

bool hash_iter_avanzar(hash_iter_t *iter){
    

    if(hash_iter_al_final(iter)) return false;
    lista_iter_avanzar(iter->lista_iter);
    while (lista_iter_al_final(iter->lista_iter)){
        lista_iter_destruir(iter->lista_iter);
        iter->indice++;
        if(hash_iter_al_final(iter)) return false;
        iter->lista_iter = lista_iter_crear(iter->hash->listas[iter->indice]);
    }
    
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
    if(hash_iter_al_final(iter)) return NULL;
    nodo_hash_t* nodo_actual = lista_iter_ver_actual(iter->lista_iter);
    return nodo_actual->clave;
}

void hash_iter_destruir(hash_iter_t *iter){
    if(!hash_iter_al_final(iter)) lista_iter_destruir(iter->lista_iter);
    free(iter);
}
