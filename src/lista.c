#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define ERROR 0

/**
 * POST:
 * Reserva memoria para una lista_t y lo inicializa en 0. Devuelve un puntero a la 
 * dirección de memoria, o NULL en caso de error.
 */
lista_t* lista_crear(){
    return calloc(1, sizeof(lista_t));
}

/**
 * POST:
 * Reserva memoria para una nodo_t y devuelve un puntero a la dirrección de memoria,
 * o NULL en caso de error.
 */
nodo_t* crear_nodo(){
    return calloc(1, sizeof(nodo_t));
}

/**
 * PRE: Recibe un puntero nodo NULL de la lista. También recibe un elemento válido.
 * POST: Le asigna memoria que fue reservada con crear_nodo() al puntero nodo recibido, 
 * y asigna el elemento recibido al nodo creado. 
 * Devuelve puntero a el nodo cargado. En caso de error al reservar memoria, devuelve NULL.
 */
nodo_t* cargar_nodo(nodo_t* nodo_a_cargar, void* elemento){
    nodo_t* nuevo = crear_nodo();
    if(!nuevo) return NULL;
    
    nodo_a_cargar = nuevo;
    nodo_a_cargar->elemento = elemento;
    
    return nodo_a_cargar;
}


/**
 * POST: Asigna al final de lista un nuevo nodo, con el elemento recibido. 
 * Devuelve NULL si no pudo insertar el elemento a causa de un error, o la lista en caso de éxito.
 */
lista_t* lista_insertar(lista_t* lista, void* elemento){
    if(!lista) return NULL;
    
    if(!lista->nodo_inicio){
        nodo_t* nuevo = crear_nodo();
        if(!nuevo) return NULL;

        lista->nodo_inicio = nuevo;
        lista->nodo_inicio->elemento = elemento;
        lista->nodo_fin = lista->nodo_inicio;
    }
    else{
        lista->nodo_fin->siguiente = cargar_nodo(lista->nodo_fin->siguiente, elemento);
        lista->nodo_fin = lista->nodo_fin->siguiente;
        lista->nodo_fin->siguiente = NULL;
    }

    lista->cantidad++;
    return lista;
}

/**
 * POST: 
 * Asigna en la posicion(a partir de 0) indicada de lista un nuevo nodo, con el elemento 
 * recibido. En caso de no existir la posicion indicada, lo inserta al final.
 * Devuelve NULL si no pudo insertar el elemento a causa de un error, o la lista en caso de exito.
 */
lista_t* lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
    if(!lista) return NULL;

    if(posicion >= (lista->cantidad)){
        lista_insertar(lista, elemento);
        return lista;
    }
    else if(posicion == 0){
        nodo_t* nuevo = NULL;
        nuevo = cargar_nodo(nuevo, elemento);
        
        nuevo->siguiente = lista->nodo_inicio;
        lista->nodo_inicio = nuevo;
    }
    else{
        int i = 1;
        nodo_t* lugar_a_insertar = lista->nodo_inicio;
        while(i < posicion){
            lugar_a_insertar = lugar_a_insertar->siguiente;
            i++;
        }

        nodo_t* nuevo_elemento = NULL;
        nuevo_elemento = cargar_nodo(nuevo_elemento, elemento);

        nuevo_elemento->siguiente = lugar_a_insertar->siguiente;
        lugar_a_insertar->siguiente = nuevo_elemento;
    }

    (lista->cantidad)++;
    return lista;
}


/**
 * POST:
 * Quita de la lista el elemento que se encuentra en la ultima posición y libera la memoria
 * que ocupa.
 * Devuelve el elemento removido de la lista o NULL en caso de error.
 */
void* lista_quitar(lista_t* lista){
    if(lista_vacia(lista)) return NULL;

    void* elemento_quitado = lista->nodo_fin->elemento;

    if(lista->nodo_fin == lista->nodo_inicio){
        free(lista->nodo_inicio);
        lista->nodo_fin = NULL;
        lista->nodo_inicio = NULL;
    }
    else{
        nodo_t* iterador = lista->nodo_inicio;
        while(iterador->siguiente->siguiente){
            iterador = iterador->siguiente;
        }

        free(lista->nodo_fin);
        lista->nodo_fin = iterador;
        lista->nodo_fin->siguiente = NULL;

    }

    (lista->cantidad)--;
    return elemento_quitado;
}

/**
 * POST:
 * Quita de la lista el elemento que se encuentra en la posición(a partir de 0) indicada, 
 * y libera le memoria que ocupa.
 * 
 * En caso de no existir esa posición se intentará borrar el último
 * elemento.
 * Devuelve el elemento removido de la lista o NULL en caso de error.
 */
void* lista_quitar_de_posicion(lista_t* lista, size_t posicion){
    if(lista_vacia(lista)) return NULL;

    nodo_t* a_eliminar = NULL;
    void* elemento_quitado = NULL; 

    if(posicion >= (lista->cantidad - 1)){
        return lista_quitar(lista);
    }
    else if(posicion == 0){
        elemento_quitado = lista->nodo_inicio->elemento;

        a_eliminar = lista->nodo_inicio;
        if(lista->nodo_inicio == lista->nodo_fin){
            lista->nodo_inicio = NULL;
            lista->nodo_fin = NULL;
        }
        else{
            lista->nodo_inicio = lista->nodo_inicio->siguiente;
        }
    }
    else{
        int i = 1;
        nodo_t* posicion_anterior = lista->nodo_inicio;
        while(i < posicion){
            posicion_anterior = posicion_anterior->siguiente;
            i++;
        }
        nodo_t* posicion_a_eliminar = posicion_anterior->siguiente;

        elemento_quitado = posicion_a_eliminar->elemento;
        a_eliminar = posicion_a_eliminar;
        posicion_anterior->siguiente = posicion_a_eliminar->siguiente;
    }
        
    free(a_eliminar);
    (lista->cantidad)--;    
    return elemento_quitado;
}


