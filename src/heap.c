#include "heap.h"
#include "struct_hospital.h"
#include "hospital.h"
#include "simulador.h"


#define NULO 0


heap_t* crear_heap(size_t tamanio_heap){
    if(tamanio_heap == 0) return NULO;
    
    heap_t* nuevo = malloc(sizeof(heap_t));
    if(!nuevo) return NULO;

    void** vector = calloc(tamanio_heap, sizeof(void*));
    if(!vector){
        free(nuevo);
        return NULO;
    } 
    nuevo->vector = vector;
    nuevo->tope = 0;
    nuevo->tamanio = tamanio_heap;

    return nuevo;
}


void swap(void** vector, size_t pos_1, size_t pos_2){
    void* aux;

    aux = vector[pos_1];
    vector[pos_1] = vector[pos_2];
    vector[pos_2] = aux;
}


void sift_up(heap_t* heap, size_t posicion_actual){
    if( !heap || (posicion_actual == 0) ) return;

    size_t posicion_padre = (posicion_actual-1)/2;

    pokemon_t* actual = heap->vector[posicion_actual];
    pokemon_t* padre = heap->vector[posicion_padre];
    if( (actual->nivel) < (padre->nivel) ){
        swap(heap->vector, posicion_actual, posicion_padre);
        sift_up(heap, posicion_padre);
    }
}


/**
 * Recibe un heap que está lleno. Reserva más memoria para agregar más elementos.
 */
void redimensionar_vector_heap(heap_t* heap){
    size_t nuevo_tamanio = 2 * (heap->tamanio);

    void** nuevo_vector = realloc(heap->vector, sizeof(void*) * nuevo_tamanio);
    if(!nuevo_vector) return;

    heap->vector = nuevo_vector;
    heap->tamanio = nuevo_tamanio;
}


void heap_insertar(heap_t* heap, void* elemento){
    if(!heap) return;

    if(heap->tope == heap->tamanio){
        redimensionar_vector_heap(heap);
    }

    heap->vector[heap->tope] = elemento;   
    sift_up(heap, heap->tope);
    (heap->tope)++;
}


void sift_down(void** vector, size_t tope, size_t pos_actual){
    if(!vector) return;
    
    size_t hijo_izquierdo = (pos_actual*2) + 1;
    size_t hijo_derecho = (pos_actual*2) + 2;
    size_t pos_a_intercambiar = pos_actual;

    if(hijo_izquierdo >= tope){
        return;
    }
    else if(hijo_derecho >= tope){
        hijo_derecho = hijo_izquierdo;
    }

    pokemon_t* derecha = vector[hijo_derecho];
    pokemon_t* izquierda = vector[hijo_izquierdo];
    
    if( (derecha->nivel) < (izquierda->nivel) ){
        pos_a_intercambiar = hijo_derecho;
    }
    else{
        pos_a_intercambiar = hijo_izquierdo;
    }


    pokemon_t* a_intercambiar = vector[pos_a_intercambiar];
    pokemon_t* actual = vector[pos_actual];
    

    if( (a_intercambiar->nivel) < (actual->nivel) ){
        swap(vector, pos_actual, pos_a_intercambiar);
        sift_down(vector, tope, pos_a_intercambiar);
    }
}


void* heap_extraer_raiz(heap_t* heap){
    if( !heap || (heap->tope == 0) ) return NULO;
    
    void* raiz_extraida = heap->vector[0];
    heap->vector[0] = heap->vector[(heap->tope)-1];

    sift_down(heap->vector, heap->tope, 0);

    (heap->tope)--;
    return raiz_extraida;
}


void heap_destruir(heap_t* heap){
    if(!heap) return;

    free(heap->vector);
    free(heap);
}



