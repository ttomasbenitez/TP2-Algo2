#ifndef STRUCT_SIMULADOR_H_
#define STRUCT_SIMULADOR_H_

#include "simulador.h"
#include "lista.h"
#include "heap.h"

struct _simulador_t{
    hospital_t* hospital;
    heap_t* pkm_recepcionados;
    pokemon_t* pkm_en_tratamiento;
    lista_t* dificultades;
    EstadisticasSimulacion estadisticas;
    InformacionDificultad info_dificultad;
    unsigned cantidad_intentos_pkm_actual;
    bool simulando;
};

#endif // STRUCT_SIMULADOR_H_