/**
 * POST:
 * Devuelve el elemento en la posición(a partir de 0) indicada.
 * Si no existe dicha posición devuelve NULL.
 */
void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
    if( lista_vacia(lista) || (posicion >= (lista->cantidad)) ) return NULL;

    int i = 0;
    nodo_t* nodo_elemento_buscado = lista->nodo_inicio;
    while(i < posicion){
        nodo_elemento_buscado = nodo_elemento_buscado->siguiente;
        i++;
    }
    return (nodo_elemento_buscado->elemento);
}


/**
 * POST:
 * Devuelve el primer elemento de la lista o NULL si la lista se
 * encuentra vacía o no existe.
 */
void* lista_primero(lista_t* lista){
    if(lista_vacia(lista)) return NULL;

    return (lista->nodo_inicio->elemento);
}

/**
 * POST:
 * Devuelve el último elemento de la lista o NULL si la lista se
 * encuentra vacía o no existe.
 */
void* lista_ultimo(lista_t* lista){
    if(lista_vacia(lista)) return NULL;

    return (lista->nodo_fin->elemento);
}

/**
 * POST:
 * Devuelve true si la lista está vacía (o no existe) o false en caso contrario.
 */
bool lista_vacia(lista_t* lista){
    if( (!lista) || ((lista->cantidad) == 0) ) return true;
    
    return false;
}

/**
 * POST:
 * Devuelve la cantidad de elementos en la lista, si esta existe.
 * Caso contrario devuelve ERROR.
 */
size_t lista_tamanio(lista_t* lista){
    if(lista_vacia(lista)) return ERROR;

    return (lista->cantidad);
}

/**
 * POST:
 * Libera la memoria reservada por la lista, si esta existe.
 */
void lista_destruir(lista_t* lista){
    if(!lista){
        printf("No se pudo destruir porque no existe el dato");
        return;
    }
        
    nodo_t* actual;

    while(lista->cantidad > 0){
        actual = lista->nodo_inicio;
        lista->nodo_inicio = lista->nodo_inicio->siguiente;
        free(actual);
        (lista->cantidad)--;
    }
    free(lista);
}

/**
 * POST:
 * Reserva memoria para un lista_iterador_t y lo inicializa en 0.
 * Le asigna lista a iterador e inicializa iterador->corriente en nodo_inicio de lista.
 * Devuelve el puntero al iterador creado o NULL en caso de error.
 */
lista_iterador_t* lista_iterador_crear(lista_t* lista){
    if(!lista) return NULL;

    lista_iterador_t* iterador = calloc(1, sizeof(lista_iterador_t));
    if(!iterador) return NULL;

    iterador->lista = lista;
    iterador->corriente = lista->nodo_inicio;

    return iterador;
}

/**
 * POST:
 * Devuelve true si hay mas elementos sobre los cuales iterar o false
 * si no hay mas.
 */
bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    if( (!iterador) || (lista_vacia(iterador->lista)) || (!iterador->corriente) ) return false;
    
    return true;
}


/**
 * POST:
 * Avanza el iterador al siguiente elemento.
 * Devuelve true si pudo avanzar el iterador o false en caso de
 * que no queden elementos o en caso de error.
 */
bool lista_iterador_avanzar(lista_iterador_t* iterador){
    if( (!iterador) || (lista_vacia(iterador->lista)) || (!iterador->corriente) ) return false;
    
	if(iterador->corriente->siguiente){
		iterador->corriente = iterador->corriente->siguiente;
        return true;
	}
    iterador->corriente = iterador->corriente->siguiente;
    return false;
}

/**
 * POST:
 * Devuelve el elemento actual del iterador o NULL en caso de que no
 * exista dicho elemento o en caso de error.
 */
void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
    if( (!iterador) || (lista_vacia(iterador->lista)) || (!iterador->corriente) ) return NULL;
	                                                        
	return (iterador->corriente->elemento);
}


/**
 * POST:
 * Libera la memoria reservada por el iterador, si este existe.
 */
void lista_iterador_destruir(lista_iterador_t* iterador){
    if(!iterador){
        printf("No se pudo destruir lista_iterador porque no existe");
        return;
    }
    free(iterador);
}


/**
 * PRE: Recibe una que  función puede devolver true si se deben seguir recorriendo
 * elementos o false si se debe dejar de iterar elementos.
 * POST: Recorre la lista e invoca la función.
 * La función devuelve la cantidad de elementos iterados o ERROR en caso de error.
 */
size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto){
    if( (!lista) || (!funcion) ) return ERROR;

    size_t cantidad_iterados = 0;
    lista_iterador_t* iterador = lista_iterador_crear(lista);
    while( (lista_iterador_tiene_siguiente(iterador) ) && ((*funcion)(iterador->corriente->elemento, contexto)) ){
        cantidad_iterados++;
        lista_iterador_avanzar(iterador);
    }
    
    lista_iterador_destruir(iterador);
    return cantidad_iterados;
}