
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);

typedef void (*abb_destruir_dato_t) (void *);

/* Crea un abb. Recibe como único parámetro la función de comparación a
 * utilizar. Devuelve un puntero al abb, el cual debe ser destruido con
 * abb_destruir().
 */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Agrega un elemento al abb
 * Devuelve true si fue una operación exitosa, o false en caso de error.
 * Pre: el abb fue creado.
 * Post: se agregó un nuevo elemento al abb
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Elimina el elemento pasado por parametro, y lo devuelve.
 * Si el arbol esta vacío, devuelve NULL.
 * Pre: el abb fue creado.
 * Post: el elemento borraado ya no se encuentra en el abb.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Devuelve el dato de la clave que pasamos por parametro. */
void *abb_obtener(const abb_t *arbol, const char *clave);

// devuelve NULL si la clave pertenece al abb, True en el caso contrario
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos que hay en el arbol. */
size_t abb_cantidad(abb_t *arbol);

// Destruye el abb
// Pre:el abb fue creado
// Post: se eliminaron todos los elementos del abb
void abb_destruir(abb_t *arbol);

//crea el iterador interno
void abb_in_order(abb_t *arbol, bool visitar(char *, void *, void *,size_t), void *extra,char* inicio,char* fin);

void abb_in_rango(abb_t *abb, bool visitar(const char *, void *, void *,size_t*), void *extra, char *min, char *max);
typedef struct abb_iter abb_iter_t;

// crea el iterador externo
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// avanza el iterador
bool abb_iter_in_avanzar(abb_iter_t *iter);

// donde esta parado el iterador
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// devuelve true si estoy al final , y false en caso contrario
bool abb_iter_in_al_final(const abb_iter_t *iter);

// destruye el iterador
void abb_iter_in_destruir(abb_iter_t* iter);