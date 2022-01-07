#include "src/hospital.h"
#include "src/struct_hospital.h"
#include "src/simulador.h"
#include "src/struct_simulador.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define CARACTER_INVALIDO '\n'

#define ES_GRANDE 1
#define ADIVINADO 0
#define ES_CHICO -1

#define CALIENTE 3
#define TIBIO 6
#define FRIO 7


typedef struct juego{
    simulador_t* simulador;
    bool jugando;
}Juego;


char leer_comando(){
    char linea[100];
    char* leido;
    leido = fgets(linea, 100, stdin);
    if(!leido)
        return 0;
    while(*leido == ' ')
        leido++;
    return (char)tolower(*leido);
}


/**
 * Crea el simulador de Juego y actualiza jugando a true.
 */
void inicializar_juego(Juego* juego, hospital_t* hospital){
    juego->simulador = simulador_crear(hospital);
    juego->jugando = true;
}


/**
 * Define una variable EstadisticasSimulacion y la llena con la información del simulador
 * simulando el evento ObtenerEstadisticas.
 * Obtiene las estadísticas del simulador y las imprime por pantalla.
 */
void mostrar_estadisticas(Juego* juego){
    EstadisticasSimulacion e;
    simulador_simular_evento(juego->simulador, ObtenerEstadisticas, &e);

    
    printf("Pokemones totales: %u\n", e.pokemon_totales);
    printf("Pokemones en espera: %u\n", e.pokemon_en_espera);
    printf("Pokemones atendidos: %u\n", e.pokemon_atendidos);
    printf("¿Hay algún pokemón en tratamiento? ");{
        if(juego->simulador->pkm_en_tratamiento){
            printf("Sí\n");
        }
        else{
            printf("No\n");
        }
    }
    printf("Entrenadores totales: %u\n", e.entrenadores_totales);
    printf("Entrenadores atendidos: %u\n", e.entrenadores_atendidos);
    printf("Cantidad de eventos simulados: %u\n", e.cantidad_eventos_simulados);
    printf("Puntos: %u\n", e.puntos);
}


/**
 * Si hay algún entrenador por atender, simula el evento de AtenderProximoEntrenador.
 */
void atender_prox_entrenador(Juego* juego){
    if(juego->simulador->estadisticas.entrenadores_totales == juego->simulador->estadisticas.entrenadores_atendidos){
        printf("No hay más entrenadores por atender\n");
        return;
    }
    
    simulador_simular_evento(juego->simulador, AtenderProximoEntrenador, NULL);
    printf("Se atendió un entrenador correctamente\n");
}


/**
 * Define una variable InformacionPokemon y la llena con la información del simulador
 * simulando el evento ObtenerInformacionPokemonEnTratamiento.
 * Obtiene la info del pokemón en tratamiento e imprime por pantalla según corresponda.
 */
void info_pokemon_en_tratamiento(Juego* juego){
    InformacionPokemon i;
    simulador_simular_evento(juego->simulador, ObtenerInformacionPokemonEnTratamiento, &i);
    if(!(i.nombre_pokemon)){
        printf("No hay ningún pokemón en tratamiento, atendé un entrenador para que lo haya\n");
        return;
    }

    printf("El Pokemon en tratamiento es: %s, y su Entrenador es: %s\n", i.nombre_pokemon, i.nombre_entrenador);
}



//*********************************DIFICULTADES**************************************

unsigned calcular_puntaje_dificultad_1(unsigned cantidad_intentos){
    if(cantidad_intentos >= 10) return 1;
    else if(cantidad_intentos >= 5) return 3;
    else return 5;
}

unsigned calcular_puntaje_dificultad_2(unsigned cantidad_intentos){
    if(cantidad_intentos >= 10) return 2;
    else if(cantidad_intentos >= 5) return 5;
    else return 10;
}



int verificar_nivel_adivinado_dificultad_1(unsigned nivel_adivinado, unsigned nivel_pokemon){
    if(nivel_adivinado == nivel_pokemon) return ADIVINADO;
    else if(nivel_adivinado > nivel_pokemon) return ES_GRANDE;
    else return ES_CHICO;
}

int verificar_nivel_adivinado_dificultad_2(unsigned nivel_adivinado, unsigned nivel_pokemon){
    if(nivel_adivinado == nivel_pokemon) return ADIVINADO;
    else if( abs((int)nivel_adivinado - (int)nivel_pokemon) <= CALIENTE ) return CALIENTE;
    else if( abs((int)nivel_adivinado - (int)nivel_pokemon) <= TIBIO ) return TIBIO;
    else return FRIO;
}



