// proyecto: Grupal/Juego
// arhivo:   JuegoMesa.cpp
// versión:  1.1  (9-Ene-2023)


#include "juego.h"

#include <iostream>
#include <cstdlib>
#include <ctime>


namespace juego {


    /*******************************************************************************************
    /*******************************************************************************************
        
    CONFIGURACIÓN DEL JUEGO
    Primera sección - Los recursos

    En esta sección se establecen las carpetas en las que aparecen los recursos usados por el 
    juego, incluyendo las imágenes que aparecen en pantalla y los sonidos que se escuchan. Hay 
    una carpeta separada para los retratos de los personajes y otra para las imágenes de las 
    habilidades.

    Las carpetas están situadas en la carpeta donde están los archivos de código fuente del 
    proyecto.

    En principio no es necesario hacer ningún cambio en esta sección, si los recursos que se 
    cambian se sitúan en las mismas carpetas.

    *******************************************************************************************/
    string JuegoMesa::carpeta_activos_juego  {"./activos/"};
    string JuegoMesa::carpeta_retratos_juego {"./retratos/"};
    string JuegoMesa::carpeta_habilids_juego {"./habilidades/"};
    string JuegoMesa::carpeta_sonidos_juego  {"./sonidos/"};
    /*******************************************************************************************
    /******************************************************************************************/

    int numeroAleatorio;

    const std::wstring JuegoMesa::tituloVentana () const {
        return L"DEADLY ARENA :: Juego de Grupo 4";
    }


    JuegoMesa::JuegoMesa () {
        //
        /*******************************************************************************************
        /*******************************************************************************************
        
        CONFIGURACIÓN DEL JUEGO
        Segunda sección - El modo del juego

        En esta sección se configura el modo general del juego, es decir como se suceden las 
        jugadas, los turnos y las rondas. Y como se establece el personaje que realiza cada jugada.  

        La configuración del modo del juego se realiza mediante la elección de alguno de los
        modos preestablecidos. Es la única forma de configurar el modo del juego.

        Hay tres modos de juego preestablecidos:

            1) Modo de juego por pares
            2) Modo de juego por equipos
            3) Modo de juego libre de doble jugada

        Las siguientes definiciones se aplican a todos los modos de juego:

        Jugada:   
            Es una acción realizada con uno de los personajes. Puede ser un desplazamiento o la
            aplicación de una habilidad.

        Turno:
            Es una secuencia de jugadas realizadas por un grupo de personajes, normalmente 
            alternando a los dos equipos. los turnos se inician con la elección de los personajes 
            que van a participar en el turno, aunque en algunos modos de juego estos están 
            preestablecidos.

        Ronda:
            Es una secuencia de turnos en las que intervienen todos los personajes y que se 
            prolongan hasta que los personajes no pueden continuar, por haber perdido sus puntos de 
            acción o por haber perdido su vitalidad. Al inicio de la ronda, se recuperan los puntos 
            de acción de todos los personajes.

        Partida:
            Está formada por una secuencia de rondas. La partida termina cuando todos los personajes 
            de un equipo pierden su vitalidad. El otro equipo es el ganador.


        Modo de juego por pares
        --------------------------
        
        En este modo debe haber el mismo número de personajes en cada equipo.

        los turnos comienzan con los jugadores eligiendo un personaje de cada equipo. El primer 
        personaje en jugar es el que tiene más iniciativa, y en caso de tener la misma, se elige al 
        azar. A continuación se suceden las jugadas, alternando los dos personajes. 

        Cuando un personaje pierde sus puntos de acción o su vitalidad, el turno continua solo con 
        el otro. Y cuando lo pierden los dos, el turno termina.

        En el nuevo turno los jugador elige de nuevo a los dos personajes, evitando a los que han
        perdido sus puntos de acción o vitalidad.

        Cuando todos los personajes han perdido sus puntos de acción o su vitalidad, termina la 
        ronda.


        Modo de juego por equipos
        ----------------------------

        En este modo debe haber el mismo número de personajes en cada equipo.

        Al principio de la partida se establece un orden de juego para los personajes. Se toman los 
        personajes según aparecen en el tablero, de arriba a abajo, alternando los dos equipos. El 
        primero de los personajes será el que tenga más iniciativa de los dos primeros, eligiéndolos 
        al azar si coinciden. 
        
        Un turno consiste en una jugada de cada personaje, tomándolos en el orden de la lista y se 
        prolonga hasta que juega el último de los personajes de la lista. Si un personaje ha perdido 
        sus puntos de acción o su vitalidad, la jugada es nula y se pasa al siguiente personaje de 
        la lista. 
        
        Los turnos se suceden hasta que todos los personajes han perdido sus puntos de acción o su 
        vitalidad, momento en que se inicia el siguiente ronda.


        Modo de juego libre de doble jugada
        --------------------------------------

        En este modo debe haber el mismo número de personajes en cada equipo.

        Al principio de la partida se establece un equipo inicial, tomando el que tiene el personaje 
        con más iniciativa o eligiéndolo al azar si coinciden. A continuación se alternan turnos de 
        uno y otro equipo. 
        
        Al principio de cada turno, el jugador elije un personaje del equipo correspondiente. El 
        turno consiste en dos jugadas seguidas del mismo personaje, tras las cuales se pasa a el 
        siguiente turno, en la que el jugador elige un personaje del otro equipo. Si un personaje 
        ha perdido sus puntos de acción o su vitalidad, no puede ser elegido.

        Cuando todos los personajes de un equipo han perdido sus puntos de acción o vitalidad, 
        termina el turno (a pesar de que el otro equipo si pueda continuar).


        Programación
        ------------

        Para cambiar el modo del juego es suficiente con cambiar los identificadores usados en 
        las dos líneas de código siguientes. Debe aparecer una de estas tres parejas:

            · ModoJuegoPares       SucesosJuegoPares       
            · ModoJuegoEquipo      SucesosJuegoEquipo    
            · ModoJuegoLibreDoble  SucesosJuegoLibreDoble

        *******************************************************************************************/
        ModoJuegoPares    * modo    = new ModoJuegoPares {this};
        SucesosJuegoPares * sucesos = new SucesosJuegoPares {this, modo};
        /*******************************************************************************************
        /******************************************************************************************/
        //
        JuegoMesaBase::configura (sucesos, modo);
    }


