#include "hospital.h"
#include "lista.h"
#include <stdio.h>
#include <string.h>
#include "split.h"
#include "struct_hospital.h"


#define NULO 0
#define ERROR 0

hospital_t* hospital_crear(){
    hospital_t* hospital = (calloc(1, sizeof(hospital_t)));
    if(!hospital) return NULO;

    lista_t* nueva_sala_de_espera = lista_crear();
    if(!nueva_sala_de_espera){
        free(hospital);
        return NULO;
    }
    hospital->sala_de_espera = nueva_sala_de_espera;

    return hospital;
}


/**
 * Recibe linea_leida y devuelve la cantidad de pokemones que se leyeron en esa linea.
 *(sabiendo que hay tantos pokemones como ((elementos en linea_leida - 2)/2).
 */
size_t cantidad_pokemones_linea(hospital_t* hospital, char** linea_leida){
    int i = 2;
    size_t contador = 0;
    while(linea_leida[i] != 0){
        contador++;
        i++;
    }

    return (contador/2);
}

/**
 * Agrega a hospital la cantidad de pokemones que posee el entrenador leído, los suma 
 * a cantidad_pokemon y suma ese entrenador a cantidad_entrenadores.
 */ 
void sumar_cantidad_pokemon_y_entrenador(hospital_t* hospital, size_t pokemones_a_agregar){
    hospital->cantidad_pokemon += pokemones_a_agregar;
    hospital->cantidad_entrenadores += 1;
}

/**
 * Recibe el string original, reserva memoria para un string igual, y copia en esa 
 * dirección de memoria reservada a original. Devuelve un puntero a la dirección de 
 * memoria o NULO en caso de error.
 */
void* crear_copia_nombre(char* original, size_t longitud_nuevo){
    void* nuevo = malloc(longitud_nuevo);
    if(!nuevo) return NULO;
    
    strcpy(nuevo, original);

    return nuevo;
}


/**
 * Agrega al hospital en la primera posicion libre del vector pokemones al pokemon leido,
 * (nombre y nivel);
 */ 
void agregar_a_hospital(hospital_t* hospital, char** linea_leida){
    int i = 2;
    while(linea_leida[i] != 0){
        if(i % 2 == 0){
            pokemon_t* pokemon = malloc(sizeof(pokemon_t));
            if(!pokemon) return;

            char* nombre_entrenador = crear_copia_nombre(linea_leida[1], strlen(linea_leida[1])+1);
            size_t id = (size_t)atoi(linea_leida[0]);
    
            pokemon->nombre_pokemon = crear_copia_nombre(linea_leida[i], strlen(linea_leida[i])+1);
            pokemon->nivel = (size_t)atoi(linea_leida[i+1]);
            pokemon->nombre_entrenador = nombre_entrenador;
            pokemon->id_entrenador = id;
            
            lista_insertar(hospital->sala_de_espera, pokemon);
        }
        i++;
    }
}


/**
 * Recibe linea_leida, libera todos sus elementos y la memoria que ocupan, 
 * luego libera linea_leida y la memoria que ocupa.
 */
void liberar_linea_leida(char** linea_leida){
    int i = 0;
    while(linea_leida[i] != 0){
        free(linea_leida[i]);
        i++;
    }
    free(linea_leida);
}



bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo){ 
    FILE* archivo_entrenadores = fopen(nombre_archivo, "r");
    if(!archivo_entrenadores){
        perror("No se pudo abrir archivo_entrenadores\n");
        return false;
    }
    
    int bytes_leidos = 0;
    int tamanio = 1024;
    char* buffer = malloc((size_t)tamanio*sizeof(int));
    
    if(!buffer){
        return NULO;
    }

    char** linea_leida;
    
    while( fgets(buffer+bytes_leidos, tamanio-bytes_leidos, archivo_entrenadores) ){
        int leido = (int)strlen(buffer+bytes_leidos);
        if( (leido > 0) && (*(buffer+bytes_leidos+leido-1) == '\n') ){
            *(buffer+bytes_leidos+leido-1) = 0;
            
            linea_leida = split(buffer, ';');
            
            size_t pokemones_a_agregar = cantidad_pokemones_linea(hospital, linea_leida);
            
            sumar_cantidad_pokemon_y_entrenador(hospital, pokemones_a_agregar);
            agregar_a_hospital(hospital, linea_leida);
           
            leido = 0;                   
            bytes_leidos = 0;
             
            liberar_linea_leida(linea_leida);
        }
        else{
            char* auxiliar = realloc(buffer, (size_t)tamanio*2);  
            if(!auxiliar){
                free(buffer);
                return NULO;
            }
            buffer = auxiliar;
            tamanio *= 2;
        }
        bytes_leidos += leido;
    }
    
    free(buffer);

    fclose(archivo_entrenadores);
   
    return true;
}


