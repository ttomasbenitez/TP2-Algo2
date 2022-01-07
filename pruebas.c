#include "pa2mm.h"
#include "src/hospital.h"
#include "src/simulador.h"
#include "src/struct_simulador.h"
#include "src/struct_hospital.h"

#include "string.h"
#include <stdbool.h>



#define ES_GRANDE 1
#define ADIVINADO 0
#define ES_CHICO -1



//*****************PRUEBAS TP1********************


bool ignorar_pokemon(pokemon_t* p){
    p = p;
    return true;
}

/* No intenten esto en sus casas */
/* Ya vamos a ver como evitar esto en el TDA Lista */
struct{
    pokemon_t* pokemon[500];
    size_t cantidad;
} acumulados;

void resetear_acumulados(){
    acumulados.cantidad = 0;
}

bool acumular_pokemon(pokemon_t* p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return true;
}

bool acumular_pokemon_hasta_miltank(pokemon_t* p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return strcmp(pokemon_nombre(p), "miltank");
}

bool acumulados_en_orden_correcto(){
    if(acumulados.cantidad < 2)
        return true;
    pokemon_t* anterior = acumulados.pokemon[0];
    for(int i=1;i<acumulados.cantidad;i++){
        pokemon_t* actual =  acumulados.pokemon[i];
        if(strcmp(pokemon_nombre(anterior), pokemon_nombre(actual)) > 0)
            return false;
    }
    return true;
}

/* Pruebas */

void puedoCrearYDestruirUnHospital(){
    hospital_t* h=NULL;

    pa2m_afirmar((h=hospital_crear()), "Crear un hospital devuelve un hospital");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital se crea con cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital se crea con cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema(){
    hospital_t* h=NULL;

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital NULL tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital NULL tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon de un hospital NULL resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/archivo_vacio.hospital"), "Puedo leer un archivo vacío");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital vacío tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital vacío tiene tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/un_entrenador.hospital"), "Puedo leer un archivo con un entrenador");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==1, "El hospital tiene 1 entrenador");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==3, "El hospital tiene 3 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==3, "Recorrer los pokemon resulta en 3 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}

void dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==5, "El hospital tiene 5 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==24, "El hospital tiene 24 pokemon");
    

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==24, "Recorrer los pokemon resulta en 24 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}


void dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer otro archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un tercer archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==15, "El hospital tiene 15 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==72, "El hospital tiene 72 pokemon");

    

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==72, "Recorrer los pokemon resulta en 72 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}


bool pkm_actual_es_miltank(pokemon_t* pkm){
    if(strcmp(pkm->nombre_pokemon, "miltank") == 0){
        return false;
    }
    return true;
}

void Puedo_Recorrer_el_Hospital_Correctamente(){
    hospital_t* h=hospital_crear();

    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");

    size_t pkm_recorridos = hospital_a_cada_pokemon(h, pkm_actual_es_miltank);
    pa2m_afirmar(pkm_recorridos == 14, "Recorrer el hospital hasta miltank recorre 14 pokemones");

    hospital_destruir(h);
}



//*********************PRUEBAS SIMULADOR*******************



void Simulador_Se_Inicializa_Correctamente(){
    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t* simulador = simulador_crear(hospital);

    DatosDificultad* actual = simulador->dificultades->nodo_inicio->elemento; 
    DatosDificultad* dos = simulador->dificultades->nodo_inicio->siguiente->elemento; 
    DatosDificultad* tres = simulador->dificultades->nodo_inicio->siguiente->siguiente->elemento; 
    
    pa2m_afirmar( (strcmp(actual->nombre, "FACIL") == 0) && (strcmp(dos->nombre, "NORMAL") == 0) && 
    (strcmp(tres->nombre, "DIFICIL") == 0), "Las dificultades se inicializan correctamente");
  

    simulador_destruir(simulador);
}



