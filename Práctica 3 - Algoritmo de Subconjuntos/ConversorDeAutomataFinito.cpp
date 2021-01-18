/*
	INSTITUTO POLITÉCNICO NACIONAL
	ESCUELA SUPERIOR DE CÓMPUTO
	COMPILADORES
	ALUMNO: 
		AYALA SEGOVIANO DONALDO HORACIO - 3CV1
	PROFESOR: NORMAN RAFAEL SAUCEDO DELGADO
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	PRÁCTICA 3: 
	TÍTULO: Implementación de un algoritmo para 
            convertir un AFN a un AFD
	FECHA: 06/NOVIEMBRE/2020 (ENTREGA: 8/NOVIEMBRE/2020)
*/

#include <algorithm>
#include <string>
#include <unordered_set>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include "AutomataFinito.cpp"
#include "Thompson.cpp"

using namespace std;


class ConversorDeAutomataFinitoNoDeterminista
{
    private:
        map<pair<int, char>,unordered_set<int>> transiciones_auxiliares;
        map<pair<int, char>,unordered_set<int>> Dtran;
        set<set<int>> Destados;
        int etiqueta;
        map<set<int>, int> etiqueta_de_subconjunto;


        set<int> e_cerradura ( set<int> T )
        {
            stack<int> ultimos_estados_visitados;
            set<int> estados_visitados;
            for (auto estado : T)
            {
                ultimos_estados_visitados.push(estado);
                estados_visitados.insert(estado);
            }
            int estado_actual;
            while ( !ultimos_estados_visitados.empty() )
            {
                estado_actual = ultimos_estados_visitados.top();
                ultimos_estados_visitados.pop();
                for ( auto estado_siguiente : transiciones_auxiliares[{estado_actual, 'E'}] )
                {
                    if ( estados_visitados.count(estado_siguiente) == 0 )
                    {
                        estados_visitados.insert(estado_siguiente);
                        ultimos_estados_visitados.push(estado_siguiente);
                    }
                }
            }
            return estados_visitados;
        }

        set<int> mover( set<int> inicio, char simbolo )
        {
            set<int> alcanzables;
            for ( auto estado : inicio )
            {
                if ( transiciones_auxiliares.count({estado, simbolo}) == 1 )
                {
                    alcanzables.insert  (
                                        transiciones_auxiliares[{estado, simbolo}].begin(),
                                        transiciones_auxiliares[{estado, simbolo}].end()
                                        );
                }
            }
            return alcanzables;
        }

        set<int> obtener_subconjunto_sin_marcar( const set<set<int>> &visitados)
        {
            for ( auto x : Destados )
            {
                if ( visitados.count(x) == 0 )
                {
                    return x;
                }
            }
            return set<int>();
        }
    public:
        AutomataFinitoDeterminista 
        convertir_AFN_a_AFD ( AutomataFinitoNoDeterminista afn )
        {
            set<int> estados_finales_afn = afn.obtener_estados_finales();
            AutomataFinitoDeterminista afd;
            set<set<int>> visitados;
            etiqueta = 1;
            transiciones_auxiliares = afn.obtener_transiciones();
            set<int> T = e_cerradura(set<int>{afn.obtener_inicial()});
            Destados.insert(T);
            etiqueta_de_subconjunto[T] = etiqueta++;
            set<int> U;
            while ( Destados.size() > visitados.size() )
            {
                // Se obtiene un conjunto sin marcar
                T = obtener_subconjunto_sin_marcar(visitados);
                // Marcar al subconjunto_actual
                visitados.insert(T);
                // Revisar si es el estado inicial
                if ( T.count(afn.obtener_inicial()) == 1 )
                {
                    afd.establecer_inicial( etiqueta_de_subconjunto[T] );
                }
                // Revisar si es estado final
                for ( auto estado : T )
                {
                    if ( estados_finales_afn.count( estado ) == 1 )
                    {
                        afd.establecer_final( etiqueta_de_subconjunto[T] );
                    }
                }

                for ( char simbolo = 'a' ; simbolo <= 'z' ; simbolo++ )
                {
                    U = mover(T, simbolo);
                    U = e_cerradura(U);
                    if ( U.size() == 0 ) continue;
                    if ( Destados.count(U) == 0 )
                    {
                        Destados.insert(U);
                        etiqueta_de_subconjunto[U] = etiqueta++;
                    }
                    Dtran[{etiqueta_de_subconjunto[T],simbolo}].insert( etiqueta_de_subconjunto[U] );
                }
            }
            afd.asignar_transiciones( Dtran );
            return afd;
            /**/
        }
};