size_t hospital_cantidad_entrenadores(hospital_t* hospital){
    if(!hospital) return ERROR;

    return hospital->cantidad_entrenadores;
}


size_t hospital_cantidad_pokemon(hospital_t* hospital){
    if(!hospital) return ERROR;

    return hospital->cantidad_pokemon;
}


void ordenar_alfabeticamente_pokemones(pokemon_t** vector_pkm, size_t tope){
    if(!vector_pkm) return;

    pokemon_t* aux;
    for(int i = 0; i < tope-1; i++){
        for(int j = i + 1; j < tope; j++){
            if(strcmp(vector_pkm[i]->nombre_pokemon, vector_pkm[j]->nombre_pokemon) > 0){
                aux = vector_pkm[i];
                vector_pkm[i] = vector_pkm[j];
                vector_pkm[j] = aux;
            }
        }
    }
}


pokemon_t** vector_pkm_ordenado_alfabeticamente(hospital_t* hospital){   
    pokemon_t** vector_pkm = malloc((hospital->cantidad_pokemon)*sizeof(pokemon_t));
    if(!vector_pkm) return NULO;


    lista_iterador_t* iterador = lista_iterador_crear(hospital->sala_de_espera);

    int i = 0;
    while( lista_iterador_tiene_siguiente(iterador) ){
        vector_pkm[i] = lista_iterador_elemento_actual(iterador);

        lista_iterador_avanzar(iterador);
        i++;
    }

    lista_iterador_destruir(iterador);
    ordenar_alfabeticamente_pokemones(vector_pkm, hospital->cantidad_pokemon);

    return vector_pkm;
}


size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)){
    if((!hospital) || (!funcion) || (!hospital->sala_de_espera) || (hospital->cantidad_pokemon == 0)) return ERROR;
     
    pokemon_t** vector_pkm = vector_pkm_ordenado_alfabeticamente(hospital);
    
    size_t cantidad_iterados = 0;
    while( cantidad_iterados < (hospital->cantidad_pokemon) ){
        if( !funcion(vector_pkm[cantidad_iterados]) ){
            free(vector_pkm);
            return cantidad_iterados+1;
        } 
        cantidad_iterados++;
    }
    free(vector_pkm);

    return cantidad_iterados;
}


void hospital_destruir(hospital_t* hospital){
    if(!hospital){
        return;
    }
    
    lista_iterador_t* iterador = lista_iterador_crear(hospital->sala_de_espera);
    pokemon_t* a_eliminar = NULO;
    
    while( lista_iterador_tiene_siguiente(iterador) ){
        a_eliminar = lista_iterador_elemento_actual(iterador);
        lista_iterador_avanzar(iterador);
                
        free(a_eliminar->nombre_pokemon);
        free(a_eliminar->nombre_entrenador);
        free(a_eliminar);
    }

    lista_destruir(hospital->sala_de_espera);
    lista_iterador_destruir(iterador);

    free(hospital);
}


size_t pokemon_nivel(pokemon_t* pokemon){
    if(!pokemon) return ERROR;

    return (pokemon->nivel);
}


const char* pokemon_nombre(pokemon_t* pokemon){
    if(!pokemon) return NULO;

    return (pokemon->nombre_pokemon);
}


entrenador_t* pokemon_entrenador(pokemon_t* pokemon){
    if(!pokemon) return NULO;
    entrenador_t* entrenador = NULO;
    entrenador->nombre_entrenador = pokemon->nombre_entrenador;
    entrenador->id = pokemon->id_entrenador;

    return entrenador;
}