void Estadisticas_Se_Actualizan_Correctamente(){
    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t* simulador = simulador_crear(hospital);

    EstadisticasSimulacion e;
    ResultadoSimulacion resultado = simulador_simular_evento(simulador, ObtenerEstadisticas, &e);
    
    pa2m_afirmar( (e.entrenadores_atendidos == 0) && (e.entrenadores_totales == hospital->cantidad_entrenadores) && 
    (e.pokemon_atendidos == 0) && (e.pokemon_en_espera == 0) && (e.pokemon_totales == hospital->cantidad_pokemon) && 
    (e.puntos == 0) , "Estadísticas se inicializan correctamente");
    
    pa2m_afirmar( (e.cantidad_eventos_simulados == 1) && (resultado == ExitoSimulacion) , "Pido estadísticas, la cantidad de eventos simulados es correcto y devuelve ExitoSimulacion");


    pa2m_afirmar(simulador_simular_evento(simulador, FinalizarSimulacion, NULL) == ExitoSimulacion, "Finalizar simulación devuelve ExitoSimulacion");
    
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e) == ErrorSimulacion, "Finalizo la simulación y al intentar simular un evento devuelve ErrorSimulacion");


    simulador_destruir(simulador);
}



//************************DIFULTAD A AGREGAR***************************

unsigned calcular_puntaje_dificultad_1(unsigned cantidad_intentos){
    if(cantidad_intentos >= 10) return 1;
    else if(cantidad_intentos >= 5) return 3;
    else return 5;
}

int verificar_nivel_adivinado_dificultad_1(unsigned nivel_adivinado, unsigned nivel_pokemon){
    if(nivel_adivinado == nivel_pokemon) return ADIVINADO;
    else if(nivel_adivinado > nivel_pokemon) return ES_GRANDE;
    else return ES_CHICO;
}

const char* salida_segun_verificacion_dificultad_1(int resultado_verificacion){
    if(resultado_verificacion == ES_CHICO) return "UN POCO MÁS";
    else if(resultado_verificacion == ES_GRANDE) return "UN POCO MENOS";
    else return "CORRECTO, ya se atendió al pokemón :)";
}

//***********************************************************************



void agregar_nueva_dificultad(simulador_t* simulador){
    DatosDificultad dificultad_1;
    char* nombre_dificultad = "TRANKI";

    dificultad_1.nombre = nombre_dificultad;
    dificultad_1.verificar_nivel = verificar_nivel_adivinado_dificultad_1;
    dificultad_1.verificacion_a_string = salida_segun_verificacion_dificultad_1;
    dificultad_1.calcular_puntaje = calcular_puntaje_dificultad_1;

    simulador_simular_evento(simulador, AgregarDificultad, &dificultad_1);
}



void Pruebas_con_Dificultades(){
    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t* simulador = simulador_crear(hospital);

    InformacionDificultad info;
    info.id = 4;
    
    ResultadoSimulacion resultado = simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info);
    
    pa2m_afirmar( (info.id == -1) && (!info.nombre_dificultad) && (resultado == ErrorSimulacion),
    "Pido info de dificultad inexistente, devuelve Error_Simulacion y actualiza el nombre pedido como NULL y el id como -1");

    int id_seleccionada = 5;
    pa2m_afirmar((simulador_simular_evento(simulador, SeleccionarDificultad, &id_seleccionada) == ErrorSimulacion), "Selecciono dificultad inexistente, devuelve ErrorSimulacion");

    id_seleccionada = 0;
    info.id = id_seleccionada;
    resultado = simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info);

    pa2m_afirmar((resultado == ExitoSimulacion) && (strcmp("FACIL", info.nombre_dificultad) == 0) && (!info.en_uso), "Obtengo info dificultad con id 0, el nombre es el correcto, la dificultad NO está en uso y devuelve ExitoSimulacion");

    simulador_simular_evento(simulador, SeleccionarDificultad, &id_seleccionada);
    simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info);
    
    pa2m_afirmar( (strcmp("FACIL", info.nombre_dificultad) == 0) && (info.en_uso), "Selecciono la dificultad con id 0 y al obtener su información está en uso");

    
    agregar_nueva_dificultad(simulador);
    id_seleccionada = 3;
    info.id = id_seleccionada;
    simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info);

    pa2m_afirmar((strcmp("TRANKI", info.nombre_dificultad) == 0) && (!info.en_uso), "Agrego correctamente una dificultad");
    

    simulador_simular_evento(simulador, SeleccionarDificultad, &id_seleccionada);
    simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info);

    pa2m_afirmar((strcmp("TRANKI", info.nombre_dificultad) == 0) && (info.en_uso), "Selecciono la nueva dificultad y pasa a estar en uso");



    simulador_destruir(simulador);
}