    /*******************************************************************************************
    /*******************************************************************************************
        
    CONFIGURACIÓN DEL JUEGO
    Tercera sección - El tablero - Primera parte

    En esta sección se preparan los muros que aparecen en el tablero de juego.

    En el juego aparecen imágenes que muestran muros que ocupan algunas casillas hexagonales
    del tablero. Los jugadores no pueden situar a los personajes sobre los muros, ni 
    atravesarlos. Tampoco pueden realizar ataque a través de los muros (excepto si están 
    marcados como ataques indirectos).  

    Existen muchas posibilidades de diseño para los muros. Es posible que los muros se usen solo
    para delimitar un área del tablero, como ocurre con el ejemplo que aparece a continuación,
    llamado 'grafico_muros_area_central'. Otra posibilidad es diseñar una especie de laberinto
    con pasillos y cámaras, como en el ejemplo: 'grafico_muros_pasillos'. Aunque, lo más 
    aconsejable es crear unos muros que sean simplemente obstáculos para los ataques entre los 
    personajes.

    Especificación de los muros
    ---------------------------

    Para especificar los muros del tablero se debe crear un array similar al que aparece en los 
    ejemplos. Es preferible crear uno nuevo y mantener los ejemplos, en ese caso , la variable 
    debe tener un nombre diferente.

    El array está formado por 51 cadenas de caracteres, cada una de ellas de 145 caracteres de 
    longitud (mas el carácter \0 final, que no es visible). La mayor parte de los caracteres de 
    la cadena son espacios en blanco. En determinadas posiciones, y solo en ellas, puede haber: 
    o bien un guion, o bien la letra O mayúscula. 
        
    Esta disposición está pensada para indicar las posiciones de las celdas de la rejilla del 
    tablero. Al ser celdas hexagonales, no es posible usar una tabla formada por filas y 
    columnas, que, además, no daría una representación visual clara del tablero.

    Si se quiere que en una celda del tablero no aparezca un fragmento del muro, se debe 
    escribir un guion en la posición correspondiente del array. Si, por el contrario, se quiere 
    que si aparezca, se debe escribir la letra o mayúscula.

    El programa es muy sensible a la colocación de los caracteres en el array. Si uno de ellos
    está desplazado (por poco que sea), se producirá una excepción y el programa no se podrá 
    ejecutar.

    Programación
    ------------

    Véase la tercera parte de esta sección.
        
    *******************************************************************************************/
    static ActorTablero::GraficoMuros grafico_muros_vacio { 
//                                      1  1  1  1  1  1  1  1  1  1  2  2  2  2  2  2  2  2  2  2  3  3  3  3  3  3  3  3  3  3  4  4  4  4  4  4  4  4  4  4
//           1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9
//                                                                                                                11111111111111111111111111111111111111111111
//                     111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999000000000011111111112222222222333333333344444  
//           0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",  
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -"};


    static ActorTablero::GraficoMuros grafico_muros_area_central { 
//                                      1  1  1  1  1  1  1  1  1  1  2  2  2  2  2  2  2  2  2  2  3  3  3  3  3  3  3  3  3  3  4  4  4  4  4  4  4  4  4  4
//           1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9
//                                                                                                                11111111111111111111111111111111111111111111
//                     111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999000000000011111111112222222222333333333344444  
//           0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234
            "-     -     -     -     -     -     -     -     O     O     O     O     -     O     O     O     O     -     -     -     -     -     -     -     -",  
            "   -     -     -     -     -     -     -     O     O     O     O     -     -     O     O     O     O     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     O     O     O     O     -     -     -     O     O     O     O     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     O     O     O     O     -     -     -     -     O     O     O     O     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     O     O     O     O     -     -     -     -     -     O     O     O     O     -     -     -     -     -     -",
            "   -     -     -     -     -     O     O     O     O     -     -     -     -     -     -     O     O     O     O     -     -     -     -     -   ",
            "-     -     -     -     -     O     O     O     O     -     -     -     -     -     -     -     O     O     O     O     -     -     -     -     -",
            "   -     -     -     -     O     O     O     O     -     -     -     -     -     -     -     -     O     O     O     O     -     -     -     -   ",
            "-     -     -     -     O     O     O     O     -     -     -     -     -     -     -     -     -     O     O     O     O     -     -     -     -",
            "   -     -     -     O     O     O     O     -     -     -     -     -     -     -     -     -     -     O     O     O     O     -     -     -   ",
            "-     -     -     O     O     O     O     -     -     -     -     -     -     -     -     -     -     -     O     O     O     O     -     -     -",
            "   -     -     O     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     O     -     -   ",
            "-     -     O     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     O     -     -",
            "   -     O     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     O     -   ",
            "-     O     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     O     -",
            "   -     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     -   ",
            "-     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     -",
            "   -     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     -   ",
            "-     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     -",
            "   -     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     -   ",
            "-     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     -     -",
            "   -     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     -     -   ",
            "-     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     -     -     -",
            "   -     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     -",
            "   -     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     -   ",
            "-     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     -",
            "   -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     -   ",
            "-     -     -     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     -",
            "   -     -     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     -   ",
            "-     -     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     -",
            "   -     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     -   ",
            "-     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     -",
            "   -     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     -   ",
            "-     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     -",
            "   -     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     -   ",
            "-     O     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     O     -",
            "   -     O     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     O     -   ",
            "-     -     O     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     O     -     -",
            "   -     -     O     O     O     O     -     -     -     -     -     -     -     -     -     -     -     -     O     O     O     O     -     -   ",
            "-     -     -     O     O     O     O     -     -     -     -     -     -     -     -     -     -     -     O     O     O     O     -     -     -",
            "   -     -     -     O     O     O     O     -     -     -     -     -     -     -     -     -     -     O     O     O     O     -     -     -   ",
            "-     -     -     -     O     O     O     O     -     -     -     -     -     -     -     -     -     O     O     O     O     -     -     -     -",
            "   -     -     -     -     O     O     O     O     -     -     -     -     -     -     -     -     O     O     O     O     -     -     -     -   ",
            "-     -     -     -     -     O     O     O     O     -     -     -     -     -     -     -     O     O     O     O     -     -     -     -     -",
            "   -     -     -     -     -     O     O     O     O     -     -     -     -     -     -     O     O     O     O     -     -     -     -     -   ",
            "-     -     -     -     -     -     O     O     O     O     -     -     -     -     -     O     O     O     O     -     -     -     -     -     -",
            "   -     -     -     -     -     -     O     O     O     O     -     -     -     -     O     O     O     O     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     O     O     O     O     -     -     -     O     O     O     O     -     -     -     -     -     -     -",
            "   -     -     -     -     -     -     -     O     O     O     O     -     -     O     O     O     O     -     -     -     -     -     -     -   ",
            "-     -     -     -     -     -     -     -     O     O     O     O     -     O     O     O     O     -     -     -     -     -     -     -     -"};


    static ActorTablero::GraficoMuros grafico_muros_pasillos { 
            "-     O     -     -     -     -     -     O     -     -     -     -     -     -     -     -     O     -     -     -     O     -     -     -     -",
            "   O     O     -     -     -     -     O     O     -     -     -     -     -     -     -     O     O     -     -     O     O     -     -     -   ",
            "O     -     O     -     -     -     O     -     O     -     -     -     -     -     -     O     -     O     -     O     -     O     -     -     -",
            "   -     -     O     -     -     O     -     -     O     -     -     -     -     -     O     -     -     O     -     O     -     O     -     -   ",
            "O     -     -     O     -     O     -     O     -     O     -     -     -     -     O     -     O     -     O     -     O     -     O     -     -",
            "   -     -     -     O     O     -     O     O     -     O     -     -     -     O     -     O     O     -     O     -     O     -     O     -   ",
            "O     -     -     -     O     -     O     -     O     -     O     -     -     O     -     O     -     O     -     O     -     O     -     O     -",
            "   -     -     -     -     O     O     O     -     O     -     O     -     O     -     O     -     -     O     -     O     -     O     -     O   ",
            "O     -     -     -     -     -     -     O     -     O     -     O     O     -     O     -     -     -     O     -     O     -     O     -     O",
            "   -     -     -     -     O     O     -     O     -     O     -     O     -     O     -     -     O     -     O     -     O     -     O     O   ",
            "O     -     -     -     -     -     -     O     O     -     O     -     -     O     -     -     O     O     -     O     -     O     -     O     -",
            "   -     -     -     -     O     O     O     -     O     -     O     -     O     -     -     O     -     O     -     O     -     O     -     -   ",
            "O     -     -     -     -     -     O     -     -     O     -     O     -     O     -     O     -     -     O     -     O     -     O     -     -",
            "   -     -     -     -     -     O     -     O     -     O     -     O     -     O     O     -     -     -     O     -     O     -     O     -   ",
            "O     -     -     -     -     -     -     O     O     -     O     -     O     -     O     -     -     -     -     O     -     O     -     O     -",
            "   -     -     -     -     O     O     -     -     O     -     O     -     O     -     -     -     -     -     -     O     -     O     -     -   ",
            "O     -     -     -     -     -     -     O     -     O     -     O     -     O     -     -     -     -     -     -     O     -     O     -     -",
            "   -     -     -     -     O     O     -     O     -     O     -     O     -     O     -     -     -     -     -     -     O     -     O     -   ",
            "O     -     -     -     O     -     O     -     O     -     O     -     O     -     O     -     -     -     -     -     O     O     -     O     -",
            "   O     -     -     O     -     O     O     -     O     -     O     -     O     -     -     -     -     -     -     -     -     O     -     -   ",
            "-     O     -     O     -     O     -     O     -     O     -     O     -     O     -     -     -     -     -     -     O     -     O     O     -",
            "   -     -     O     -     O     -     -     O     -     O     -     O     -     O     -     -     -     -     -     -     O     -     -     -   ",
            "-     O     O     -     O     -     -     -     O     -     O     -     O     -     O     -     -     -     -     -     -     O     O     O     -",
            "   -     O     -     O     -     -     -     -     O     -     O     -     O     -     -     -     -     -     -     -     -     O     -     -   ",
            "-     -     -     O     -     -     -     -     -     -     -     O     -     O     -     -     -     -     -     -     O     -     O     -     -",
            "   -     -     O     -     -     -     -     -     -     -     O     O     O     O     -     -     -     -     -     -     O     -     O     -   ",
            "-     O     O     O     -     -     -     -     -     -     -     -     -     -     O     -     -     -     -     -     -     O     -     O     -",
            "   -     -     -     -     -     -     -     -     -     -     O     O     O     -     -     -     -     -     -     -     -     O     -     -   ",
            "-     O     O     O     -     -     -     -     -     -     -     -     -     -     O     -     -     -     -     -     O     O     O     -     -",
            "   -     -     -     -     -     -     -     -     -     -     O     O     O     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     O     O     O     -     -     -     -     -     -     -     -     -     -     O     -     -     -     -     -     O     O     O     O     -",
            "   -     -     -     -     -     -     -     -     -     -     O     O     O     -     -     -     -     -     -     -     -     -     -     O   ",
            "-     O     O     O     -     -     -     -     -     -     -     -     -     -     O     -     -     -     -     -     O     O     O     O     O",
            "   -     -     O     -     -     -     -     -     -     -     O     O     O     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     O     -     -     -     -     -     -     -     -     -     -     -     -     O     -     -     -     -     -     O     O     O     O     O",
            "   -     O     -     -     -     -     -     -     -     -     O     O     O     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     O     -     -     -     -     -     -     -     -     -     -     -     O     -     -     -     -     -     O     O     O     O     O",
            "   -     -     O     -     -     -     -     -     -     -     O     O     O     -     -     -     -     -     -     -     -     -     -     -   ",
            "-     O     -     O     -     -     -     -     -     -     -     O     -     O     O     -     -     -     -     -     O     O     O     O     O",
            "   -     O     -     O     -     -     -     -     -     -     O     -     -     O     -     -     -     -     -     -     -     -     -     -   ",
            "-     -     O     -     O     -     -     -     -     -     O     -     O     -     O     -     -     -     -     -     O     O     O     O     O",
            "   -     -     O     -     O     -     -     -     -     O     -     O     O     -     O     -     -     -     -     -     -     -     -     -   ",
            "-     -     -     O     -     O     -     -     -     O     -     O     -     O     -     O     -     -     -     -     O     O     O     O     O",
            "   -     -     -     O     -     O     -     -     O     -     O     -     -     O     -     O     -     O     -     O     -     O     -     -   ",
            "-     -     -     -     O     -     O     -     O     -     O     -     O     -     O     -     O     O     O     O     -     -     O     O     O",
            "   -     -     -     -     O     -     O     O     -     O     -     O     O     -     O     -     O     -     O     -     O     -     -     -   ",
            "-     -     -     -     -     O     -     O     -     O     -     O     -     O     -     O     -     -     -     -     O     O     -     O     O",
            "   -     -     -     -     -     O     -     -     -     -     O     -     -     O     -     O     -     O     -     O     -     O     O     -   ",
            "-     -     -     -     -     -     O     -     O     -     O     -     -     -     O     -     O     O     O     O     -     -     O     O     O",
            "   -     -     -     -     -     -     O     O     O     O     -     -     -     -     O     O     O     -     O     -     -     -     -     O   ",
            "-     -     -     -     -     -     -     O     -     O     -     -     -     -     -     O     -     -     -     -     -     -     -     -     -"};
    /*******************************************************************************************
    /******************************************************************************************/


    void JuegoMesa::preparaTablero () {
        agregaTablero(new ActorTablero(this));
        //
        /*******************************************************************************************
        /*******************************************************************************************

        CONFIGURACIÓN DEL JUEGO
        Tercera sección - El tablero - Segunda parte

        En esta sección se configura el fondo del tablero. Hay dos formas de establecer el fondo del
        tablero:

             1) Usando una imagen única que rellena completamente el fondo del tablero.

             2) Usando un conjunto de imágenes que se colocan en filas y columnas (en una rejilla).
                Cada imagen del conjunto aparece repetidamente (en distintas posiciones) en el fondo
                del tablero.

        Configuración con una sola imagen
        ---------------------------------

        Para rellenar el fondo del tablero se usa una única imagen.

        La imagen se situa en la esquina superior izquierda de la ventana del juego y se extiende
        en todo su tamaño. Para ajustar el área de la ventana que ocupa la imagen, se debe preparar
        previamente (mediante un programa de edición de imágenes) para que su dimensiones sean
        1710x1010 px.

        La imagen usada puede ser de tipo png o PNG.

        Configuración con imágenes en una rejilla
        -----------------------------------------

        Para rellenar el fondo del tablero se usa un conjunto de imágenes colocadas en una rejilla
        (en filas y columnas), que ocupan toda la superficie de la ventana.

        La imagen usada para rellenar el fondo del tablero no se usa directamente, se considera
        formada por un conjunto de imágenes. Para ello, se divide en 16 partes, con 4 filas y 4
        columnas, que se colocan en el fondo del tablero, seleccionado las partes de manera
        aleatoria. La razón de este funcionamiento es que se consiguen evitar los patrones visuales
        que pueden aparecer si se colocan de forma uniforme.

        El tamaño de la imagen usada es de 128x128 px (y cada parte es de 32x32 px).

        Programación con una sola imagen
        --------------------------------

        Para cambiar la imagen del fondo es suficiente con situar la imagen nueva en la carpeta de
        activos del juego, descomentar la línea de código siguiente, y cambiar el nombre del archivo.
        Además, es necesario comentar la línea de código que aparece en el apartado siguiente
        'Programación con imágenes en una rejilla'.

        *******************************************************************************************/
        //tablero ()->ponArchivoFondo (carpeta_activos_juego + "hubble_jo175_potw2318a.jpg");
        /*******************************************************************************************

        Programación con imágenes en una rejilla
        ----------------------------------------

        Para cambiar la imagen del fondo es suficiente con situar la imagen nueva en la carpeta de
        activos del juego y cambiar el nombre en la línea de código siguiente.

        Es conveniente usar un archivo de tipo PNG. Además se deben respetar las medidas indicadas
        previamente.


        *******************************************************************************************/
        // Inicializar la semilla aleatoria
        srand(time(0));

        // Generar un número aleatorio entre 1 y 3
        numeroAleatorio = rand() % 3 + 1;
        
        // Realizar una acción diferente según el número aleatorio
        switch (numeroAleatorio) {
        case 1:
            // Acción para el número 1
            tablero ()->ponArchivoFondo(carpeta_activos_juego + "arena1.jpg");
            break;
        case 2:
            // Acción para el número 2
            tablero ()->ponArchivoFondo(carpeta_activos_juego + "arena2.jpg");
            break;
        case 3:
            // Acción para el número 2
            tablero ()->ponArchivoFondo(carpeta_activos_juego + "arena3.jpg");
            break;
        default:
            // Acción por defecto (en caso de un número fuera del rango.
            tablero ()->ponArchivoFondo (carpeta_activos_juego + "arena1.jpg");
            break;
        }
        //tablero ()->ponArchivoBaldosas (carpeta_activos_juego + "estampas_fondo.png");
        /*******************************************************************************************
        /******************************************************************************************/
        //
        //
        /*******************************************************************************************
        /*******************************************************************************************
        
        CONFIGURACIÓN DEL JUEGO
        Cuarta sección - Los personajes - Primera parte

        En esta sección se configuran los equipos del juego. 

        Cada equipo tiene un nombre y un escudo que aparece sobre la bandera en la parte superior de
        los paneles laterales del juego. En escudo se superpone a la bandera, sin taparla. El tamaño
        de las dos imágenes es de 40x40 px.
        
        Programación
        ------------

        Para cambiar los nombres de los equipos, se deben cambiar las dos líneas de código 
        siguientes. 
        
        Para cambiar las imágenes de los escudos es suficiente con situar las imágenes nuevas en la 
        carpeta de activos del juego y cambiar los nombres en las líneas de código siguiente. 
        
        Es conveniente usar archivos de tipo PNG que permiten la transparencia. Además se deben 
        respetar las medidas indicadas previamente.

        *******************************************************************************************/
        tablero ()->equipa (LadoTablero::Izquierda, L"Devils", carpeta_activos_juego + "escudo_devils.png");
        tablero ()->equipa (LadoTablero::Derecha,   L"Tusk",   carpeta_activos_juego + "escudo_tusk.png");
        /*******************************************************************************************
        /******************************************************************************************/
        //
        //
        /*******************************************************************************************
        /*******************************************************************************************
        
        CONFIGURACIÓN DEL JUEGO
        Tercera sección - El tablero - Tercera parte

        En esta sección se preparan los muros que aparecen en el tablero de juego. En la primera
        parte de la sección se preparan los muros que aparecen en el tablero, en esta tercera parte 
        se establecen los muros que usará el juego.

        Programación
        ------------

        Para cambiar los muros que aparecen en el tablero de juego es suficiente con cambiar el 
        nombre de la variable que contiene el array que los especifica en la línea de código que 
        aparece a continuación.
        
        *******************************************************************************************/
        tablero ()->situaMuros (grafico_muros_area_central);
        /*******************************************************************************************
        /******************************************************************************************/
    }


    void JuegoMesa::preparaPersonajes () {
        //
        /*******************************************************************************************
        /*******************************************************************************************
        
        CONFIGURACIÓN DEL JUEGO
        Cuarta sección - Los personajes - Tercera parte

        En esta sección se configuran los personajes del juego. En la segunda parte se establecen
        las variables destinadas a almacenar las instancias que definen los personajes. En esta 
        parte se crean las instancias y se configuran.

        Las variable usan un nombre similar al nombre del personaje correspondiente. Las variables 
        son de tipo puntero, la creación de las instancias utiliza la palabra reservada 'new'. 

        Atributos
        ---------

        Se asignan los siguientes atributos a los personajes:

             a) Nombre: Asignado en la creación de la instancia en una cadena de caracteres. Deben 
                ser evitados los nombres compuestos y los nombres largos.

             b) Equipo: Se establece indicando si el personaje aparece en el panel de la izquierda o 
                en el panel de la derecha en el tablero de juego.

             c) Orden en equipo: Debe ser un índice a partir de 0 y consecutivo.

             d) Iniciativa: Establece que personaje actúa en primer lugar en algunas de las 
                circunstancias del juego. Debe ser positivo.

        Imágenes
        --------

        Cada personaje tiene un retrato con el que aparece en la pantalla del juego. El tamaño de la
        imagen usada es de 75x75 px. La imagen puede tener zonas transparentes, en cuyo caso aparece 
        sobre un fondo gris oscuro. El retrato se especifica en la creación de la instancia.

        De la misma forma, cada personaje tiene una imagen para la ficha con la que aparece en el 
        tablero. El tamaño de la imagen usada es de 31x31 px.

        Programación
        ------------

        Se deben modificar las lineas de código que aparecen a continuación.

        Se debe sustituir las listas de creaciones de instancias y las listas de llamadas a los 
        métodos por listas similares, obtenidas a partir de la lista de personajes diseñados para el 
        juego.

        Se deben cambiar los valores de los parámetros según los valores establecidos para cada 
        personaje en el diseño del juego.

        Para cambiar las imágenes de los retratos de los personajes es suficiente con situar las 
        imágenes nuevas en la carpeta de retratos del juego y cambiar los nombres de los archivos en 
        las llamadas al método correspondientes. Lo mismo ocurre con las imágenes de las fichas.
        
        Es conveniente usar archivos de tipo PNG que permiten la transparencia. Además se deben 
        respetar las medidas indicadas previamente.

        *******************************************************************************************/
        // TODO
        Ninja1 = new ActorPersonaje{ this, LadoTablero::Izquierda, 0, L"Masaaki" };
        Caballero1 = new ActorPersonaje{ this, LadoTablero::Izquierda, 1, L"Arturo" };
        Pirata1 = new ActorPersonaje{ this, LadoTablero::Izquierda, 2, L"Barbanegra" };
        Samurai1 = new ActorPersonaje{ this, LadoTablero::Izquierda, 3, L"Yoshitsune" };
        Monje1 = new ActorPersonaje{ this, LadoTablero::Izquierda, 4, L"Ashin" };

        Ninja2 = new ActorPersonaje{ this, LadoTablero::Derecha, 0, L"Kawakami" };
        Caballero2 = new ActorPersonaje{ this, LadoTablero::Derecha, 1, L"Siegfried" };
        Pirata2 = new ActorPersonaje{ this, LadoTablero::Derecha, 2, L"Kidd" };
        Samurai2 = new ActorPersonaje{ this, LadoTablero::Derecha, 3, L"Masamune" };
        Monje2 = new ActorPersonaje{ this, LadoTablero::Derecha, 4, L"Je" };
        //
        Ninja1->ponArchivoRetrato (carpeta_retratos_juego + "ninja1.png");
        Caballero1->ponArchivoRetrato (carpeta_retratos_juego + "caballero1.png");
        Samurai1->ponArchivoRetrato (carpeta_retratos_juego + "samurai1.png");
        Pirata1->ponArchivoRetrato (carpeta_retratos_juego + "pirata1.png");
        Monje1->ponArchivoRetrato (carpeta_retratos_juego + "shaolin1.png");

        Ninja2->ponArchivoRetrato(carpeta_retratos_juego + "ninja1.png");
        Caballero2->ponArchivoRetrato(carpeta_retratos_juego + "caballero1.png");
        Samurai2->ponArchivoRetrato(carpeta_retratos_juego + "samurai1.png");
        Pirata2->ponArchivoRetrato(carpeta_retratos_juego + "pirata1.png");
        Monje2->ponArchivoRetrato(carpeta_retratos_juego + "shaolin1.png");
        //
        Ninja1->ponArchivoFicha (carpeta_retratos_juego + "ficha_roja.png");
        Caballero1->ponArchivoFicha (carpeta_retratos_juego + "ficha_roja.png");
        Samurai1->ponArchivoFicha (carpeta_retratos_juego + "ficha_roja.png");
        Pirata1->ponArchivoFicha (carpeta_retratos_juego + "ficha_roja.png");
        Monje1->ponArchivoFicha (carpeta_retratos_juego + "ficha_roja.png");

        Ninja2->ponArchivoFicha (carpeta_retratos_juego + "ficha_azul.png");
        Caballero2->ponArchivoFicha (carpeta_retratos_juego + "ficha_azul.png");
        Samurai2->ponArchivoFicha (carpeta_retratos_juego + "ficha_azul.png");
        Pirata2->ponArchivoFicha(carpeta_retratos_juego + "ficha_azul.png");
        Monje2->ponArchivoFicha(carpeta_retratos_juego + "ficha_azul.png");
        //
        Ninja1->ponIniciativa (20);
        Caballero1->ponIniciativa (16);
        Samurai1->ponIniciativa (18);
        Pirata1->ponIniciativa (17);
        Monje1->ponIniciativa(19);

        Ninja2->ponIniciativa (20);
        Caballero2->ponIniciativa (16);
        Samurai2->ponIniciativa (18);
        Pirata2->ponIniciativa (17);
        Monje2->ponIniciativa(19);
        //

        agregaPersonaje (Ninja1);
        agregaPersonaje (Caballero1);
        agregaPersonaje (Pirata1);
        agregaPersonaje (Samurai1);
        agregaPersonaje (Monje1);
        agregaPersonaje (Ninja2);
        agregaPersonaje (Caballero2);
        agregaPersonaje (Pirata2);
        agregaPersonaje (Samurai2);
        agregaPersonaje (Monje2);
        //
        /*******************************************************************************************
        /******************************************************************************************/
    }


    void JuegoMesa::preparaHabilidades () {
        //
        /*******************************************************************************************
        /*******************************************************************************************
        
        CONFIGURACIÓN DEL JUEGO
        Quinta sección - Las habilidades - Segunda parte

        En esta sección se configuran las habilidades compartidas por los personajes del juego. En 
        la primera parte se establecen las variables destinadas a almacenar las instancias que 
        definen las habilidades. En esta segunda parte se crean las instancias y se configuran.

        Las variable usan un nombre similar al nombre de la habilidad correspondiente. Las variables 
        son de tipo puntero, la creación de las instancias utiliza la palabra reservada 'new'. 

        Atributos
        ---------

        Se asignan los siguientes atributos a las habilidades:

             a) Nombre: Asignado en la creación de la instancia en una cadena de caracteres. 

             b) Enfoque: Indica a que esta enfocada la habilidad. Puede ser a un personaje, a un 
                área formada por varias celdas del tablero o hacia el mismo personaje que usa la 
                habilidad. 

             c) Acceso: Indica si el efecto de la habilidad puede atravesar muros. Puede ser directo,
                indirecto o ninguno.

             d) Antagonista: Indica a que grupo de personajes se dirige la habilidad. Puede ser a un
                oponente, a un aliado o a sí mismo.

             e) Descripción: Es un texto largo que describe la habilidad.

        Imágenes
        --------

        Cada habilidad tiene una imagen con el que aparece en la pantalla del juego. El tamaño de la
        imagen usada es de 50x50 px. La imagen puede tener zonas transparentes, en cuyo caso aparece 
        sobre una imagen de fondo, que puede ser distinta para cada habilidad o la misma para todas. 
        Las dos imágenes de cada habilidad se especifican la misma llamada a un método.

        Sonidos
        -------

        Cada habilidad tiene un sonido que suena cuando el personaje la usa. El sonido se carga de 
        un archivo de tipo 'wav', 'ogg' o 'flac', pero no 'mp3'. El tiempo de emisión del sonido
        debe ser corto (escasos segundos).

        Programación
        ------------

        Se deben modificar las lineas de código que aparecen a continuación.

        Se debe sustituir las listas de creaciones de instancias y las listas de llamadas a los 
        métodos por listas similares, obtenidas a partir de la lista de habilidades diseñadas para 
        el juego.

        Se deben cambiar los valores de los parámetros según los valores establecidos para cada 
        habilidad en el diseño del juego.

        Para cambiar las imágenes de las habilidad es suficiente con situar las imágenes nuevas en 
        la carpeta de habilidades del juego y cambiar los nombres en las llamadas al método 
        correspondiente. 
        
        Para cambiar los sonidos de las habilidad es suficiente con situar los sonidos nuevos en 
        la carpeta de sonidos del juego y cambiar los nombres en las llamadas al método 
        correspondiente. 
        
        Es conveniente usar archivos de tipo PNG que permiten la transparencia. Además se deben 
        respetar las medidas indicadas previamente.

        *******************************************************************************************/
     
        ninjaAtaqueCorto = new Habilidad {
                L"Dos espadas cortas", 
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente};
        ninjaAtaqueLargo = new Habilidad{
                L"Kusarigama",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        ninjaAtaqueDistancia = new Habilidad{
                L"Shuriken",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        ninjaAtaqueEspecial1 = new Habilidad{
                L"Especial 1: Bomba de humo",
                EnfoqueHabilidad::area, AccesoHabilidad::directo,   Antagonista::aliado };
        ninjaAtaqueEspecial2 = new Habilidad{
                L"Especial 2: Veneno",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };

        caballeroAtaqueCorto = new Habilidad{
                L"Espada larga",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        caballeroAtaqueLargo = new Habilidad{
                L"Lanza de caballero",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        caballeroAtaqueDistancia = new Habilidad{
                L"Ballesta",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        caballeroAtaqueEspecial1 = new Habilidad{
                L"Especial 1: Escudo",
                EnfoqueHabilidad::si_mismo, AccesoHabilidad::ninguno,   Antagonista::si_mismo };
        caballeroAtaqueEspecial2 = new Habilidad{
                L"Especial 2: Caballo",
                EnfoqueHabilidad::si_mismo, AccesoHabilidad::ninguno,   Antagonista::si_mismo };


        pirataAtaqueCorto = new Habilidad{
                L"Sable",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        pirataAtaqueLargo = new Habilidad{
                L"Trabuco",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        pirataAtaqueDistancia = new Habilidad{
                L"Mosquete",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        pirataAtaqueEspecial1 = new Habilidad{
                L"Especial 1: Pistola",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        pirataAtaqueEspecial2 = new Habilidad{
                L"Especial 2: Cañón",
                EnfoqueHabilidad::area, AccesoHabilidad::indirecto,   Antagonista::oponente };


        samuraiAtaqueCorto = new Habilidad{
                L"Katana",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        samuraiAtaqueLargo = new Habilidad{
                L"Naginata",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        samuraiAtaqueDistancia = new Habilidad{
                L"Yumi",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        samuraiAtaqueEspecial1 = new Habilidad{
                L"Especial 1: Iaidō",
                EnfoqueHabilidad::si_mismo, AccesoHabilidad::ninguno,   Antagonista::si_mismo };
        samuraiAtaqueEspecial2 = new Habilidad{
                L"Especial 2: Kiai",
                EnfoqueHabilidad::area, AccesoHabilidad::indirecto,   Antagonista::oponente };


        monjeAtaqueCorto = new Habilidad{
                L"Espada",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        monjeAtaqueLargo = new Habilidad{
                L"Lanza",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        monjeAtaqueDistancia = new Habilidad{
                L"Cadenas",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        monjeAtaqueEspecial1 = new Habilidad{
                L"Especial 1: Puño de hierro",
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente };
        monjeAtaqueEspecial2 = new Habilidad{
                L"Especial 2: Curación",
                EnfoqueHabilidad::area, AccesoHabilidad::directo,   Antagonista::aliado };

        /*
        ataqueArco           = new Habilidad {
                L"Ataque a distancia normal",     
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente};
        ataqueEspadaPoderoso = new Habilidad {
                L"Ataque poderoso",               
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::oponente};
        defensaFerrea        = new Habilidad {
                L"Defensa férrea",                
                EnfoqueHabilidad::si_mismo,  AccesoHabilidad::ninguno,   Antagonista::si_mismo};
        curacionSimple       = new Habilidad {
                L"Curación",                      
                EnfoqueHabilidad::personaje, AccesoHabilidad::directo,   Antagonista::aliado};
        curacionGrupo        = new Habilidad {
                L"Curación en grupo",             
                EnfoqueHabilidad::area,      AccesoHabilidad::directo,   Antagonista::aliado};
        proyectilMagico      = new Habilidad {
                L"Proyectil mágico",              
                EnfoqueHabilidad::personaje, AccesoHabilidad::indirecto, Antagonista::oponente};
        bolaFuego            = new Habilidad {
                L"Bola de fuego",                 
                EnfoqueHabilidad::area,      AccesoHabilidad::directo,   Antagonista::oponente};
        //
        ataqueEspadaNormal  ->ponDescripcion (
                L"El personaje usa su espada para atacar a un enemigo que se encuentra en una casilla próxima.");
        ataqueArco          ->ponDescripcion (
                L"El personaje usa su arco para atacar a un enemigo que se encuentra en una casilla próxima.");
        ataqueEspadaPoderoso->ponDescripcion (
                L"El personaje blande su espada con fuerza para atacar a un enemigo que se encuentra en una casilla próxima.");
        defensaFerrea       ->ponDescripcion (
                L"El personaje aumenta su defensa hasta su próximo ronda.");
        curacionSimple      ->ponDescripcion (
                L"El personaje canaliza poder divino para sanar a un aliado.");
        curacionGrupo      ->ponDescripcion (
                L"El personaje canaliza poder divino para sanar a todos los personajes en un área.");
        proyectilMagico     ->ponDescripcion (
                L"El personaje acumula energía mágica y la lanza contra un enemigo.");
        bolaFuego           ->ponDescripcion (
                L"El personaje acumula energía mágica y la lanza contra un grupo de enemigos.");
        */

        ninjaAtaqueCorto->ponDescripcion(
            L"El personaje usa sus espadas cortas para atacar a un enemigo que se encuentra en una casilla próxima.");
        ninjaAtaqueLargo->ponDescripcion(
            L"El personaje usa su kusarigama para atacar a un enemigo que se encuentra en una casilla próxima.");
        ninjaAtaqueDistancia->ponDescripcion(
            L"El personaje usa sus shuriken para atacar a un enemigo que se encuentra en una casilla lejana.");
        ninjaAtaqueEspecial1->ponDescripcion(
            L"El personaje se esconde de sus enemigos usando una bomba de humo.");
        ninjaAtaqueEspecial2->ponDescripcion(
            L"El personaje envenena a un enemigo próximo.");
        
        caballeroAtaqueCorto->ponDescripcion(
            L"El personaje usa su espada para atacar a un enemigo que se encuentra en una casilla próxima.");
        caballeroAtaqueLargo->ponDescripcion(
            L"El personaje usa su lanza para atacar a un enemigo que se encuentra en una casilla próxima.");
        caballeroAtaqueDistancia->ponDescripcion(
            L"El personaje usa su ballesta para atacar a un enemigo que se encuentra en una casilla lejana.");
        caballeroAtaqueEspecial1->ponDescripcion(
            L"El personaje se defiende usando su escudo.");
        caballeroAtaqueEspecial2->ponDescripcion(
            L"El personaje se monta en su caballo.");

        pirataAtaqueCorto->ponDescripcion(
            L"El personaje usa su sable para atacar a un enemigo que se encuentra en una casilla próxima.");
        pirataAtaqueLargo->ponDescripcion(
            L"El personaje usa su trabuco para atacar a un enemigo que se encuentra en una casilla próxima.");
        pirataAtaqueDistancia->ponDescripcion(
            L"El personaje usa su mosquete para atacar a un enemigo que se encuentra en una casilla lejana.");
        pirataAtaqueEspecial1->ponDescripcion(
            L"El personaje usa su pistola para atacar a un enemigo que se encuentra en una casilla lejana.");
        pirataAtaqueEspecial2->ponDescripcion(
            L"El personaje usa su cañón para atacar a los enemigos que se encuentran en una casilla lejana.");

        samuraiAtaqueCorto->ponDescripcion(
            L"El personaje usa su katana para atacar a un enemigo que se encuentra en una casilla próxima.");
        samuraiAtaqueLargo->ponDescripcion(
            L"El personaje usa su naginata para atacar a un enemigo que se encuentra en una casilla próxima.");
        samuraiAtaqueDistancia->ponDescripcion(
            L"El personaje usa su yumi para atacar a un enemigo que se encuentra en una casilla lejana.");
        samuraiAtaqueEspecial1->ponDescripcion(
            L"El personaje se concentra para defenderse de los ataques enemigos.");
        samuraiAtaqueEspecial2->ponDescripcion(
            L"El personaje grita usando su kiai para paralizar a los enemigos.");

        monjeAtaqueCorto->ponDescripcion(
            L"El personaje usa su espada para atacar a un enemigo que se encuentra en una casilla próxima.");
        monjeAtaqueLargo->ponDescripcion(
            L"El personaje usa su lanza para atacar a un enemigo que se encuentra en una casilla próxima.");
        monjeAtaqueDistancia->ponDescripcion(
            L"El personaje usa su cadenas para atacar a un enemigo que se encuentra en una casilla lejana.");
        monjeAtaqueEspecial1->ponDescripcion(
            L"El personaje usa sus puños de hierro para atacar a un enemigo que se encuentra en una casilla próxima.");
        monjeAtaqueEspecial2->ponDescripcion(
            L"El personaje se concentra para curar a los aliados próximos y a si mismo.");

        ninjaAtaqueCorto->ponArchivosImagenes(
            carpeta_habilids_juego + "n_atq_corto.png", carpeta_habilids_juego + "fondo_5.png");
        ninjaAtaqueLargo->ponArchivosImagenes(
            carpeta_habilids_juego + "n_atq_largo.png", carpeta_habilids_juego + "fondo_5.png");
        ninjaAtaqueDistancia->ponArchivosImagenes(
            carpeta_habilids_juego + "n_atq_dist.png", carpeta_habilids_juego + "fondo_5.png");
        ninjaAtaqueEspecial1->ponArchivosImagenes(
            carpeta_habilids_juego + "n_hab1.png", carpeta_habilids_juego + "fondo_5.png");
        ninjaAtaqueEspecial2->ponArchivosImagenes(
            carpeta_habilids_juego + "n_hab2.png", carpeta_habilids_juego + "fondo_5.png");

        samuraiAtaqueCorto->ponArchivosImagenes(
            carpeta_habilids_juego + "s_atq_corto.png", carpeta_habilids_juego + "fondo_5.png");
        samuraiAtaqueLargo->ponArchivosImagenes(
            carpeta_habilids_juego + "s_atq_largo.png", carpeta_habilids_juego + "fondo_5.png");
        samuraiAtaqueDistancia->ponArchivosImagenes(
            carpeta_habilids_juego + "s_atq_dist.png", carpeta_habilids_juego + "fondo_5.png");
        samuraiAtaqueEspecial1->ponArchivosImagenes(
            carpeta_habilids_juego + "s_hab1.png", carpeta_habilids_juego + "fondo_5.png");
        samuraiAtaqueEspecial2->ponArchivosImagenes(
            carpeta_habilids_juego + "s_hab2.png", carpeta_habilids_juego + "fondo_5.png");

        pirataAtaqueCorto->ponArchivosImagenes(
            carpeta_habilids_juego + "p_atq_corto.png", carpeta_habilids_juego + "fondo_5.png");
        pirataAtaqueLargo->ponArchivosImagenes(
            carpeta_habilids_juego + "p_atq_largo.png", carpeta_habilids_juego + "fondo_5.png");
        pirataAtaqueDistancia->ponArchivosImagenes(
            carpeta_habilids_juego + "p_atq_dist.png", carpeta_habilids_juego + "fondo_5.png");
        pirataAtaqueEspecial1->ponArchivosImagenes(
            carpeta_habilids_juego + "p_hab1.png", carpeta_habilids_juego + "fondo_5.png");
        pirataAtaqueEspecial2->ponArchivosImagenes(
            carpeta_habilids_juego + "p_hab2.png", carpeta_habilids_juego + "fondo_5.png");

        monjeAtaqueCorto->ponArchivosImagenes(
            carpeta_habilids_juego + "sh_atq_corto.png", carpeta_habilids_juego + "fondo_5.png");
        monjeAtaqueLargo->ponArchivosImagenes(
            carpeta_habilids_juego + "sh_atq_largo.png", carpeta_habilids_juego + "fondo_5.png");
        monjeAtaqueDistancia->ponArchivosImagenes(
            carpeta_habilids_juego + "sh_atq_dist.png", carpeta_habilids_juego + "fondo_5.png");
        monjeAtaqueEspecial1->ponArchivosImagenes(
            carpeta_habilids_juego + "sh_hab1.png", carpeta_habilids_juego + "fondo_5.png");
        monjeAtaqueEspecial2->ponArchivosImagenes(
            carpeta_habilids_juego + "sh_hab2.png", carpeta_habilids_juego + "fondo_5.png");

        caballeroAtaqueCorto->ponArchivosImagenes(
            carpeta_habilids_juego + "c_atq_corto.png", carpeta_habilids_juego + "fondo_5.png");
        caballeroAtaqueLargo->ponArchivosImagenes(
            carpeta_habilids_juego + "c_atq_largo.png", carpeta_habilids_juego + "fondo_5.png");
        caballeroAtaqueDistancia->ponArchivosImagenes(
            carpeta_habilids_juego + "c_atq_dist.png", carpeta_habilids_juego + "fondo_5.png");
        caballeroAtaqueEspecial1->ponArchivosImagenes(
            carpeta_habilids_juego + "c_hab1.png", carpeta_habilids_juego + "fondo_5.png");
        caballeroAtaqueEspecial2->ponArchivosImagenes(
            carpeta_habilids_juego + "c_hab2.png", carpeta_habilids_juego + "fondo_5.png");

        /*
        ataqueEspadaNormal  ->ponArchivosImagenes (
                carpeta_habilids_juego + "espada.png",          carpeta_habilids_juego + "fondo_5.png");
        ataqueArco          ->ponArchivosImagenes (
                carpeta_habilids_juego + "arco_flecha.png",     carpeta_habilids_juego + "fondo_5.png");
        ataqueEspadaPoderoso->ponArchivosImagenes (
                carpeta_habilids_juego + "espada_poderosa.png", carpeta_habilids_juego + "fondo_5.png");
        defensaFerrea       ->ponArchivosImagenes (
                carpeta_habilids_juego + "escudo.png",          carpeta_habilids_juego + "fondo_5.png");
        curacionSimple      ->ponArchivosImagenes (
                carpeta_habilids_juego + "vela.png",            carpeta_habilids_juego + "fondo_5.png");
        curacionGrupo       ->ponArchivosImagenes (
                carpeta_habilids_juego + "vela_triple.png",     carpeta_habilids_juego + "fondo_5.png");
        proyectilMagico     ->ponArchivosImagenes (
                carpeta_habilids_juego + "cristales.png",       carpeta_habilids_juego + "fondo_5.png");
        bolaFuego           ->ponArchivosImagenes (
                carpeta_habilids_juego + "bola_fuego.png",      carpeta_habilids_juego + "fondo_5.png");
        
        ataqueEspadaNormal  ->ponArchivoSonido (carpeta_sonidos_juego + "Magic Missiles.wav");
        ataqueArco          ->ponArchivoSonido (carpeta_sonidos_juego + "Magic Missiles.wav");
        ataqueEspadaPoderoso->ponArchivoSonido (carpeta_sonidos_juego + "Magic Missiles.wav");
        defensaFerrea       ->ponArchivoSonido (carpeta_sonidos_juego + "Magic Missiles.wav");
        curacionSimple      ->ponArchivoSonido (carpeta_sonidos_juego + "Magic Missiles.wav");
        curacionGrupo       ->ponArchivoSonido (carpeta_sonidos_juego + "Magic Missiles.wav");
        proyectilMagico     ->ponArchivoSonido (carpeta_sonidos_juego + "Magic Missiles.wav");
        bolaFuego           ->ponArchivoSonido (carpeta_sonidos_juego + "Magic Missiles.wav");
        */

        ninjaAtaqueCorto->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        ninjaAtaqueLargo->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        ninjaAtaqueDistancia->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        ninjaAtaqueEspecial1->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        ninjaAtaqueEspecial2->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");

        caballeroAtaqueCorto->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        caballeroAtaqueLargo->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        caballeroAtaqueDistancia->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        caballeroAtaqueEspecial1->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        caballeroAtaqueEspecial2->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");

        pirataAtaqueCorto->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        pirataAtaqueLargo->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        pirataAtaqueDistancia->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        pirataAtaqueEspecial1->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        pirataAtaqueEspecial2->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");

        samuraiAtaqueCorto->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        samuraiAtaqueLargo->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        samuraiAtaqueDistancia->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        samuraiAtaqueEspecial1->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        samuraiAtaqueEspecial2->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");

        monjeAtaqueCorto->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        monjeAtaqueLargo->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        monjeAtaqueDistancia->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        monjeAtaqueEspecial1->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");
        monjeAtaqueEspecial2->ponArchivoSonido(carpeta_sonidos_juego + "Magic Missiles.wav");

        agregaHabilidad(ninjaAtaqueCorto);
        agregaHabilidad(ninjaAtaqueLargo);
        agregaHabilidad(ninjaAtaqueDistancia);
        agregaHabilidad(ninjaAtaqueEspecial1);
        agregaHabilidad(ninjaAtaqueEspecial2);

        agregaHabilidad(caballeroAtaqueCorto);
        agregaHabilidad(caballeroAtaqueLargo);
        agregaHabilidad(caballeroAtaqueDistancia);
        agregaHabilidad(caballeroAtaqueEspecial1);
        agregaHabilidad(caballeroAtaqueEspecial2);

        agregaHabilidad(pirataAtaqueCorto);
        agregaHabilidad(pirataAtaqueLargo);
        agregaHabilidad(pirataAtaqueDistancia);
        agregaHabilidad(pirataAtaqueEspecial1);
        agregaHabilidad(pirataAtaqueEspecial2);

        agregaHabilidad(samuraiAtaqueCorto);
        agregaHabilidad(samuraiAtaqueLargo);
        agregaHabilidad(samuraiAtaqueDistancia);
        agregaHabilidad(samuraiAtaqueEspecial1);
        agregaHabilidad(samuraiAtaqueEspecial2);

        agregaHabilidad(monjeAtaqueCorto);
        agregaHabilidad(monjeAtaqueLargo);
        agregaHabilidad(monjeAtaqueDistancia);
        agregaHabilidad(monjeAtaqueEspecial1);
        agregaHabilidad(monjeAtaqueEspecial2);
        
        /*******************************************************************************************
        /******************************************************************************************/
    }


    void JuegoMesa::preparaTiposEstadisticas () {
        /*******************************************************************************************
        /*******************************************************************************************
        
        CONFIGURACIÓN DEL JUEGO
        Sexta sección - Los tipos de ataque, defensa y daño (estadísticas) - Segunda parte

        En esta sección se configuran los tipos de ataque, defensa y daño del juego. Estos tipos  
        son usados para establecer las estadisticas del juego en los personajes y en las habilidades. 
        En la primera parte se establecen las variables destinadas a almacenar las instancias que 
        definen los tipos de ataque, defensa y daño. En esta parte se crean las instancias y se 
        configuran.

        Las variable usan un nombre similar al nombre de los tipos correspondiente. Las variables 
        son de tipo puntero, la creación de las instancias utiliza la palabra reservada 'new'. 

        Atributos
        ---------

        Se asignan los siguientes atributos a los tipos:

             a) Descripción: Asignado en la creación de la instancia en una cadena de caracteres. 
                Deben ser cortos y deben comenzar con la palabra 'Ataque', 'Defensa' o 'Daño'.

        Programación
        ------------

        Se deben modificar las líneas de código que aparecen a continuación.

        Se debe sustituir las listas de creaciones de instancias por listas similares, obtenidas a 
        partir de la lista de tipos diseñados para el juego. El nombre de la clase (por ejemplo 
        'TipoAtaque' para los ataques) establece si es un tipo de ataque, de defensa o de daño.

        Se deben cambiar los valores de los parámetros según los valores establecidos para cada 
        tipo en el diseño del juego.

        *******************************************************************************************/
        //
        ataqueCuerpoACuerpo  = new TipoAtaque  {L"Ataque cuerpo a cuerpo"};
        ataqueADistancia     = new TipoAtaque  {L"Ataque a distancia"};
        //ataqueMagico         = new TipoAtaque  {L"Ataque mágico"};
        defensaCuerpoACuerpo = new TipoDefensa {L"Defensa cuerpo a cuerpo"};
        defensaADistancia    = new TipoDefensa {L"Defensa a distancia"};
        //defensaMagica        = new TipoDefensa {L"Defensa mágica"};
        danoFisico           = new TipoDano    {L"Daño físico"};
        //danoMagico           = new TipoDano    {L"Daño mágico"};
        //
        agregaAtaque  (ataqueCuerpoACuerpo );
        agregaAtaque  (ataqueADistancia);
        //agregaAtaque  (ataqueMagico);
        agregaDefensa (defensaCuerpoACuerpo);
        agregaDefensa (defensaADistancia );
        //agregaDefensa (defensaMagica );
        agregaDano    (danoFisico);
        //agregaDano    (danoMagico);
        //
        /*******************************************************************************************
        /******************************************************************************************/
    }


    void JuegoMesa::agregaHabilidadesPersonajes () {
        /*******************************************************************************************
        /*******************************************************************************************
        
        CONFIGURACIÓN DEL JUEGO
        Octava sección - Las habilidades de los personajes

        En esta sección se establecen que habilidades que tiene cada personajes del juego.

        Cada personaje debe tener entre 1 y 10 habilidades.

        Programación
        ------------

        Se deben modificar las líneas de código que aparecen a continuación.

        Se deben sustituir las listas de habilidades de cada personaje por listas similares, 
        obtenidas a partir del diseño del juego.

        *******************************************************************************************/
        //

        Ninja1->agregaHabilidad(ninjaAtaqueCorto);
        Ninja1->agregaHabilidad(ninjaAtaqueLargo);
        Ninja1->agregaHabilidad(ninjaAtaqueDistancia);
        Ninja1->agregaHabilidad(ninjaAtaqueEspecial1);
        Ninja1->agregaHabilidad(ninjaAtaqueEspecial2);
        Ninja2->agregaHabilidad(ninjaAtaqueCorto);
        Ninja2->agregaHabilidad(ninjaAtaqueLargo);
        Ninja2->agregaHabilidad(ninjaAtaqueDistancia);
        Ninja2->agregaHabilidad(ninjaAtaqueEspecial1);
        Ninja2->agregaHabilidad(ninjaAtaqueEspecial2);

        Pirata1->agregaHabilidad(pirataAtaqueCorto);
        Pirata1->agregaHabilidad(pirataAtaqueLargo);
        Pirata1->agregaHabilidad(pirataAtaqueDistancia);
        Pirata1->agregaHabilidad(pirataAtaqueEspecial1);
        Pirata1->agregaHabilidad(pirataAtaqueEspecial2);
        Pirata2->agregaHabilidad(pirataAtaqueCorto);
        Pirata2->agregaHabilidad(pirataAtaqueLargo);
        Pirata2->agregaHabilidad(pirataAtaqueDistancia);
        Pirata2->agregaHabilidad(pirataAtaqueEspecial1);
        Pirata2->agregaHabilidad(pirataAtaqueEspecial2);

        Samurai1->agregaHabilidad(samuraiAtaqueCorto);
        Samurai1->agregaHabilidad(samuraiAtaqueLargo);
        Samurai1->agregaHabilidad(samuraiAtaqueDistancia);
        Samurai1->agregaHabilidad(samuraiAtaqueEspecial1);
        Samurai1->agregaHabilidad(samuraiAtaqueEspecial2);
        Samurai2->agregaHabilidad(samuraiAtaqueCorto);
        Samurai2->agregaHabilidad(samuraiAtaqueLargo);
        Samurai2->agregaHabilidad(samuraiAtaqueDistancia);
        Samurai2->agregaHabilidad(samuraiAtaqueEspecial1);
        Samurai2->agregaHabilidad(samuraiAtaqueEspecial2);

        Monje1->agregaHabilidad(monjeAtaqueCorto);
        Monje1->agregaHabilidad(monjeAtaqueLargo);
        Monje1->agregaHabilidad(monjeAtaqueDistancia);
        Monje1->agregaHabilidad(monjeAtaqueEspecial1);
        Monje1->agregaHabilidad(monjeAtaqueEspecial2);
        Monje2->agregaHabilidad(monjeAtaqueCorto);
        Monje2->agregaHabilidad(monjeAtaqueLargo);
        Monje2->agregaHabilidad(monjeAtaqueDistancia);
        Monje2->agregaHabilidad(monjeAtaqueEspecial1);
        Monje2->agregaHabilidad(monjeAtaqueEspecial2);

        Caballero1->agregaHabilidad(caballeroAtaqueCorto);
        Caballero1->agregaHabilidad(caballeroAtaqueLargo);
        Caballero1->agregaHabilidad(caballeroAtaqueDistancia);
        Caballero1->agregaHabilidad(caballeroAtaqueEspecial1);
        Caballero1->agregaHabilidad(caballeroAtaqueEspecial2);
        Caballero2->agregaHabilidad(caballeroAtaqueCorto);
        Caballero2->agregaHabilidad(caballeroAtaqueLargo);
        Caballero2->agregaHabilidad(caballeroAtaqueDistancia);
        Caballero2->agregaHabilidad(caballeroAtaqueEspecial1);
        Caballero2->agregaHabilidad(caballeroAtaqueEspecial2);
        //
        /*******************************************************************************************
        /******************************************************************************************/
    }


    void JuegoMesa::agregaEstadisticasHabilidades () {
        /*******************************************************************************************
        /*******************************************************************************************
        
        CONFIGURACIÓN DEL JUEGO
        Novena sección - Las estadísticas de las habilidades

        En esta sección se establecen diversos atributos y valores para las habilidades del juego,
        incluyendo las estadisticas del juego.

        Estadísticas
        ------------

        Las habilidades se clasifican en habilidades de ataque, de curación y auto-aplicada. Esta 
        clasificación se establece mediante el atributo 'Antagonista' de la habilidad. 

            - Si 'Antagonista' es 'oponente' la habilidad es de ataque. El ataque puede afectar a un 
              personaje o a los personajes de un área del tablero.
            - Si 'Antagonista' es 'aliado' la habilidad es de curación. La curación puede afectar a 
              un personaje o a los personajes de un área del tablero.
            - Si 'Antagonista' es 'si_mismo' la habilidad es auto-aplicada.

        Los valores que se establecen para las habilidades son:

             a) Coste: Es el número de puntos de acción que se gastan al usar la habilidad.

             b) Alcance: En las habilidades de ataque y curación debe estar establecido el alcance 
                de la habilidad. La habilidad solo se puede usar si el alcance lo permite. Es un 
                valor que indica un número de casillas del tablero y debe ser positivo.

             c) Radio de ara de alcance: En las habilidades de ataque y curación que afectan a un 
                área del tablero, debe estar establecido el radio del área. Es un valor que indica 
                un número de casillas del tablero y debe ser positivo. 

             d) Tipo de ataque: Cuando una habilidad es de ataque, debe tener establecido un tipo de 
                ataque. El tipo de ataque se usa en el personaje atacante.

             e) Tipo de defensa: Cuando una habilidad es de ataque, debe tener establecido un tipo 
                de defensa. El tipo de defensa se usa en el personaje atacado.

             f) Tipo de daño: Cuando una habilidad es de ataque, debe tener establecido un tipo de 
                daño. El tipo de daño se usa en el personaje atacado.

             g) Valor del daño: Cuando una habilidad es de ataque, debe tener establecido un valor 
                para el daño en puntos de vitalidad. El valor del daño se usa con el personaje 
                atacado.

             h) Curación: Cuando una habilidad es de curación, debe tener establecido un valor de 
                curación en puntos.

             i) Efecto en ataque: Cuando una habilidad es auto-aplicada, puede tener establecido uno 
                o más tipos de ataque, asignado como tipo de efecto en ataque. Alternativamente 
                (y excluyentemente) puede tener establecido un efecto en defensa (véase k).

             j) Valor del efecto en ataque: Cuando una habilidad es auto-aplicada, para cada tipo de 
                efecto en ataque establecido (véase i), debe tener establecido un valor del efecto
                en ataque en puntos de vitalidad.

             k) Efecto en defensa: Cuando una habilidad es auto-aplicada, puede tener establecido 
                uno o más tipos de defensa, asignado como tipo de efecto en defensa. Alternativamente 
                (excluyentemente) puede tener establecido un efecto en ataque (véase i).

             l) Valor del efecto en defensa: Cuando una habilidad es auto-aplicada, para cada tipo de 
                efecto en defensa establecido (véase k), debe tener establecido un valor del efecto
                en defensa en puntos de vitalidad.


        Programación
        ------------

        Se deben modificar las líneas de código que aparecen a continuación.

        Se deben sustituir las asignaciones de valores por asignaciones similares, obtenidas a 
        partir del diseño del juego.

        Los métodos usados en las líneas de código siguientes establecen si se debe usar un tipo de 
        ataque, de defensa o de daño. Por ejemplo, el método 'asignaAtaque' debe ser llamado con un 
        parámetro del tipo (puntero) 'TipoAtaque'.  

        *******************************************************************************************/
        
        ninjaAtaqueCorto->ponCoste(3);
        ninjaAtaqueCorto->ponAlcance(1);
        ninjaAtaqueCorto->asignaAtaque(ataqueCuerpoACuerpo);
        ninjaAtaqueCorto->asignaDefensa(defensaCuerpoACuerpo);
        ninjaAtaqueCorto->asignaDano(danoFisico, 18);

        monjeAtaqueCorto->ponCoste(3);
        monjeAtaqueCorto->ponAlcance(1);
        monjeAtaqueCorto->asignaAtaque(ataqueCuerpoACuerpo);
        monjeAtaqueCorto->asignaDefensa(defensaCuerpoACuerpo);
        monjeAtaqueCorto->asignaDano(danoFisico, 21);
        
        pirataAtaqueCorto->ponCoste(3);
        pirataAtaqueCorto->ponAlcance(1);
        pirataAtaqueCorto->asignaAtaque(ataqueCuerpoACuerpo);
        pirataAtaqueCorto->asignaDefensa(defensaCuerpoACuerpo);
        pirataAtaqueCorto->asignaDano(danoFisico, 24);

        caballeroAtaqueCorto->ponCoste(3);
        caballeroAtaqueCorto->ponAlcance(1);
        caballeroAtaqueCorto->asignaAtaque(ataqueCuerpoACuerpo);
        caballeroAtaqueCorto->asignaDefensa(defensaCuerpoACuerpo);
        caballeroAtaqueCorto->asignaDano(danoFisico, 27);

        samuraiAtaqueCorto->ponCoste(3);
        samuraiAtaqueCorto->ponAlcance(1);
        samuraiAtaqueCorto->asignaAtaque(ataqueCuerpoACuerpo);
        samuraiAtaqueCorto->asignaDefensa(defensaCuerpoACuerpo);
        samuraiAtaqueCorto->asignaDano(danoFisico, 30);

        ninjaAtaqueLargo->ponCoste(3);
        ninjaAtaqueLargo->ponAlcance(2);
        ninjaAtaqueLargo->asignaAtaque(ataqueADistancia);
        ninjaAtaqueLargo->asignaDefensa(defensaADistancia);
        ninjaAtaqueLargo->asignaDano(danoFisico, 21);

        ninjaAtaqueDistancia->ponCoste(5);
        ninjaAtaqueDistancia->ponAlcance(3);
        ninjaAtaqueDistancia->asignaAtaque(ataqueADistancia);
        ninjaAtaqueDistancia->asignaDefensa(defensaADistancia);
        ninjaAtaqueDistancia->asignaDano(danoFisico, 27);

        samuraiAtaqueLargo->ponCoste(3);
        samuraiAtaqueLargo->ponAlcance(2);
        samuraiAtaqueLargo->asignaAtaque(ataqueADistancia);
        samuraiAtaqueLargo->asignaDefensa(defensaADistancia);
        samuraiAtaqueLargo->asignaDano(danoFisico, 34);

        samuraiAtaqueDistancia->ponCoste(5);
        samuraiAtaqueDistancia->ponAlcance(6);
        samuraiAtaqueDistancia->asignaAtaque(ataqueADistancia);
        samuraiAtaqueDistancia->asignaDefensa(defensaADistancia);
        samuraiAtaqueDistancia->asignaDano(danoFisico, 37);

        pirataAtaqueLargo->ponCoste(3);
        pirataAtaqueLargo->ponAlcance(2);
        pirataAtaqueLargo->asignaAtaque(ataqueADistancia);
        pirataAtaqueLargo->asignaDefensa(defensaADistancia);
        pirataAtaqueLargo->asignaDano(danoFisico, 27);

        pirataAtaqueDistancia->ponCoste(5);
        pirataAtaqueDistancia->ponAlcance(5);
        pirataAtaqueDistancia->asignaAtaque(ataqueADistancia);
        pirataAtaqueDistancia->asignaDefensa(defensaADistancia);
        pirataAtaqueDistancia->asignaDano(danoFisico, 30);

        caballeroAtaqueLargo->ponCoste(3);
        caballeroAtaqueLargo->ponAlcance(2);
        caballeroAtaqueLargo->asignaAtaque(ataqueADistancia);
        caballeroAtaqueLargo->asignaDefensa(defensaADistancia);
        caballeroAtaqueLargo->asignaDano(danoFisico, 31);

        caballeroAtaqueDistancia->ponCoste(5);
        caballeroAtaqueDistancia->ponAlcance(4);
        caballeroAtaqueDistancia->asignaAtaque(ataqueADistancia);
        caballeroAtaqueDistancia->asignaDefensa(defensaADistancia);
        caballeroAtaqueDistancia->asignaDano(danoFisico, 35);

        monjeAtaqueLargo->ponCoste(3);
        monjeAtaqueLargo->ponAlcance(2);
        monjeAtaqueLargo->asignaAtaque(ataqueADistancia);
        monjeAtaqueLargo->asignaDefensa(defensaADistancia);
        monjeAtaqueLargo->asignaDano(danoFisico, 24);

        monjeAtaqueDistancia->ponCoste(5);
        monjeAtaqueDistancia->ponAlcance(3);
        monjeAtaqueDistancia->asignaAtaque(ataqueADistancia);
        monjeAtaqueDistancia->asignaDefensa(defensaADistancia);
        monjeAtaqueDistancia->asignaDano(danoFisico, 29);

        /*TODO: LOS ESPECIALES NO ESTAN BIEN*/
        ninjaAtaqueEspecial1->ponCoste(7);
        ninjaAtaqueEspecial1->ponAlcance(1);
        ninjaAtaqueEspecial1->ponRadioAlcance(2);
        //TODO: No se puede poner esta habilidad sin curacion, asumimos que tiene un efecto de recuperacion minimo
        ninjaAtaqueEspecial1->asignaCuracion(1);

        ninjaAtaqueEspecial2->ponCoste(7);
        ninjaAtaqueEspecial2->ponAlcance(2);
        ninjaAtaqueEspecial2->asignaAtaque(ataqueADistancia);
        ninjaAtaqueEspecial2->asignaDefensa(defensaADistancia);
        ninjaAtaqueEspecial2->asignaDano(danoFisico, 40);

        monjeAtaqueEspecial1->ponCoste(6);
        monjeAtaqueEspecial1->ponAlcance(4);
        monjeAtaqueEspecial1->asignaAtaque(ataqueCuerpoACuerpo);
        monjeAtaqueEspecial1->asignaDefensa(defensaCuerpoACuerpo);
        monjeAtaqueEspecial1->asignaDano(danoFisico, 48);

        monjeAtaqueEspecial2->ponCoste(8);
        monjeAtaqueEspecial2->ponAlcance(2);
        monjeAtaqueEspecial2->ponRadioAlcance(3);
        monjeAtaqueEspecial2->asignaCuracion(50);

        caballeroAtaqueEspecial1->ponCoste(5);
        caballeroAtaqueEspecial1->agregaEfectoDefensa(defensaCuerpoACuerpo, 30);
        caballeroAtaqueEspecial1->agregaEfectoDefensa(defensaADistancia, 30);

        caballeroAtaqueEspecial2->ponCoste(7);
        caballeroAtaqueEspecial2->agregaEfectoDefensa(defensaCuerpoACuerpo, 10);

        pirataAtaqueEspecial1->ponCoste(3);
        pirataAtaqueEspecial1->ponAlcance(4);
        pirataAtaqueEspecial1->asignaAtaque(ataqueADistancia);
        pirataAtaqueEspecial1->asignaDefensa(defensaADistancia);
        pirataAtaqueEspecial1->asignaDano(danoFisico, 38);

        pirataAtaqueEspecial2->ponCoste(8);
        pirataAtaqueEspecial2->ponAlcance(6);
        pirataAtaqueEspecial2->asignaAtaque(ataqueADistancia);
        pirataAtaqueEspecial2->asignaDefensa(defensaADistancia);
        pirataAtaqueEspecial2->asignaDano(danoFisico, 30);
        pirataAtaqueEspecial2->ponRadioAlcance(3);

        samuraiAtaqueEspecial1->ponCoste(7);
        samuraiAtaqueEspecial1->agregaEfectoDefensa(defensaCuerpoACuerpo, 30);

        samuraiAtaqueEspecial2->ponCoste(5);
        samuraiAtaqueEspecial2->ponAlcance(3);
        samuraiAtaqueEspecial2->asignaAtaque(ataqueADistancia);
        samuraiAtaqueEspecial2->asignaDefensa(defensaADistancia);
        samuraiAtaqueEspecial2->asignaDano(danoFisico, 20);
        samuraiAtaqueEspecial2->ponRadioAlcance(3);

        /*
        ataqueEspadaNormal->ponCoste (3);                                       
        ataqueEspadaNormal->ponAlcance (1);
        ataqueEspadaNormal->asignaAtaque  (ataqueCuerpoACuerpo); 
        ataqueEspadaNormal->asignaDefensa (defensaCuerpoACuerpo); 
        ataqueEspadaNormal->asignaDano    (danoFisico, 20); 
        //
        ataqueArco->ponCoste (5);
        ataqueArco->ponAlcance (15);
        ataqueArco->asignaAtaque  (ataqueADistancia);
        ataqueArco->asignaDefensa (defensaADistancia);
        ataqueArco->asignaDano    (danoFisico, 20);
        //
        ataqueEspadaPoderoso->ponCoste (5);
        ataqueEspadaPoderoso->ponAlcance (1);
        ataqueEspadaPoderoso->asignaAtaque  (ataqueCuerpoACuerpo);
        ataqueEspadaPoderoso->asignaDefensa (defensaCuerpoACuerpo);
        ataqueEspadaPoderoso->asignaDano    (danoFisico, 50);
        //
        defensaFerrea->ponCoste (3);
        defensaFerrea->agregaEfectoDefensa (defensaCuerpoACuerpo, 30);
        defensaFerrea->agregaEfectoDefensa (defensaADistancia,    30);
        //
        curacionSimple->ponCoste (5);
        curacionSimple->ponAlcance (3);  
        curacionSimple->asignaCuracion (40);
        //
        curacionGrupo->ponCoste (10);
        curacionGrupo->ponAlcance (3);  
        curacionGrupo->ponRadioAlcance (3);
        curacionGrupo->asignaCuracion (40);
        //
        proyectilMagico->ponCoste (7);
        proyectilMagico->ponAlcance (15);                                            
        proyectilMagico->asignaAtaque  (ataqueMagico);
        proyectilMagico->asignaDefensa (defensaMagica);
        proyectilMagico->asignaDano    (danoMagico, 30);
        //
        bolaFuego->ponCoste (10);
        bolaFuego->ponAlcance (8);
        bolaFuego->ponRadioAlcance (3);
        bolaFuego->asignaAtaque  (ataqueMagico);
        bolaFuego->asignaDefensa (defensaMagica);
        bolaFuego->asignaDano    (danoMagico, 50);
        */
        /*******************************************************************************************
        /******************************************************************************************/
    }


    void JuegoMesa::agregaEstadisticasPersonajes () {
        /*******************************************************************************************
        /*******************************************************************************************
        
        CONFIGURACIÓN DEL JUEGO
        Décima sección - Los tipos de ataque, defensa y reducción de daño de los personajes

        En esta sección se establecen que tipos de ataque, defensa y reducción de daño tiene cada 
        personajes del juego, junto con la correspondiente puntuación.

        Configuración
        -------------

        La configuración de los tipos se debe realizar de la siguiente forma:

             a) Si un personaje tiene una habilidad y esta habilidad tiene un tipo de ataque, el 
                personaje debe tener ese tipo de ataque.

             b) Cada personaje debe tener todos los tipos de defensa.

             c) Cada personaje debe tener todos los tipos de daño, en forma de reducción de daño.

             d) Si un personaje tiene una habilidad y esta habilidad tiene un efecto en ataque,
                que está definido usando un tipo de ataque, el personaje debe tener dicho tipo de 
                ataque.

             e) Si un personaje tiene una habilidad y esta habilidad tiene un efecto en defensa,
                que está definido usando un tipo de defensa, el personaje debe tener dicho tipo de 
                defensa. Esta propiedad está establecida previamente mediante el punto (b)

             f) Cada tipo de ataque, defensa y reducción de daño del personaje debe tener una 
                puntuación. La puntuación no puede ser negativa.        

        Programación
        ------------

        Se deben modificar las líneas de código que aparecen a continuación.

        Se deben sustituir las listas de tipos de ataque, defensa y reducción de daño de cada 
        personaje por listas similares, obtenidas a partir del diseño del juego.

        Los métodos usados en las líneas de código siguientes establecen si el tipo es ataque, 
        defensa o daño. Por ejemplo, el método 'agregaReduceDano' debe ser llamado con un parámetro 
        del tipo (puntero) 'TipoDano'.  

        *******************************************************************************************/
        //
        Monje1->agregaAtaque(ataqueCuerpoACuerpo, 55);
        Monje1->agregaAtaque(ataqueADistancia, 57);
        Monje1->agregaDefensa(defensaCuerpoACuerpo, 58);
        Monje1->agregaDefensa(defensaADistancia, 60);
        Monje1->agregaReduceDano(danoFisico, 10);
        
        Samurai1->agregaAtaque(ataqueCuerpoACuerpo, 71);
        Samurai1->agregaAtaque(ataqueADistancia, 74);
        Samurai1->agregaDefensa(defensaCuerpoACuerpo, 60);
        Samurai1->agregaDefensa(defensaADistancia, 68);
        Samurai1->agregaReduceDano(danoFisico, 5);
                           
        Ninja1->agregaAtaque(ataqueCuerpoACuerpo, 50);
        Ninja1->agregaAtaque(ataqueADistancia, 54);
        Ninja1->agregaDefensa(defensaCuerpoACuerpo, 60);
        Ninja1->agregaDefensa(defensaADistancia, 68);
        Ninja1->agregaReduceDano(danoFisico, 5);

        Caballero1->agregaAtaque(ataqueCuerpoACuerpo, 68);
        Caballero1->agregaAtaque(ataqueADistancia, 70);
        Caballero1->agregaDefensa(defensaCuerpoACuerpo, 70);
        Caballero1->agregaDefensa(defensaADistancia, 74);
        Caballero1->agregaReduceDano(danoFisico, 8);
                               
        Pirata1->agregaAtaque(ataqueCuerpoACuerpo, 66);
        Pirata1->agregaAtaque(ataqueADistancia, 68);
        Pirata1->agregaDefensa(defensaCuerpoACuerpo, 60);
        Pirata1->agregaDefensa(defensaADistancia, 68);
        Pirata1->agregaReduceDano(danoFisico, 6);

        // Equipo rival
        Samurai2->agregaAtaque(ataqueCuerpoACuerpo, 71);
        Samurai2->agregaAtaque(ataqueADistancia, 74);
        Samurai2->agregaDefensa(defensaCuerpoACuerpo, 60);
        Samurai2->agregaDefensa(defensaADistancia, 68);
        Samurai2->agregaReduceDano(danoFisico, 5);

        Ninja2->agregaAtaque(ataqueCuerpoACuerpo, 50);
        Ninja2->agregaAtaque(ataqueADistancia, 54);
        Ninja2->agregaDefensa(defensaCuerpoACuerpo, 60);
        Ninja2->agregaDefensa(defensaADistancia, 68);
        Ninja2->agregaReduceDano(danoFisico, 5);

        Caballero2->agregaAtaque(ataqueCuerpoACuerpo, 68);
        Caballero2->agregaAtaque(ataqueADistancia, 70);
        Caballero2->agregaDefensa(defensaCuerpoACuerpo, 70);
        Caballero2->agregaDefensa(defensaADistancia, 74);
        Caballero2->agregaReduceDano(danoFisico, 8);

        Pirata2->agregaAtaque(ataqueCuerpoACuerpo, 66);
        Pirata2->agregaAtaque(ataqueADistancia, 68);
        Pirata2->agregaDefensa(defensaCuerpoACuerpo, 60);
        Pirata2->agregaDefensa(defensaADistancia, 68);
        Pirata2->agregaReduceDano(danoFisico, 6);

        Monje2->agregaAtaque(ataqueCuerpoACuerpo, 55);
        Monje2->agregaAtaque(ataqueADistancia, 57);
        Monje2->agregaDefensa(defensaCuerpoACuerpo, 58);
        Monje2->agregaDefensa(defensaADistancia, 60);
        Monje2->agregaReduceDano(danoFisico, 10);
         
        //
        /*******************************************************************************************
        /******************************************************************************************/
    }


    void JuegoMesa::preparaSistemaAtaque () {
        /*******************************************************************************************
        /*******************************************************************************************
        
        CONFIGURACIÓN DEL JUEGO
        Séptima sección - Los grados de efectividad - Segunda parte

        En esta sección se configuran los grados de efectividad de los ataques del juego. En la 
        primera parte se establecen las variables destinadas a almacenar las instancias que definen 
        los grados de efectividad. En esta parte se crean las instancias y se configuran.

        Las variable usan un nombre similar al nombre de los tipos correspondiente. Las variables 
        son de tipo puntero, la creación de las instancias utiliza la palabra reservada 'new'. 

        Atributos
        ---------

        Se asignan los siguientes atributos a los tipos:

             a) Descripción: Asignado en la creación de la instancia en una cadena de caracteres. 
                Deben ser cortos.

             b) Intervalo de puntos: Es el intervalo de puntos al que se aplica la efectividad. 
                Se establece con un valor mínimo y uno máximo. En el primer intervalo el valor 
                mínimo es el número entero mínimo y en el último intervalo, el valor máximo es 
                el número entero máximo.

             c) Efectividad: Es un porcentaje que se aplica a una puntuación para modificar su 
                valor cuando dicha puntuación esta en el intervalo definido por el intervalo
                correspondiente. 
            
        Programación
        ------------

        Se deben modificar las líneas de código que aparecen a continuación.

        Se debe sustituir las listas de creaciones de instancias por listas similares, obtenidas a 
        partir de la lista de grados de efectividad diseñados para el juego. 

        Se deben cambiar los valores de los parámetros según los valores establecidos para cada 
        grados de efectividad en el diseño del juego.

        *******************************************************************************************/
        //
        GradoEfectividad * fallo   = new GradoEfectividad   {L"Fallo"};
        GradoEfectividad * roce    = new GradoEfectividad   {L"Roce"};
        GradoEfectividad * golpe   = new GradoEfectividad   {L"Golpe"};
        GradoEfectividad * critico = new GradoEfectividad   {L"Impacto crítico"};
        //
        fallo  ->estableceRango (INT_MIN,      29,    0);
        roce   ->estableceRango (     30,      49,   50);
        golpe  ->estableceRango (     50,      94,  100);
        critico->estableceRango (     95, INT_MAX,  130);
        //
        agregaEfectividad (fallo);
        agregaEfectividad (roce);
        agregaEfectividad (golpe);
        agregaEfectividad (critico);
        //
        /*******************************************************************************************
        /******************************************************************************************/
    }


    void JuegoMesa::configuraJuego () {
        agregaMusica (new ActorMusica {this});
        //
        /*******************************************************************************************
        /*******************************************************************************************
        
        CONFIGURACIÓN DEL JUEGO
        Undécima sección - Configuración miscelánea del juego.

        En esta sección se establecen varios valores de configuración que no tienen sitio en otras 
        secciones.

        Configuración
        -------------

        La configuración tiene los siguientes aspectos:

                 a) Se establece el consumo de puntos de acción de los desplazamientos de los 
                    personajes. Para ello se indica el número píxeles de desplazamiento que 
                    originan el consumo de un punto de acción.

                 b) Se establece la posición inicial de las fichas de los personajes en el tablero 
                    de juego.
         
                 c) Se establecen los archivos de sonido, que se escuchan en momentos concretos del
                    juego. Se deben especificar los sonidos:

                      - Sonido de una pulsación con el ratón (clic)
                      - Sonido de desplazamiento de una ficha.

                 d) Se establece la música que se escucha con el juego. En un fragmento musical que 
                    se repite en bucle.

                 e) Se almacenan los nombres de los alumnos diseñadores del juego. Estos nombres
                    aparecen en la ayuda del juego.

                 f) Se almacena el curso académico durante el que los alumnos han elaborado el 
                    diseño del juego.

        Sonidos y música
        ----------------

        Los sonidos se cargan de archivos de tipo 'wav', 'ogg' o 'flac', pero no 'mp3'. El tiempo de
        emisión del sonido debe ser corto (escasos segundos).
        
        Los sonidos y la música se han tomado de:
                https://opengameart.org/content/metal-click
                https://opengameart.org/content/walking-on-snow-sound
        La licencia aparece en esas mismas páginas.

        La música que suena en momentos determinados del juego también se carga de un archivo. Sin 
        embargo, no es necesario que sea corta, por que se atenúa a los pocos segundos.

        La música se han tomado de:
                https://opengameart.org/content/unused-music
        La licencia aparece en esa misma página.

        Programación
        ------------

        Se deben modificar las líneas de código que aparecen a continuación.

        Se deben sustituir las listas de personajes por listas similares, obtenidas a partir del 
        diseño del juego.

        *******************************************************************************************/
        //
        modo ()->configuraDesplaza (RejillaTablero::distanciaCeldas);
        //
        Samurai1 ->ponSitioFicha (Coord {23, 15});
        Ninja1  ->ponSitioFicha (Coord {17, 15});
        Monje1->ponSitioFicha (Coord {29, 15});
        Pirata1->ponSitioFicha (Coord {35, 15});
        Caballero1->ponSitioFicha(Coord{ 39, 15 });
        Samurai2->ponSitioFicha (Coord {23, 35});
        Ninja2 ->ponSitioFicha (Coord {17, 35});
        Monje2->ponSitioFicha (Coord {29, 35});
        Pirata2 ->ponSitioFicha (Coord {35, 35});
        Caballero2->ponSitioFicha(Coord{ 39, 35 });
        //
        tablero ()->asignaSonidoEstablece (carpeta_sonidos_juego + "Metal Click.wav", 100);
        tablero ()->asignaSonidoDesplaza  (carpeta_sonidos_juego + "SnowWalk.ogg",    100);
        switch (numeroAleatorio) {
            case 1:
                // Acción para el número 1
                musica   ()->asignaMusica  (carpeta_sonidos_juego + "a1.ogg",20);
                break;
            case 2:
                // Acción para el número 2
                musica   ()->asignaMusica  (carpeta_sonidos_juego + "a2.ogg",20);
                break;
            case 3:
                // Acción para el número 3
                musica   ()->asignaMusica  (carpeta_sonidos_juego + "a3.ogg",20);
                break;
            default:
                // Acción por defecto (en caso de un número fuera del rango)
                musica   ()->asignaMusica  (carpeta_sonidos_juego + "a1.ogg",20);
                break;
        }
        //musica  ()->asignaMusica (carpeta_sonidos_juego + "Track_1.ogg",10);
        //
        agregaNombreAlumno (L"Adrian Lopez Diaz Tendero");
        agregaNombreAlumno (L"Antonio Suano Lara");
        agregaNombreAlumno (L"Ahmed Ahmad Anino");
        agregaNombreAlumno (L"Darius Daniel Rat");
        agregaNombreAlumno (L"Denis Iulian Prodisteanu");
        indicaCursoAcademico (L"2022 - 2023");
        //
        /*******************************************************************************************
        /******************************************************************************************/
    }


    void JuegoMesa::termina () {
        //
        JuegoMesaBase::termina ();
        // Personajes
        Ninja1 = nullptr;
        Ninja2 = nullptr;
        Caballero1 = nullptr;
        Caballero2 = nullptr;
        Samurai1 = nullptr;
        Samurai2 = nullptr;
        Pirata1 = nullptr;
        Pirata2 = nullptr;
        Monje1 = nullptr;
        Monje2 = nullptr;

        // Habilidades
        ninjaAtaqueCorto = nullptr;
        ninjaAtaqueLargo = nullptr;
        ninjaAtaqueDistancia = nullptr;
        ninjaAtaqueEspecial1 = nullptr;
        ninjaAtaqueEspecial2 = nullptr;

        caballeroAtaqueCorto = nullptr;
        caballeroAtaqueLargo = nullptr;
        caballeroAtaqueDistancia = nullptr;
        caballeroAtaqueEspecial1 = nullptr;
        caballeroAtaqueEspecial2 = nullptr;

        pirataAtaqueCorto = nullptr;
        pirataAtaqueLargo = nullptr;
        pirataAtaqueDistancia = nullptr;
        pirataAtaqueEspecial1 = nullptr;
        pirataAtaqueEspecial2 = nullptr;

        samuraiAtaqueCorto = nullptr;
        samuraiAtaqueLargo = nullptr;
        samuraiAtaqueDistancia = nullptr;
        samuraiAtaqueEspecial1 = nullptr;
        samuraiAtaqueEspecial2 = nullptr;

        monjeAtaqueCorto = nullptr;
        monjeAtaqueLargo = nullptr;
        monjeAtaqueDistancia = nullptr;
        monjeAtaqueEspecial1 = nullptr;
        monjeAtaqueEspecial2 = nullptr;

        //
        ataqueCuerpoACuerpo  = nullptr;
        ataqueADistancia     = nullptr;
        //ataqueMagico         = nullptr;
        defensaCuerpoACuerpo = nullptr;
        defensaADistancia    = nullptr;
        //defensaMagica        = nullptr;
        danoFisico           = nullptr;
        //danoMagico           = nullptr;
    }


}