#ifndef HEAP_H_
#define HEAP_H_

#include <stdio.h>
#include <stdlib.h>


typedef struct heap{
    void** vector;
    size_t tope;
    size_t tamanio;
}heap_t;

/**
 * Reserva memoria para un heap y reserva e inicializa 
 * un vector de punteros de tamanio_heap elementos, tamanio_heap tiene que ser mayor a
 * cero.
 *
 * Devuelve NULL en caso de error.
 */
heap_t* crear_heap(size_t tamanio_heap);

/**
 * Compara el nodo en posicion_actual con su padre, y si el padre es mayor que el hijo,
 * los intercambia de posición, buscando que quede un heap minimal.
 */
void sift_up(heap_t* heap, size_t posicion_actual);

/**
 * Inserta un elemento en la última posición del heap y usando sift_up() actualiza su
 * posición según corresponda.
 */
void heap_insertar(heap_t* heap, void* elemento);

/**
 * Recibe una posición actual de un elemento del heap, se compara el elemento con sus hijos,
 * con el hijo de menor nivel se fija si es menor que el padre, si es menor, se intercambian.
 * Se repite el proceso con la nueva posición del que era padre hasta que haya encontrado
 * su posición correcta según un heap minimal.
 */
void sift_down(void** vector, size_t tope, size_t pos_actual);

/**
 * Extrae el elemento en la raíz del heap, asigna a la raíz el elemento que estaba en la
 * última posición y reduce el tope. Mediante sift_down() deja al que era el último elemento
 * en su posición correcta.
 * Devuelve el elemento que se sacó al principio o NULL en caso de error.
 */
void* heap_extraer_raiz(heap_t* heap);

/**
 * Libera la memoria ocupada por el heap y su vector. 
 */
void heap_destruir(heap_t* heap);


#endif // HEAP_H_