void Puedo_Adivinar_los_Pokemones(){
    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t* simulador = simulador_crear(hospital);

    EstadisticasSimulacion estadisticas;


    Intento intento;
    intento.nivel_adivinado = 4;


    pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ErrorSimulacion, "Intento adivinar pokemon sin que haya alguno en tratamiento, devuelve ErrorSimulacion");


    InformacionPokemon info_pkm;
    ResultadoSimulacion resultado = simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &info_pkm);
    
    pa2m_afirmar( (resultado == ErrorSimulacion) && (!info_pkm.nombre_pokemon) && (!info_pkm.nombre_entrenador),"Pido info pokemón en tratamiento y no hay ninguno, nombre pokemón y nombre entrenador se actualizan a NULL, devuelve ErrorSimulacion");



    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
    pa2m_afirmar( (simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ExitoSimulacion) && 
    (simulador->estadisticas.pokemon_atendidos == 0), "Atiendo un entrenador, adivinar con un nivel chico devuelve ExitoSimulacion y el pokemon sigue en tratamiento");


    resultado = simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &info_pkm);
    pa2m_afirmar( (resultado == ExitoSimulacion) && (strcmp("rampardos", info_pkm.nombre_pokemon)== 0) && 
    (strcmp("lucas", info_pkm.nombre_entrenador)== 0), "Pido info pokemón, el pokemón en tratamiento y su entrenador son correctos, se devuelve ExitoSimulacion");


    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);
    pa2m_afirmar(estadisticas.entrenadores_atendidos == 1, "La cantidad de entrenadores atendidos es correcta");
    pa2m_afirmar(estadisticas.pokemon_en_espera == 3, "La cantidad de pokemones en espera es correcta");



    intento.nivel_adivinado = 10;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);

    pa2m_afirmar( (intento.es_correcto) && (simulador->estadisticas.pokemon_atendidos == 1) && (simulador->pkm_recepcionados->tope == 2), "Adivino el nivel y deja de estar en tratamiento el pokemón adivinado");

    simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &info_pkm);
    pa2m_afirmar((strcmp("charizard", info_pkm.nombre_pokemon)== 0) && (strcmp("lucas", info_pkm.nombre_entrenador)== 0), "Pido info pokemón, el nuevo pokemón en tratamiento y su entrenador son correctos");



    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
    simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &info_pkm);
    pa2m_afirmar((strcmp("charizard", info_pkm.nombre_pokemon)== 0) && (strcmp("lucas", info_pkm.nombre_entrenador)== 0)
    , "Atiendo otro entrenador, el pokemón en tratamiento sigue siendo el que estaba antes");


    intento.nivel_adivinado = 20;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &info_pkm);
    pa2m_afirmar((intento.es_correcto) && (strcmp("toxicroak", info_pkm.nombre_pokemon)== 0) && (strcmp("valen", info_pkm.nombre_entrenador)== 0), "Adivino el pokemón en tratamiento, el nuevo en tratamiento es el de menor nivel de los que están en espera");




    intento.nivel_adivinado = 20;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 43;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 45;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 59;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 65;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 85;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);


    intento.nivel_adivinado = 85;
    pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ErrorSimulacion
    , "Atiendo todos los pokemones que había en espera, al atender otro devuelve ErrorSimulacion");


    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);
    pa2m_afirmar(estadisticas.pokemon_en_espera == 0, "No hay pokemones en espera");
    pa2m_afirmar(estadisticas.pokemon_atendidos == 8, "La cantidad de pokemones atendidos es correcta");
    pa2m_afirmar(estadisticas.entrenadores_atendidos == 2, "La cantidad de entrenadores atendidos es correcta");
    pa2m_afirmar(estadisticas.cantidad_eventos_simulados == 20, "La cantidad de eventos simulados es correcta");


    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
    resultado = simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
    ResultadoSimulacion resultado_2 = simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

    pa2m_afirmar( (resultado == ExitoSimulacion) && (resultado_2 == ErrorSimulacion), "Atiendo a todos los entrenadores restantes y devuelve ExitoSimulacion, intento atender a uno de más y devuelve ErrorSimulacion");


    pa2m_nuevo_grupo("Adivinar pokemones con diferentes dificultades");


    agregar_nueva_dificultad(simulador);
    int id_dificultad = 3;
    simulador_simular_evento(simulador, SeleccionarDificultad, &id_dificultad);


    intento.nivel_adivinado = 20;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    pa2m_afirmar((strcmp(intento.resultado_string, "UN POCO MENOS") == 0) && (!intento.es_correcto), "Con la dificultad agregada me paso en el nivel adivinado, el string resultado es el esperado");


    intento.nivel_adivinado = 10;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    pa2m_afirmar((strcmp(intento.resultado_string, "UN POCO MÁS") == 0) && (!intento.es_correcto), "Con la dificultad agregada me falta para adivinar el nivel, el string resultado es el esperado");




    intento.nivel_adivinado = 14;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 22;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 28;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 29;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);


    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);
    pa2m_afirmar(estadisticas.pokemon_atendidos == 12, "Agrego una dificultad, la selecciono y adivino 4 pokemones");


    id_dificultad = 0;
    simulador_simular_evento(simulador, SeleccionarDificultad, &id_dificultad);


    intento.nivel_adivinado = 35;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    pa2m_afirmar((strcmp(intento.resultado_string, "Estás dentro del rango de 3 niveles") == 0) && (!intento.es_correcto), "Con dificultad FACIL me paso en el nivel adivinado, el string resultado es el esperado");

    intento.nivel_adivinado = 15;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    pa2m_afirmar((strcmp(intento.resultado_string, "Estás dentro del rango de 20 niveles") == 0) && (!intento.es_correcto), "Con dificultad FACIL me falta para adivinar el nivel, el string resultado es el esperado");




    intento.nivel_adivinado = 32;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 32;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 32;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 33;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 35;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);



    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);
    pa2m_afirmar(estadisticas.pokemon_atendidos == 17, "Selecciono la dificultad FACIL y adivino 5 pokemones");



    id_dificultad = 1;
    simulador_simular_evento(simulador, SeleccionarDificultad, &id_dificultad);


    intento.nivel_adivinado = 55;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    pa2m_afirmar((strcmp(intento.resultado_string, "Te re pasaste") == 0) && (!intento.es_correcto), "Con dificultad NORMAL me paso en el nivel adivinado, el string resultado es el esperado");

    intento.nivel_adivinado = 42;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    pa2m_afirmar((strcmp(intento.resultado_string, "Te falta un poco") == 0) && (!intento.es_correcto), "Con dificultad NORMAL me falta un poco en el nivel adivinado, el string resultado es el esperado");



    intento.nivel_adivinado = 43;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 43;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 48;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 52;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);


    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);
    pa2m_afirmar(estadisticas.pokemon_atendidos == 21, "Selecciono la dificultad NORMAL y adivino 4 pokemones");


    id_dificultad = 2;
    simulador_simular_evento(simulador, SeleccionarDificultad, &id_dificultad);


    intento.nivel_adivinado = 55;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    pa2m_afirmar((strcmp(intento.resultado_string, "No es.") == 0) && (!intento.es_correcto), "Con dificultad DIFICIL me paso en el nivel adivinado, el string resultado es el esperado");


    intento.nivel_adivinado = 54;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 86;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
    intento.nivel_adivinado = 93;
    simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);


    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);
    pa2m_afirmar(estadisticas.pokemon_atendidos == 24, "Selecciono la dificultad DIFICIL y adivino 3 pokemones");
    pa2m_afirmar(estadisticas.pokemon_atendidos == estadisticas.pokemon_totales, "Se atendieron todos los pokemones");



    simulador_destruir(simulador);
}