const char* salida_segun_verificacion_dificultad_1(int resultado_verificacion){
    if(resultado_verificacion == ES_CHICO) return "UN POCO MÁS";
    else if(resultado_verificacion == ES_GRANDE) return "UN POCO MENOS";
    else return "CORRECTO, ya se atendió al pokemón :)";
}

const char* salida_segun_verificacion_dificultad_2(int resultado_verificacion){
    if(resultado_verificacion == CALIENTE) return "CALIENTE";
    else if(resultado_verificacion == TIBIO) return "TIBIO";
    else if(resultado_verificacion == FRIO) return "FRIO";
    else return "CORRECTO, ya se atendió al pokemón :)";
}


/**
 * Define una variable DatosDificultad y la llena con la información correspondiente
 * a la dificultad creada, la agrega al simulador simulando el evento AgregarDificultad.
 */
void agregar_nueva_dificultad(Juego* juego){
    DatosDificultad dificultad_1;
    char* nombre_dificultad = "TRANKI";

    dificultad_1.nombre = nombre_dificultad;
    dificultad_1.verificar_nivel = verificar_nivel_adivinado_dificultad_1;
    dificultad_1.verificacion_a_string = salida_segun_verificacion_dificultad_1;
    dificultad_1.calcular_puntaje = calcular_puntaje_dificultad_1;

    simulador_simular_evento(juego->simulador, AgregarDificultad, &dificultad_1);
}

/**
 * Define otra variable DatosDificultad y la llena con la información correspondiente
 * a la otra dificultad creada, la agrega al simulador simulando el evento AgregarDificultad.
 */
void agregar_otra_dificultad(Juego* juego){
    DatosDificultad dificultad_2;
    char* nombre_dificultad = "FRIO/CALOR";

    dificultad_2.nombre = nombre_dificultad;
    dificultad_2.verificar_nivel = verificar_nivel_adivinado_dificultad_2;
    dificultad_2.verificacion_a_string = salida_segun_verificacion_dificultad_2;
    dificultad_2.calcular_puntaje = calcular_puntaje_dificultad_2;

    simulador_simular_evento(juego->simulador, AgregarDificultad, &dificultad_2);
}

//****************************************************************************


/**
 * Si el simulador tiene algún pokemón en tratamiento, pide al usuario un nivel y lo
 * carga en una variable Intento, simula el evento AdivinarNivelPokemon e imprime por
 * pantalla el resultado del intento. 
 * Mientras no se adivine el nivel del pokemón repite el proceso.
 */
void intentar_adivinar_nivel_pkm(Juego* juego){
    if(!juego->simulador->pkm_en_tratamiento){
        printf("No hay ningún pokemón en tratamiento, hay que atender primero a un entrenador\n");
        return;
    }

    bool se_adivino = false;
    do{
        printf("Decime que nivel creés que es el Pokemón que está siendo atendido:\n");
        
        unsigned nivel;
        scanf("%u", &nivel);

        Intento actual;
        actual.nivel_adivinado = nivel;

        simulador_simular_evento(juego->simulador, AdivinarNivelPokemon, &actual);
        
        se_adivino = actual.es_correcto;
        printf("%s\n", actual.resultado_string);
    }while(!se_adivino);

}


/**
 * Define una variable InformacionDificultad y la llena con la el id de cada dificultad disponible.
 * Simula el evento ObtenerInformacionDificultad y mientras la dificultad exista la imprime
 * por pantalla.
 */
void mostrar_dificultades_disponibles(Juego* juego, int id_buscada, int* cantidad_dificultades){
    InformacionDificultad info;
    
    info.id = id_buscada;
    simulador_simular_evento(juego->simulador, ObtenerInformacionDificultad, &info);
    
    if(info.nombre_dificultad){
        printf("\n(%i). Nombre dificultad: %s", id_buscada, info.nombre_dificultad);
        if(info.en_uso){
            printf("(en uso)");
        }
        (*cantidad_dificultades)++;
        mostrar_dificultades_disponibles(juego, id_buscada+1, cantidad_dificultades);
    }
    
}


