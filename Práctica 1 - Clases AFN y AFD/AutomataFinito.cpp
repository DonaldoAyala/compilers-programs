/*
	INSTITUTO POLITÉCNICO NACIONAL
	ESCUELA SUPERIOR DE CÓMPUTO
	COMPILADORES
	ALUMNO: 
		AYALA SEGOVIANO DONALDO HORACIO - 3CV1
	PROFESOR: NORMAN RAFAEL SAUCEDO DELGADO
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	PRÁCTICA 1: 
	TÍTULO: IMPLEMENTACION DE CLASES AFN Y AFD
	FECHA: 23/OCTUBRE/2020 (ENTREGA: 25/OCTUBRE/2020)
*/
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <unordered_set>
#include <map>
#include <set>
#include <vector>
#include <utility>

using namespace std;

/*
    La clase Thomson es un conversor de
    autómatas finitos no deterministas a autómatas
    finitos deterministas.

*/
class Thomson {
    private:
        /*
            e_cerradura_recursiva recolecta los estados 
            correspondientes a la operación de e_cerradura
            para un solo estado específico.
            recibe:  transiciones, alcance, estado_inicial
            regresa: void
            ** alcance es una referencia y  guarda los estados
            ** alcanzados por la operacion  **
        */
        void e_cerradura_recursiva ( 
                map<pair<int, char>,unordered_set<int>> transiciones,
                set<int> &alcance,
                int e_actual )
        {
            alcance.insert(e_actual);
            for ( auto estado_sig : transiciones[{e_actual,'E'}] )
            {
                transiciones[{e_actual,'E'}].erase(e_actual);
                e_cerradura_recursiva ( transiciones, alcance, estado_sig );
            }
        }
        /*
            e_cerradura ejecuta la función de cerradura sobre un 
            conjunto de estados. Usa la función e_cerradura_recursiva
            para obtener los estados correspondientes
            recibe:  transiciones, estado_inicio
            regresa: conjunto de estados
        */
        set<int> e_cerradura (
            map<pair<int, char>,unordered_set<int>> transiciones,
            set<int> inicio)
        {
            set<int> cerradura;
            set<int> temp;
            vector<int> v;
            for ( auto estado : inicio )
            {
                e_cerradura_recursiva ( transiciones, temp, estado );
                
	            merge(temp.begin(), temp.end(), cerradura.begin(), cerradura.end(), back_inserter(v));
                cerradura = set<int> (v.begin(), v.end());
            }
            return cerradura;
        }

        /*
            mover ejecuta la operación mover del algoritmo,
            busca todos los estados a los que se puede desde
            un conjunto de estados con un simbolo
            recibe: transiciones, estados_iniciales, simbolo
        */
        set<int> mover(
            map<pair<int, char>,unordered_set<int>> transiciones,
            set<int> T,
            char a )
        {
            set<int> temp;
            for ( auto estado: T )
            {
                for ( auto destino : transiciones[{estado, a}] )
                {
                    temp.insert( destino );
                }
            }
            return temp;
        }
    public:

        /*
            AFN_a_AFD convierte las transiciones de un AFN
            a transiciones equivalentes de un AFD mediante el algoritmo
            de construcción de subconjuntos
            recibe:  transiciones del AFN
            regresa: transiciones de un AFD equivalentes
        */
        map<pair<int, char>,unordered_set<int>> 
        AFN_a_AFD (
            map<pair<int, char>,unordered_set<int>> transiciones)
        {
            map<pair<int, char>,unordered_set<int>> transiciones_afd;
            set < set<int> > destados;
            set < set<int> > marcados;
            set<int> T;
            set<int> U;
            T = e_cerradura( transiciones, set<int>{1});
            destados.insert(T); 
            set<int>  temp = mover( transiciones, T, 'a' );
            map<set<int>, int> id;
            int cont_id = 1;
            id[T] = cont_id;
            cont_id++;
            while ( destados.size() )
            {
                T = *destados.begin();
                destados.erase(T);
                marcados.insert(T);
                for ( int i = 'a' ; i <= 'z' ; i++ )
                {
                    U = e_cerradura ( transiciones, mover( transiciones, T, i) );
                    if ( !destados.count(U) && !marcados.count(U) )
                    {
                        destados.insert( U );
                        id[U] = cont_id;
                        cont_id++;
                    }
                    unordered_set<int> v;v.insert(id[U]);
                    transiciones_afd[{id[T], i}] = v;
                }
            }
            return transiciones_afd;
        }
};


/*
    La clase AutomataFinito implementará el funcionamiento
    de un autómata, implementará los métodos específicados en
    la práctica.
*/
class AutomataFinito {
    private:
        int estado_inicial;
        set<int> estados_finales;
        /*
            Las transiciones del autómata se guardaran de
            la forma:
                map < (inicio, simbolo) , {finales} >
        */
        map < pair<int, char>, unordered_set<int> > transiciones;
    