void Puedo_Crear_un_Simulador_NULL(){
    simulador_t* simulador = simulador_crear(NULL);
    pa2m_afirmar(!simulador, "No puedo crear un simulador con un hospital NULL");

    pa2m_afirmar( (simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ErrorSimulacion) && (simulador_simular_evento(simulador, FinalizarSimulacion, NULL) == ErrorSimulacion)
    , "Simular un evento con un simulador inválido devuelve ErrorSimulacion");
}



void Puedo_Enviar_Datos_Invalidos(){
    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t* simulador = simulador_crear(hospital);


    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, NULL) == ErrorSimulacion, "Pido información dificultad con dato NULL, devuelve ErrorSimulacion");

    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, NULL) == ErrorSimulacion, "Pido información pokemón en tratamiento con dato NULL, devuelve ErrorSimulacion");

    pa2m_afirmar(simulador_simular_evento(simulador, SeleccionarDificultad, NULL) == ErrorSimulacion, "Selecciono dificultad con dato NULL, devuelve ErrorSimulacion");

    pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, NULL) == ErrorSimulacion, "Intento adivinar nivel pokemón con dato NULL, devuelve ErrorSimulacion");

    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, NULL) == ErrorSimulacion, "Pido estadísticas con dato NULL, devuelve ErrorSimulacion");


    EstadisticasSimulacion estadisticas;
    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);
    pa2m_afirmar(estadisticas.cantidad_eventos_simulados == 6,"Al enviar un dato inválido cuenta igual como evento simulado");

    simulador_destruir(simulador);
}