/**
 * Pide al usuario el id de la dificultad que quiere usar.
 * Simula el evento SeleccionarDificultad y si esta existe pasa a ser la dificultad en
 * uso.
 */
void seleccionar_dificultad(Juego* juego){
    int cantidad_dificultades = 0;
    mostrar_dificultades_disponibles(juego, 0, &cantidad_dificultades);
    printf("\n\n¿Qué dificultad querés usar? Apretá el número que corresponda.\n");
    
    int id_buscada = 0;
    scanf("%d", &id_buscada);
    
    if(id_buscada > cantidad_dificultades){
        printf("\n\n\nLa id pasada no corresponde a ninguna dificultad\n");
        printf("La nueva dificultad actual es inválida\n");
    }
    simulador_simular_evento(juego->simulador, SeleccionarDificultad, &id_buscada);
}


/**
 * Pide al usuario el id de la dificultad que quiere usar, y la carga en una variable
 * InformacionDificultad.
 * Simula el evento ObtenerInformacionDificultad y si esta existe imprime por pantalla
 * su información.
 */
void mostrar_dificultad_buscada(Juego* juego){
    printf("Decime el id de la dificultad de la que querés información\n");
    int id_buscada;
    scanf("%d", &id_buscada);

    InformacionDificultad info;
    info.id = id_buscada;
    simulador_simular_evento(juego->simulador, ObtenerInformacionDificultad, &info);
    
    if(!info.nombre_dificultad){
        printf("\n");
        printf("No existe ninguna dificultad con ese id\n");
        return;
    }

    printf("Nombre dificultad: %s. ¿Está en uso?: ", info.nombre_dificultad);
    if(info.en_uso){
        printf("Sí\n");
    }
    else{
        printf("No\n");
    }
}


/**
 * Simula el evento FinalizarSimulacion que termina la simulación del juego.
 */
void finalizar_simulacion_juego(Juego* juego){
    system("clear");
    simulador_simular_evento(juego->simulador, FinalizarSimulacion, NULL);
    printf("La simulación terminó\n\n\n\nPuntos Totales: %u\n", juego->simulador->estadisticas.puntos);
    juego->jugando = false;
}


void mostrar_menu(){
    printf("\n\n");
    
    printf("Lista de Opciones disponibles\n");
    printf("E --> Mostrar estadísticas\nP --> Atender próximo entrenador\nI --> Información Pokemón en tratamiento\n");
    printf("A --> Adivinar nivel Pokemón\nD --> Seleccionar dificultad a usar\n");
    printf("O --> Obtener información dificultad\nQ --> Finalizar simulación\n");
}


/**
 * Recibe un comando y si este existe lo ejecuta. 
 */
void ejecutar_comando(Juego *juego, char comando){
    if(comando == CARACTER_INVALIDO) return;
    

    switch (comando) {
        case 'q': 
            finalizar_simulacion_juego(juego);
            return;
        break;

        case 'e':
            system("clear");
            mostrar_estadisticas(juego);
        break;

        case 'p':
            system("clear");
            atender_prox_entrenador(juego);
        break;

        case 'i':
            system("clear");
            info_pokemon_en_tratamiento(juego);
        break;

        case 'a':
            system("clear");
            intentar_adivinar_nivel_pkm(juego);
        break;

        case 'd':
            system("clear");
            seleccionar_dificultad(juego);
        break;

        case 'm':
            system("clear");
            mostrar_menu();
        break;

        case 'o':
            system("clear");
            mostrar_dificultad_buscada(juego);
        break;

        default: 
            system("clear");
            printf("Esa no es una opción válida\n");
        break;
    }

    if(comando != 'm'){
        printf("\n\n\n\n\n\n\n\n\n\n");
        printf("Apretar 'M' para ver el Menú\n\n");
    }
}



void destruir_juego(Juego* juego){
    if(!juego) return;

    simulador_destruir(juego->simulador);
}



int main(int argc, char *argv[]) {

    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");
    hospital_leer_archivo(hospital, "ejemplos/un_entrenador.hospital");

    Juego juego;
    inicializar_juego(&juego, hospital);

    agregar_nueva_dificultad(&juego);
    agregar_otra_dificultad(&juego);

    mostrar_menu();

    do{
        printf("\n¿Qué querés hacer? > ");
        char c = leer_comando();
        ejecutar_comando(&juego, c);
    }while(juego.jugando);
    
    destruir_juego(&juego);

    return 0;
}
