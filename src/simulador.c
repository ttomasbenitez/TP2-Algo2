#include "simulador.h"
#include "struct_simulador.h"
#include "heap.h"
#include <string.h>
#include "struct_hospital.h"


#define ID_FACIL 0
#define ID_NORMAL 1
#define ID_DIFICIL 2

#define DIFICULTAD_INVALIDA -1

#define ES_GRANDE 1
#define ES_MUY_GRANDE 2
#define ADIVINADO 0
#define ES_CHICO -1
#define ES_MUY_CHICO -2

#define NULO 0
#define EXITO 0
#define ERROR -1

#define MAX_PUNTOS_DIFICULTAD_FACIL 15
#define MEDIO_PUNTOS_DIFICULTAD_FACIL 10
#define MIN_PUNTOS_DIFICULTAD_FACIL 3

#define MAX_PUNTOS_DIFICULTAD_NORMAL 20
#define MEDIO_PUNTOS_DIFICULTAD_NORMAL 10
#define MIN_PUNTOS_DIFICULTAD_NORMAL 5

#define MAX_PUNTOS_DIFICULTAD_DIFICIL 25
#define MEDIO_PUNTOS_DIFICULTAD_DIFICIL 15
#define MIN_PUNTOS_DIFICULTAD_DIFICIL 5

//******************************DIFICULTADES********************************


unsigned calcular_puntaje_dificultad_facil(unsigned cantidad_intentos){
    if(cantidad_intentos >= 10) return MIN_PUNTOS_DIFICULTAD_FACIL;
    else if(cantidad_intentos >= 5) return MEDIO_PUNTOS_DIFICULTAD_FACIL;
    else return MAX_PUNTOS_DIFICULTAD_FACIL;
}

unsigned calcular_puntaje_dificultad_normal(unsigned cantidad_intentos){
    if(cantidad_intentos >= 10) return MIN_PUNTOS_DIFICULTAD_NORMAL;
    else if(cantidad_intentos >= 5) return MEDIO_PUNTOS_DIFICULTAD_NORMAL;
    else return MAX_PUNTOS_DIFICULTAD_NORMAL;
}

unsigned calcular_puntaje_dificultad_dificil(unsigned cantidad_intentos){
    if(cantidad_intentos >= 10) return MIN_PUNTOS_DIFICULTAD_DIFICIL;
    else if(cantidad_intentos >= 5) return MEDIO_PUNTOS_DIFICULTAD_DIFICIL;
    else return MAX_PUNTOS_DIFICULTAD_DIFICIL;
}



int verificar_nivel_adivinado_dificultad_facil(unsigned nivel_adivinado, unsigned nivel_pokemon){
    if(nivel_adivinado == nivel_pokemon) return ADIVINADO;
    
    else return (abs((int)nivel_adivinado - (int)nivel_pokemon));
}

int verificar_nivel_adivinado_dificultad_normal(unsigned nivel_adivinado, unsigned nivel_pokemon){
    if(nivel_adivinado == nivel_pokemon) return ADIVINADO;
    else if(nivel_adivinado > (nivel_pokemon+10)) return ES_MUY_GRANDE;
    else if(nivel_adivinado > nivel_pokemon) return ES_GRANDE;
    else if((nivel_adivinado+10) < nivel_pokemon) return ES_MUY_CHICO;
    else return ES_CHICO;
}

int verificar_nivel_adivinado_dificultad_dificil(unsigned nivel_adivinado, unsigned nivel_pokemon){
    if(nivel_adivinado == nivel_pokemon) return ADIVINADO;
    else return ERROR;
}