//*************************PRUEBA realloc() HEAP**************************

void Heap_Se_Agranda_Correctamente(){
    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    heap_t* heap = crear_heap(2);
    
    lista_iterador_t* iterador = lista_iterador_crear(hospital->sala_de_espera);
    while(lista_iterador_tiene_siguiente(iterador)){
        heap_insertar(heap, lista_iterador_elemento_actual(iterador));
        lista_iterador_avanzar(iterador);    
    }
    
    pa2m_afirmar(heap->tamanio != 2, "El heap se agrandó correctamente");
    pa2m_afirmar( (heap->tope == 24) && (strcmp("lucas", ((pokemon_t*)heap->vector[0])->nombre_entrenador)== 0) &&
    (strcmp("pucci", ((pokemon_t*)heap->vector[1])->nombre_entrenador)== 0) && (strcmp("valen", ((pokemon_t*)heap->vector[2])->nombre_entrenador)== 0), "Se agregaron correctamente todos los elementos");

    lista_iterador_destruir(iterador);
    hospital_destruir(hospital);
    heap_destruir(heap);
}


int main(){
    pa2m_nuevo_grupo("PRUEBAS HOSPITAL");


    pa2m_nuevo_grupo("Pruebas de  creación y destrucción");
    puedoCrearYDestruirUnHospital();

    pa2m_nuevo_grupo("Pruebas con NULL");
    dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema();

    pa2m_nuevo_grupo("Pruebas con un archivo vacío");
    dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de un entrenador");
    dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de varios entrenadores");
    dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital();
    Puedo_Recorrer_el_Hospital_Correctamente();


    pa2m_nuevo_grupo("Pruebas con mas de un archivo");
    dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital();



    pa2m_nuevo_grupo("PRUEBAS SIMULADOR");


    pa2m_nuevo_grupo("Pruebas inicialización de Simulador");
    Simulador_Se_Inicializa_Correctamente();
    Estadisticas_Se_Actualizan_Correctamente();


    pa2m_nuevo_grupo("Pruebas de Dificultades");
    Pruebas_con_Dificultades();


    pa2m_nuevo_grupo("Adivinar nivel Pokemón");
    Puedo_Adivinar_los_Pokemones();


    pa2m_nuevo_grupo("Pruebas con datos inválidos y NULL");
    Puedo_Crear_un_Simulador_NULL();
    Puedo_Enviar_Datos_Invalidos();


    pa2m_nuevo_grupo("Pruebas redimensionamiento heap");
    Heap_Se_Agranda_Correctamente();


    return pa2m_mostrar_reporte();
}