    public:

        /*
            La funcion cargar_desde cargará un autómata
            desde un archivo.
            recibe:  string ruta
            regresa: bool:exito
                    (verdadero si se puede cargar)
                    (falso si no se puede cargar)
            
            El formato del autómata
            contenido del archivo será:

            inicio:estado1                  // estado inicial
            finales:estado1,estado2,...     // estados finales
            inicio->fin,simbolo             // transiciones
            inicio->fin,simbolo
            inicio->fin,simbolo
            ...

            por ejemplo:

            inicial:1
            finales:11
            1->2,E
            1->8,E
            2->3,E
            2->5,E
            3->4,a
            5->6,b
            6->7,E
            4->7,E
            7->2,E
            7->8,E
            8->9,a
            9->10,b
            11->11,b
        */
        bool cargar_desde (const string &ruta) 
        {
            fstream archivo (ruta);
            string linea;
            if ( archivo.is_open() )
            {
                // Lee el estado inicial
                getline(archivo, linea);
                linea =  dividir_cadena(linea, ":")[1];
                estado_inicial = stoi(linea);

                // Lee los estados finales
                getline(archivo, linea);
                linea = dividir_cadena( linea, ":" )[1];
                vector<string> temp = dividir_cadena(linea, ",");
                for ( auto str_estado : temp )
                {
                    estados_finales.insert( stoi(str_estado) );
                }

                // Lee las transiciones
                string de, hacia;
                char simbolo;
                while ( getline(archivo, linea) ) 
                {
                    if ( linea.empty() ) continue;
                    temp = dividir_cadena(linea, "->");
                    de = temp[0];
                    hacia = dividir_cadena(temp[1], ",")[0];
                    simbolo = dividir_cadena(temp[1], ",")[1][0];
                    transiciones[{stoi(de), simbolo}].insert(stoi(hacia));
                }
            }
            else
            {
                return false;
            }
            return true;
        }
        
        /*
            El método guardar_en guardará el autómata contenido
            en transiciones con el mismo formato de entrada.
            recibe:  string:ruta
            entrega: bool:exito
                    (verdadero si se puede guardar)
                    (falso si no se puede guardar)
        */
        bool guardar_en (const string &ruta)
        {
            ofstream archivo (ruta);
            string linea;
            if ( archivo.is_open() )
            {
                archivo << "inicio:" << estado_inicial << "\n";
                archivo << "finales:";
                int no_estados = estados_finales.size();
                for ( auto estado : estados_finales )
                {   
                    if ( --no_estados )
                        archivo << estado << ",";
                    else
                        archivo << estado;
                }
                archivo << "\n";
                for ( auto tr : transiciones )
                {
                    for ( auto destino : tr.second )
                    {
                        archivo << tr.first.first 
                                << "->" 
                                << destino << "," 
                                << tr.first.second 
                                << "\n";
                    }
                }
                archivo.close();
            }
            return true;
        }
                
        /*
            dividir_cadena divide una cadena según un separador
            recibe:  string:cadena, string:separador
            regresa: vector<string>:vector de cadenas divididas por el separador
        */
        vector<string> dividir_cadena ( const string cadena, string separador ) 
        {
            string valor = "";
            vector<string> valores;
            for ( int i = 0 ; i < cadena.size() ; i++ )
            {
                if ( cadena.substr(i, separador.size() ) == separador)
                {
                    if ( valor.size() > 0 )
                    {
                        valores.push_back(valor);
                        valor = "";
                        i += separador.size() - 1;
                    }
                }
                else 
                {
                    valor.push_back( cadena[i] );
                }
            }
            if ( valor.size() > 0 )
            {
                valores.push_back(valor);
            }
            return valores;
        }
        
        /*
            agregar_transicion agrega la transicion indicada al
            autómata cargado en memoria
            recibe:  int:inicio, int:fin, char:simbolo
            regresa: vacío
        */
        void agregar_transicion ( int inicio, int fin, char simbolo )
        {
            transiciones[{ inicio, simbolo }].insert(fin);
        }
        
        /*
            eliminar_transicion remueve una transicion del autómata
            cargado en memoria
            recibe:  int:inicio, int:fin, char:simbolo
            regresa: vacío
        */
        void eliminar_transicion ( int inicio, int fin, char simbolo ) 
        {
            transiciones[{ inicio, simbolo }].insert(fin);
        }

        /*
            obtener_inicial regresa el estado inicial del autómata 
            cargado en memoria
            recibe:  vacío
            regresa: int:estado_inicial
        */
        int obtener_inicial()
        {
            return estado_inicial;
        }

        /*
            obtener_finales regresa el conjunto de estados finales
            del autómata cargado en memoria
            recibe:  vacío
            regresa: vector<int>:estado_inicial
        */
        vector<int> obtener_finales()
        {
            vector<int> temp;
            for (auto x : estados_finales)
                temp.push_back( x );
            return temp;
        }