const char* salida_segun_verificacion_dificultad_facil(int resultado_verificacion){
    if(resultado_verificacion == ADIVINADO) return "CORRECTO, ya se atendió al pokemón :)";
    else if(resultado_verificacion <= 3) return "Estás dentro del rango de 3 niveles";
    else if(resultado_verificacion <= 5) return "Estás dentro del rango de 5 niveles";
    else if(resultado_verificacion <= 10) return "Estás dentro del rango de 10 niveles";
    else if(resultado_verificacion <= 20) return "Estás dentro del rango de 20 niveles";
    else if(resultado_verificacion <= 40) return "Estás dentro del rango de 40 niveles";
    else if(resultado_verificacion <= 50) return "Estás dentro del rango de 50 niveles";
    else return "Tas re lejos";
}

const char* salida_segun_verificacion_dificultad_normal(int resultado_verificacion){
    if(resultado_verificacion == ADIVINADO) 
        return "CORRECTO, ya se atendió al pokemón :)";
    else if(resultado_verificacion == ES_MUY_GRANDE) 
        return "Te re pasaste";
    else if(resultado_verificacion == ES_GRANDE) 
        return "Te pasaste un poco";
    else if(resultado_verificacion == ES_MUY_CHICO) 
        return "Te falta muchísimo";
    else
        return "Te falta un poco";
}

const char* salida_segun_verificacion_dificultad_dificil(int resultado_verificacion){
    if(resultado_verificacion == ADIVINADO) return "CORRECTO, ya se atendió al pokemón :)";
    else return "No es.";
}


//*************************************************************************************


/**
 * Recibe un largo y reserva un bloque de memoria de tamaño largo bytes para un string. 
 * Devuelve el puntero a la dirección de memoria creada o NULL en caso de error.
 */
char* reservar_memoria_nombre(size_t largo){
    return malloc(largo*sizeof(char));
}


void inicializar_estadisticas(simulador_t* simulador, hospital_t* hospital){
    simulador->estadisticas.cantidad_eventos_simulados = 0;
    simulador->estadisticas.entrenadores_atendidos = 0;
    simulador->estadisticas.entrenadores_totales = (unsigned)hospital->cantidad_entrenadores;
    simulador->estadisticas.pokemon_atendidos = 0;
    simulador->estadisticas.pokemon_en_espera = 0;
    simulador->estadisticas.pokemon_totales = (unsigned)hospital->cantidad_pokemon;
    simulador->estadisticas.puntos = 0;
}


/**
 * Recibe el nombre de una dificultad y reserva memoria para DatosDificultad y para el 
 * nombre de la dificultad.
 * Asigna las funciones correspondientes a la nueva dificultad según corresponda.
 * Devuelve la dirección de memoria de la dificultad creada o NULO en caso de error.
 */
DatosDificultad* crear_y_cargar_dificultad(char* nombre){
    DatosDificultad* dificultad = malloc(sizeof(DatosDificultad));
    if(!dificultad) return NULO;

    
    char* nombre_dif = reservar_memoria_nombre(strlen(nombre)+1);
    if(!nombre_dif){
        free(dificultad);
        return NULO;
    }

    strcpy(nombre_dif, nombre);
    dificultad->nombre = nombre_dif;
    
    if(strcmp(nombre, "FACIL") == 0){
        dificultad->verificar_nivel = verificar_nivel_adivinado_dificultad_facil;
        dificultad->verificacion_a_string = salida_segun_verificacion_dificultad_facil;
        dificultad->calcular_puntaje = calcular_puntaje_dificultad_facil;
    }
    else if(strcmp(nombre, "NORMAL") == 0){
        dificultad->verificar_nivel = verificar_nivel_adivinado_dificultad_normal;
        dificultad->verificacion_a_string = salida_segun_verificacion_dificultad_normal;
        dificultad->calcular_puntaje = calcular_puntaje_dificultad_normal;
    }
    else{
        dificultad->verificar_nivel = verificar_nivel_adivinado_dificultad_dificil;
        dificultad->verificacion_a_string = salida_segun_verificacion_dificultad_dificil;
        dificultad->calcular_puntaje = calcular_puntaje_dificultad_dificil;
    }

    
    return dificultad;
}


