#ifndef STRUCT_HOSPITAL_H_
#define STRUCT_HOSPITAL_H_

#include "hospital.h"
#include "lista.h"

struct _hospital_pkm_t{
    lista_t* sala_de_espera; 
    size_t cantidad_pokemon;
    size_t cantidad_entrenadores;
};

struct _pkm_t{
    char* nombre_pokemon;
    size_t nivel;
    char* nombre_entrenador;
    size_t id_entrenador;
};

struct _entrenador_t{
    char* nombre_entrenador;
    size_t id;
};


#endif // STRUCT_HOSPITAL_H_