        /*
            obtener_finales regresa el conjunto de estados finales
            del autómata cargado en memoria
            recibe:  vacío
            regresa: vector<int>:estado_inicial
        */
        void establecer_inicial ( int estado )
        {
            estado_inicial = estado;
        }

        /*
            obtener_finales agrega un estado al conjunto de
            estados finales
            recibe:  int:estado
            regresa: vacío
        */
        void establecer_final ( int estado )
        {
            estados_finales.insert(estado);
        }

        /*
            esAFN verifica si el automata cargado en memoria
            es un Autómata Finito No Determinista
            recibe:  vacío
            regresa: bool:(true si es AFN)
        */
        bool esAFN ()
        {
            set < pair<int,char> > transiciones_vis;
            for ( auto transicion : transiciones )
            {
                if ( transicion.first.second == 'E' || transiciones_vis.count(transicion.first) )
                {
                    return true;
                }
                else
                {
                    transiciones_vis.insert( transicion.first );
                }   
            }
            return false;
        }
        
        /*
            esAFD verifica si el automata cargado en memoria
            es un Autómata Finito Determinista
            recibe:  vacío
            regresa: bool:(true si es AFD)
        */
        bool esAFD ()
        {
            set < pair<int,char> > transiciones_vis;
            for ( auto transicion : transiciones )
            {
                if ( transicion.first.second == 'E' || transiciones_vis.count(transicion.first) )
                {
                    return false;
                }
                else
                {
                    transiciones_vis.insert( transicion.first );
                }   
            }
            return true;
        }
        
        /*
            acetpa_cadena verifica si un autómata acepta
            una cadena
            recibe:  vacío
            regresa: bool:(true si es AFN)
        */
        bool acepta_cadena ( string cadena )
        {
            // Para un AFD
            if ( esAFD() )
            {
                int estado_actual = estado_inicial;
                for ( int i = 0 ; i < cadena.size() ; i++ ) 
                {
                    if ( transiciones.count({estado_actual, cadena[i]}) )
                    {
                        estado_actual = *transiciones[{estado_actual, cadena[i]}].begin();
                    }
                    else
                    {
                        return false;
                    }
                }
                if ( estados_finales.count(estado_actual) )
                    return true;
            }
            else
            // Para un AFN se convierte en AFD
            {
                Thomson conversor;
                map < pair<int, char>, unordered_set<int> > afd_temporal = conversor.AFN_a_AFD(transiciones);
                int estado_actual = estado_inicial;
                for ( int i = 0 ; i < cadena.size() ; i++ ) 
                {
                    if ( afd_temporal.count({estado_actual, cadena[i]}) )
                    {
                        estado_actual = *afd_temporal[{estado_actual, cadena[i]}].begin();
                    }
                    else
                    {
                        return false;
                    }
                }
                if ( estados_finales.count(estado_actual) )
                    return true;
            }
            return false;
        }
        /*
            generar_cadena genera una cadena que será aceptada por el
            autómata cargado en memoria. Usa la función auxiliar
            generar_cadena_recursiva para generar la cadena.
            recibe:  vacío
            regresa: cadena_generada
        */
        string generar_cadena()
        {
            map<pair<int, char>,unordered_set<int>> transiciones_aux = transiciones;
            string acumulado = "";
            string cadena = ""; 
            int estado = estado_inicial;
            generar_cadena_recursivo ( transiciones_aux, acumulado, cadena, estado );
            
            return cadena;
        }

        /*
            generar_cadena_recursiva genera una cadena mediante una
            búsqueda de produndidad en el autómata cargado en memoria
            recibe:  vacío
            regresa: cadena_generada
        */
        void generar_cadena_recursivo (
            map<pair<int, char>,unordered_set<int>> transiciones_aux,
            string &acumulado, 
            string &cadena, 
            int estado )
        {
            if ( estados_finales.count(estado) )
            {
                cadena = acumulado;
                return;
            }
            for ( int i = 'a' ; i <= 'z' ; i++ )
            {
                if ( transiciones_aux.count({estado, i}) || transiciones_aux.count({estado, 'E'}))
                {
                    int estado_sig = *transiciones[{estado, i}].begin();
                    transiciones_aux[{estado, i}].erase(estado_sig);
                    acumulado.push_back(i);
                    generar_cadena_recursivo(transiciones_aux, acumulado, cadena, estado_sig);
                    acumulado.pop_back();
                }
            }
        }
};
/*
    La clase AutomataFinitoNoDeterminista hereda de AutomataFinito
    todas las funciones especificadas en la práctica
*/
class AutomataFinitoNoDeterminista : public AutomataFinito {

};

/*
    La clase AutomataFinitoDeterminista hereda de AutomataFinito
    todas las funciones especificadas en la práctica
*/
class AutomataFinitoDeterminista : public AutomataFinito {
    
};

int main () {
    
    return 0;
}