/**
 *Inserta el la lista de dificultades las tres dificultades predeterminadas del simulador.
 */
void inicializar_lista_dificultades(lista_t* dificultades){
    
    DatosDificultad* dificultad_1 = crear_y_cargar_dificultad("FACIL");
    lista_insertar(dificultades, (void*)dificultad_1);
    
    DatosDificultad* dificultad_2 = crear_y_cargar_dificultad("NORMAL");
    lista_insertar(dificultades, (void*)dificultad_2);

    DatosDificultad* dificultad_3 = crear_y_cargar_dificultad("DIFICIL");
    lista_insertar(dificultades, (void*)dificultad_3);
}


/**
 * Inicializa la dificultad en uso del simulador con los datos de la dificultad NORMAL.
 * Crea una copia del nombre y la guarda.
 */
void inicializar_dificultad_en_uso(InformacionDificultad* usandose){
    char* nombre_dificultad_inicial = "NORMAL";

    char* nombre = reservar_memoria_nombre(strlen(nombre_dificultad_inicial)+1);
    if(!nombre) return;

    strcpy(nombre, nombre_dificultad_inicial);

    usandose->id = ID_NORMAL;
    usandose->nombre_dificultad = nombre;
    usandose->en_uso = true;
}





simulador_t* simulador_crear(hospital_t* hospital){
    if(!hospital) return NULO;  
    
    simulador_t* nuevo = calloc(1, sizeof(simulador_t));
    if(!nuevo) return NULO;

    heap_t* heap = crear_heap((hospital->cantidad_pokemon)+1);
    if(!heap){
        free(nuevo);
        return NULO;
    } 

    lista_t* lista_dificultades = lista_crear();
    if(!lista_dificultades){
        free(nuevo);
        free(heap);
        return NULO;
    }

    nuevo->dificultades = lista_dificultades;
    nuevo->pkm_recepcionados = heap;
    nuevo->hospital = hospital;
    nuevo->simulando = true;

    inicializar_estadisticas(nuevo, hospital);
    inicializar_dificultad_en_uso(&(nuevo->info_dificultad));
    inicializar_lista_dificultades(nuevo->dificultades);

    return nuevo;
}





void obtener_estadisticas(simulador_t* simulador, EstadisticasSimulacion* estadistica){
    (*estadistica) = simulador->estadisticas;
}


/**
 * Crea un iterador de lista y recorre la lista comparando el entrenador del pokemón en 
 * la primera posición, mientras el entrenador actual sea igual al de la primera posición
 * se agrega al heap pkm_recepcionados y suma 1 a insertados. 
 * Devuelve la cantidad de pokemones insertados pkm_recepcionados.
 */
size_t recorrer_e_insertar(simulador_t* simulador){

    pokemon_t* primero = lista_primero(simulador->hospital->sala_de_espera);
    heap_insertar(simulador->pkm_recepcionados, (void*)primero);
    size_t insertados = 1;

    lista_iterador_t* iterador = lista_iterador_crear(simulador->hospital->sala_de_espera);

    while(lista_iterador_tiene_siguiente(iterador)){
        lista_iterador_avanzar(iterador);
        pokemon_t* actual = lista_iterador_elemento_actual(iterador);

        if(actual){
            if(primero->id_entrenador != actual->id_entrenador){
                lista_iterador_destruir(iterador);
                return insertados;
            }

            heap_insertar(simulador->pkm_recepcionados, (void*)actual);
        }

        insertados++;
    }
    lista_iterador_destruir(iterador);

    return insertados;
}


/**
 * Llama a recorrer_e_insertar() y con la cantidad de ya_recepcionados quita de la 
 * sala_de_espera los pokemones que ya fueron agregados al heap pkm_recepcionados.
 * Si no hay ningún pokemón en tratamiento quita el elemento en la raíz del heap
 * y lo guarda como pokemon_en_tratamiento.
 * Actualiza las estadísticas según corresponda.
 * Devuelve ERROR en caso de fallar o EXITO si no hubo problema.
 */
