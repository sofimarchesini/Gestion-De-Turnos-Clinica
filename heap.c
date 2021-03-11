#include "heap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>  // bool
#include <stddef.h>   // size_t

#define TAM_INICIAL 50
#define CTE_REDIMENSION 2

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE elementos
 * *****************************************************************/
struct heap {
    void **elementos;
    size_t tam;
    size_t cant;
    cmp_func_t cmp;
};

/* ******************************************************************
 *                FUNCIONES AUXILIARES
 * *****************************************************************/

void swap(void **elementos, size_t pos1, size_t pos2) {
    void *aux = elementos[pos1];
    elementos[pos1] = elementos[pos2];
    elementos[pos2] = aux;
}

size_t posicion_padre(size_t pos) {
    return (pos-1)/ 2;
}

size_t posicion_hijo_izq(size_t pos) {
    return (pos*2) + 1;
}

size_t posicion_hijo_der(size_t pos) {
    return  (pos*2) + 2;
}

void upheap(void **elementos, size_t pos,cmp_func_t cmp){
     
    if(pos==0) return;
    size_t pos_padre = posicion_padre(pos);
    if (cmp(elementos[pos],elementos[pos_padre])<0) return;
    swap(elementos,pos,pos_padre);
    upheap(elementos,pos_padre,cmp);
}

void downheap(void** elementos,size_t tam,size_t pos,cmp_func_t cmp){
    
    if (pos >= tam/ 2) return;
    size_t der = posicion_hijo_der(pos);
    size_t izq = posicion_hijo_izq(pos);
    size_t mayor = pos;
	
    if(izq>=tam) mayor = pos;
    else if(der>=tam) {
        if(cmp(elementos[pos],elementos[izq]) < 0) mayor = izq;
        else{
            mayor = pos;
        }
    }else{
        mayor = der;
        if(izq < tam && cmp(elementos[mayor], elementos[izq]) < 0) mayor = izq;
        if(der < tam && cmp(elementos[mayor], elementos[pos]) < 0) mayor = pos;
    }
	if(mayor != pos){
		swap(elementos, mayor,pos);
		downheap(elementos, tam,mayor,cmp);
	}  
}

bool redimensionar_heap(heap_t *heap, size_t nuevo_tam) {
    void **nuevos_elementos = realloc(heap->elementos, nuevo_tam *sizeof(void*));
    if (!nuevos_elementos) return false;
    heap->elementos = nuevos_elementos;
    heap->tam = nuevo_tam;
    return true;
}

/* ******************************************************************
 *                DEFINICION DE LAS PRIMITIVAS DE HEAP
 * *****************************************************************/
heap_t *heap_crear(cmp_func_t cmp){
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap)return NULL;
    heap->tam = TAM_INICIAL;
    heap->elementos  = malloc(heap->tam*sizeof(void*));
    if(!heap->elementos){
        free(heap);
        return NULL;
    }
    heap->cant = 0;
    heap->cmp = cmp;
    return heap;
}

void heapify(void** elementos,size_t tam, cmp_func_t cmp){
	for(size_t i = tam; i > 0; i--){
		downheap(elementos, tam,i-1,cmp); 
	}
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t* heap = heap_crear(cmp);
    if(!heap ) return NULL;
    if(n>TAM_INICIAL){
        size_t i =1;
        while(n>TAM_INICIAL*CTE_REDIMENSION*i){
            i++;
        }
        redimensionar_heap(heap,TAM_INICIAL*CTE_REDIMENSION*i);
    }
    for(int i=0;i<n;i++) {
        heap->elementos[i]  = arreglo[i];
    }
    heap->cant = n;
    heapify(heap->elementos,heap->cant,heap->cmp);
    return heap; 
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){ 
    if (destruir_elemento) {
        size_t i = 0;
        while (i<heap->cant){
            destruir_elemento(heap->elementos[i]);
            i++;
        }
    }
    free(heap->elementos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap){
    return heap->cant==0;
}

bool heap_encolar(heap_t *heap, void *elem){
    if (heap->cant == heap->tam) {
        if(!redimensionar_heap(heap, CTE_REDIMENSION * heap->tam)) return false;
    }
    heap->elementos[heap->cant] = elem;
    upheap(heap->elementos,heap->cant,heap->cmp);
    heap->cant++;
    return true;
}

void *heap_ver_max(const heap_t *heap){
    if (heap_esta_vacio(heap)) return NULL;
    return heap->elementos[0];
}

void *heap_desencolar(heap_t *heap){
    if(heap_esta_vacio(heap)) return NULL;
    if(heap_cantidad(heap)==1){
        heap->cant--;
        return heap->elementos[0];
    }
    if (heap->cant*4 <= heap->tam  && heap->tam >= TAM_INICIAL*CTE_REDIMENSION){
        redimensionar_heap(heap, heap->tam/CTE_REDIMENSION);
    }
    void* eliminado = heap->elementos[0];
    swap(heap->elementos,0,heap->cant-1);
    heap->cant--;

    downheap(heap->elementos,heap->cant,0,heap->cmp);
    return eliminado;
}

/* ******************************************************************
 *                METODO DE ORDENAMIENTO HEAPSORT
 * *****************************************************************/

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){

    if (!elementos || cant == 1 || !cmp) return;

    heapify(elementos, cant, cmp);
    for (size_t i = cant-1; i > 0; i--) {
        swap(elementos, 0,i);
        cant--;
        downheap(elementos,cant,0,cmp);
    }       
}
