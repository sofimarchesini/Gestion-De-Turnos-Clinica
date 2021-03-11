#ifndef lista_H
#define lista_H

#include <stdbool.h>
#include <stddef.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */

struct lista;
typedef struct lista lista_t;
struct lista_iter;
typedef struct lista_iter lista_iter_t;


/* ******************************************************************
 *                    PRIMITIVAS DE LA lista
 * *****************************************************************/

// Crea una LISTA.
// Post: devuelve una nueva listavacía.
lista_t *lista_crear(void);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

// Devuelve verdadero si la lista no tiene elementos enlistados, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento en la primera posicion de  la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento al principio de la lista
bool lista_insertar_primero(lista_t *lista, void *dato);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Agrega un nuevo elemento en la ultima posicion de  la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento al final de la lista
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca elultimo elemento de la lista. Si la lista tiene elementos, se quita el
// ultimo de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se elimino el ultimo elemento de la lista
//void *lista_borrar_ultimo(lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimoelemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

//devuelve el largo de la lista.
//pre : la lista fue creada
//post : se devolvio el largo de la lista
size_t lista_largo(const lista_t *lista);

// Itera la lista. Si visitar devuelve false, la lista dejará de iterarse.
// Pre: la lista fue creada. La función visitar se llamará para cada elemento
// de la lista y debe ser != NULL. El parametro extra se la utilizará en la función
// visitar.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO DE LA LISTA
 * *****************************************************************/
 
// crea un iterador externo de la lista
// post : el iterador fue creado en la primera posicion de la lista
lista_iter_t *lista_iter_crear(lista_t *lista);

// avanza una posicion en la lista
// pre : el iterador fue creado
// post : el iterador avanzo una posicion, 
//devuelve true si avanzo correctamente
bool lista_iter_avanzar(lista_iter_t *iter);

//devuelve el elemento actual
// pre : el iterador fue creado
// post : devuelve el elemento actual
void *lista_iter_ver_actual(const lista_iter_t *iter);
// Verifica si el iterador se encuentra al final de la lista.
// Pre: el iterador fue creado.
// Post: devuelve true en si se encuentra al final, false en caso
// contrario.

bool lista_iter_al_final(const lista_iter_t *iter);

// destruye el iterador.
// Pre: el iterador fue creado.
// Post: se elimino el iterador
void lista_iter_destruir(lista_iter_t *iter);

// inserta un dato donde esta posicionado el iterador
// pre : el iterador fue creado
// post se inserto un elemento
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Elimina el elemento en la posición donde se encuentra el iterador.
// Pre: el iterador fue creado.
// Post: se eliminó un elemento de la lista.
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación realizada.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_lista.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en lista.h
// (esto es, las pruebas no pueden acceder a los miembros del struct lista).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_estudiante(void);


#endif  // lista_H