int atender_proximo_entrenador(simulador_t* simulador){
    if(!simulador || !simulador->hospital->cantidad_entrenadores) return ERROR;

    size_t ya_recepcionados = recorrer_e_insertar(simulador);
    
    while(ya_recepcionados > 0){
        lista_quitar_de_posicion(simulador->hospital->sala_de_espera, 0);
        ya_recepcionados--;
    }

    if(!simulador->pkm_en_tratamiento){
        simulador->pkm_en_tratamiento = heap_extraer_raiz(simulador->pkm_recepcionados);
    }
    
    
    (simulador->estadisticas.entrenadores_atendidos)++;
    simulador->estadisticas.pokemon_en_espera = (unsigned)simulador->pkm_recepcionados->tope;

    return EXITO;
}


/**
 * PRE: Recibe el simulador y un puntero a una variable InformacionPokemon.
 * POST: Si hay algún pokemón en tratamiento carga su nombre y el de su entrenador en la
 * info recibida, caso contrario los carga con NULO.
 * Devuelve la info.
 */
InformacionPokemon* obtener_info_pokemon_en_tratamiento(simulador_t* simulador, InformacionPokemon* info){
    if(!simulador->pkm_en_tratamiento){
        info->nombre_entrenador = NULO;
        info->nombre_pokemon = NULO;
        return info;
    }
    
    info->nombre_pokemon = simulador->pkm_en_tratamiento->nombre_pokemon;
    info->nombre_entrenador = simulador->pkm_en_tratamiento->nombre_entrenador;

    return info;
}


/**
 * PRE: Recibe el id de la dificultad buscada y la lista de estas.
 * POST: Crea un iterador y recorre las dificultades hasta que el id_buscado sea 0, 
 * restándole 1 en cada iteración o hasta que no haya más elementos.
 * Si terminada la iteración el id_buscado no es 0 devuelve NULO ya que no existen tantas
 * dificultades como la buscada.
 * Caso contrario devuelve el último elemento iterado que es la difultad buscada.
 */
void* buscar_dificultad_segun_id(lista_t* dificultades, int id_buscado){
    lista_iterador_t* iterador = lista_iterador_crear(dificultades);
    while( (id_buscado > 0) && (lista_iterador_tiene_siguiente(iterador)) ){
        lista_iterador_avanzar(iterador);
        id_buscado--;
    }

    if(id_buscado > 1){
        lista_iterador_destruir(iterador);
        return NULO;
    } 

    void* dificultad_buscada = lista_iterador_elemento_actual(iterador);
    lista_iterador_destruir(iterador);
    
    
    return dificultad_buscada;    
}


/**
 * PRE: Recibe el simulador y actual cargado con el nivel adivinado.
 * POST: Busca la dificultad en uso y verifica si el nivel adivinado es correcto.
 * Si es correcto libera la memoria ocupada por el pokemon_en_tratamiento y lo reemplaza
 * por el elemento en la raíz del heap pkm_recepcionados.
 * Carga al Intento* actual según corresponda, resetea la cantidad_intentos_pkm_actual a 0
 * y actualiza las estadísticas según corresponda.
 * 
 * Si no es correcto carga Intento* actual como corresponda.
 */
int adivinar_nivel_pkm(simulador_t* simulador, Intento* actual){
    (simulador->cantidad_intentos_pkm_actual)++;
    DatosDificultad* en_uso = buscar_dificultad_segun_id(simulador->dificultades, simulador->info_dificultad.id);


    int resultado = en_uso->verificar_nivel(actual->nivel_adivinado, (unsigned)simulador->pkm_en_tratamiento->nivel);
    actual->resultado_string = en_uso->verificacion_a_string(resultado);
   
    if(resultado == 0){
        actual->es_correcto = true;
        
        free(simulador->pkm_en_tratamiento->nombre_entrenador);
        free(simulador->pkm_en_tratamiento->nombre_pokemon);
        free(simulador->pkm_en_tratamiento);

        simulador->pkm_en_tratamiento = heap_extraer_raiz(simulador->pkm_recepcionados);


        (simulador->estadisticas.puntos) += en_uso->calcular_puntaje(simulador->cantidad_intentos_pkm_actual);
        (simulador->estadisticas.pokemon_atendidos)++;
        
        simulador->cantidad_intentos_pkm_actual = 0;
        
        if(simulador->pkm_en_tratamiento){
            (simulador->estadisticas.pokemon_en_espera)--;    
        }

    } 
    else{
        actual->es_correcto = false;
    }

    return EXITO;
}


/**
 * PRE: Recibe el simulador y un DatosDificultad* datos.
 * POST: Reserva la memoria necesaria para la dificultad recibida y su nombre y la
 * agrega a la lista de dificultades.
 * Devuelve ERROR si hubo algún problema o EXITO si todo salió bien.
 */
int agregar_dificultad(simulador_t* simulador, DatosDificultad* datos){
    DatosDificultad* nueva_dificultad = malloc(sizeof(DatosDificultad));
    if(!nueva_dificultad) return ERROR;

    char* nombre = reservar_memoria_nombre(strlen(datos->nombre)+1);
    if(!nombre){
        free(nueva_dificultad);
        return ERROR;
    }
    strcpy(nombre, datos->nombre);

    nueva_dificultad->nombre = nombre;
    nueva_dificultad->verificar_nivel = datos->verificar_nivel;
    nueva_dificultad->verificacion_a_string = datos->verificacion_a_string;
    nueva_dificultad->calcular_puntaje = datos->calcular_puntaje;

    lista_insertar(simulador->dificultades, (void*)nueva_dificultad);

    return EXITO;
}


/**
 * PRE: Recibe el id de la dificultad buscada y el simulador.
 * POST: Libera la dificultad vieja y carga en la dificultad en uso la dificultad correspondiente
 * al id pasado, si esta no existe la carga como inválida y devuelve ERROR.
 * En caso de error devuelve ERROR, y si no hay problema devuelve EXITO.
 */
int seleccionar_nueva_dificultad(simulador_t* simulador, int* id_buscado){
    
    DatosDificultad* nueva_en_uso = buscar_dificultad_segun_id(simulador->dificultades, *id_buscado);
    
    free((char*)simulador->info_dificultad.nombre_dificultad);

    if(!nueva_en_uso){
        simulador->info_dificultad.nombre_dificultad = NULO;
        simulador->info_dificultad.id = DIFICULTAD_INVALIDA;
        simulador->info_dificultad.en_uso = false;
        return ERROR;
    }

    char* nombre = reservar_memoria_nombre(strlen(nueva_en_uso->nombre)+1);
    if(!nombre) return ERROR;

    strcpy(nombre, nueva_en_uso->nombre);
    
    simulador->info_dificultad.nombre_dificultad = nombre; 
    simulador->info_dificultad.id = *id_buscado;
    

    return EXITO;
}


/**
 * PRE: Recibe el simulador y un InformacionDificultad* con el el id de la dificultad 
 * buscada cargado.
 * POST: Si la dificultad no existe carga en el InformacionDificultad* recibido datos 
 * inválidos. O los datos correspondientes a la dificultad encontrada si existe.
 * Devuelve ERROR en caso de error o EXITO si no hay problemas.
 */
int obtener_info_dificultad(simulador_t* simulador, InformacionDificultad* info){
    DatosDificultad* dificultad = buscar_dificultad_segun_id(simulador->dificultades, info->id);
    if(!dificultad){
        info->nombre_dificultad = NULO;
        info->id = DIFICULTAD_INVALIDA;
        info->en_uso = false;
        return ERROR;
    }

    info->nombre_dificultad = dificultad->nombre;
    if(simulador->info_dificultad.id == info->id){
        info->en_uso = true;
    }
    else{
        info->en_uso = false;
    }

    return EXITO;
}





ResultadoSimulacion simulador_simular_evento(simulador_t* simulador, EventoSimulacion evento, void* datos){
    if( !simulador || !simulador->simulando ) return ErrorSimulacion;
    (simulador->estadisticas.cantidad_eventos_simulados)++;

    switch(evento){

        case ObtenerEstadisticas:
            if(!datos) return ErrorSimulacion;
        
            obtener_estadisticas(simulador, datos);
        
        break;

        case AtenderProximoEntrenador:
            if(simulador->estadisticas.entrenadores_totales == simulador->estadisticas.entrenadores_atendidos) return ErrorSimulacion;
            
            atender_proximo_entrenador(simulador);
        
        break;

        case ObtenerInformacionPokemonEnTratamiento:
            if(!datos) return ErrorSimulacion;
            else if(!simulador->pkm_en_tratamiento){
                obtener_info_pokemon_en_tratamiento(simulador, datos);
                return ErrorSimulacion;
            } 

            obtener_info_pokemon_en_tratamiento(simulador, datos);
        
        break;

        case AgregarDificultad:
            if(!datos) return ErrorSimulacion;
            
            agregar_dificultad(simulador, datos);
        
        break;

        case AdivinarNivelPokemon:
            if( (!datos) || (!simulador->pkm_en_tratamiento) ) return ErrorSimulacion;
            
            adivinar_nivel_pkm(simulador, datos);
        
        break;

        case SeleccionarDificultad:
            if(!datos) return ErrorSimulacion;

            if(seleccionar_nueva_dificultad(simulador, datos) == ERROR) return ErrorSimulacion;

        break;

        case ObtenerInformacionDificultad:
            if(!datos) return ErrorSimulacion;
            
            if( obtener_info_dificultad(simulador, datos) == ERROR) return ErrorSimulacion;

        break;

        case FinalizarSimulacion:
            simulador->simulando = false;
        
        break;

        default:
            return ErrorSimulacion;
        break;
       
    }

    return ExitoSimulacion;
}





/**
 * Recibe el heap de pkm_recepcionados y libera la memoria utilizada por cada elemento.
 */
void liberar_elementos_heap(heap_t* heap){
    if(heap->tope > 0){
        pokemon_t* pkm_a_eliminar = heap->vector[(heap->tope)-1];
        free(pkm_a_eliminar->nombre_entrenador);
        free(pkm_a_eliminar->nombre_pokemon);
        free(pkm_a_eliminar);

        (heap->tope)--;
        liberar_elementos_heap(heap);
    }
}

/**
 * PRE: Recibe el elemento en la posición actual de un iterador de la lista.
 * POST: Si el elemento no es nulo, libera la memoria que ocupa.
 * Devuelve true.
 */
bool destruir_elemento_lista(void* elemento, void* contexto){
    if(elemento){
        DatosDificultad* actual = elemento;
        free((char*)actual->nombre);
        free(actual);
    }
    return true;
}

/**
 * Destruye el simulador y toda la memoria ocupada por él.
 */
void simulador_destruir(simulador_t* simulador){
    if(!simulador) return;

    liberar_elementos_heap(simulador->pkm_recepcionados);
    heap_destruir(simulador->pkm_recepcionados);


    if(simulador->pkm_en_tratamiento){
        free(simulador->pkm_en_tratamiento->nombre_pokemon);
        free(simulador->pkm_en_tratamiento->nombre_entrenador);
    }
    free(simulador->pkm_en_tratamiento);


    lista_con_cada_elemento(simulador->dificultades, destruir_elemento_lista, NULL);
    lista_destruir(simulador->dificultades);


    hospital_destruir(simulador->hospital);


    if(simulador->info_dificultad.nombre_dificultad){
        free((char*)(simulador->info_dificultad.nombre_dificultad));
    }


    free(simulador);
}

