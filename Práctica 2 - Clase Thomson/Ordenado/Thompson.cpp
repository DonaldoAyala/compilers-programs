#include <map>
#include <vector>
#include <utility>
#include <stack>
#include "AutomataFinito.cpp"

using namespace std;

class Arbol {
    private:
        char simbolo;
        Arbol *hijo_izquierdo;
        Arbol *hijo_derecho;
    public:
        Arbol (char _simbolo)
        {
            this->simbolo = _simbolo;
        }
        Arbol ()
        {
            simbolo = '0';
            hijo_izquierdo = NULL;
            hijo_derecho = NULL;
        }

        Arbol* obtener_hijo_izquierdo ()
        {
            return hijo_izquierdo;
        }
        void asignar_hijo_izquierdo (Arbol *hijo)
        {
            hijo_izquierdo = hijo;
        }
        

        Arbol* obtener_hijo_derecho ()
        {
            return hijo_derecho;
        }
        void asignar_hijo_derecho (Arbol *hijo)
        {
            hijo_derecho = hijo;
        }

        char obtener_simbolo ()
        {
            return simbolo;
        }
        void asignar_simbolo (char _simbolo)
        {
            simbolo = _simbolo;
        }
};

class Thompson
{
    private:
        Arbol *arbol_de_expresion;
        map<pair<int, char>,unordered_set<int>> transiciones;

    public:
        AutomataFinitoNoDeterminista
            convertir ( string expresion_regular )
            {
                string postfijo;
                expresion_regular = preprocesar_infijo(expresion_regular);
                inorden_a_postorden(expresion_regular, postfijo);
                Arbol *auxiliar;
                arbol_de_expresion = generar_arbol_de_expresion(
                    auxiliar, postfijo
                );
                AutomataFinitoNoDeterminista afn;
                int cont_estados = 1;
                pair< pair<int,int>, vector<pair<int,char>> >
                transicion_inicial 
                    = generar_transiciones(arbol_de_expresion, cont_estados);
                afn.establecer_inicial(transicion_inicial.first.first);
                afn.establecer_final(transicion_inicial.first.second);
                afn.asignar_transiciones( transiciones );
                return afn;
            }
    private:
        int prioridad ( char simbolo )
        {
            switch (simbolo)
            {
                case '*':
                    return 3;
                case '+':
                    return 3;
                case '.':
                    return 2;
                case '|':
                    return 1;
                default:
                    return 0;
            }
        }
        Arbol* generar_arbol_de_expresion (Arbol* arbol, string postfijo)
        {
            int i_postfijo = 0;
            char simbolo = '0';
            stack<Arbol*> pila;
            Arbol* tmp_arbol1;
            Arbol* tmp_arbol2;
            while ( i_postfijo < postfijo.size() )
            {
                
                simbolo = postfijo[i_postfijo];
                if ( !(simbolo == '+' 
                    || simbolo == '|' 
                    || simbolo == '*'
                    || simbolo == '.' ))
                {
                    arbol = new Arbol(simbolo);
                    pila.push(arbol);
                }
                else
                {
                    if ( simbolo == '*' )
                    {
                        /*
                            Para 'n*' se crea el sig. nodo
                                *
                            / 
                            n   
                        */
                        arbol = new Arbol (simbolo);
                        tmp_arbol1 = pila.top(); pila.pop();
                        arbol->asignar_hijo_izquierdo( tmp_arbol1 );
                    }
                    else if ( simbolo == '+' )
                    {
                        /*
                            Para 'n+' se crea el sig. Arbol
                                .
                            / \
                            n   *
                                /
                                n
                        */
                        arbol = new Arbol('.');
                        tmp_arbol1 = pila.top(); pila.pop();
                        arbol->asignar_hijo_izquierdo(tmp_arbol1);
                        arbol->asignar_hijo_derecho(new Arbol('*'));
                        tmp_arbol2 = arbol->obtener_hijo_derecho();
                        tmp_arbol2 -> asignar_hijo_izquierdo( tmp_arbol1 );
                    }
                    else
                    {
                        /*
                            Para operadores binarios se crea el sig. Arbol
                                |    |    . 
                               / \   |   / \  
                              n   m  |  n   m
                        */
                        arbol = new Arbol(simbolo);
                        tmp_arbol1 = pila.top(); pila.pop();
                        tmp_arbol2 = pila.top(); pila.pop();
                        arbol->asignar_hijo_derecho(tmp_arbol1);
                        arbol->asignar_hijo_izquierdo(tmp_arbol2);
                    }
                    pila.push(arbol);
                }
                i_postfijo++;
            }
            return arbol;
        }
        void inorden_a_postorden (string infijo, string &postfijo)
        {
            int i_infijo = 0;
            int i_postfijo = 0;
            stack<char> pila;
            char simbolo;
            while (i_infijo < infijo.size())
            {
                simbolo = infijo[i_infijo];
                if (simbolo == '(')
                {
                    pila.push(simbolo);
                    i_infijo++;
                    continue;
                }
                if ( simbolo == ')' )
                {
                    while ( !pila.empty() && pila.top() != '(' )
                    {
                        postfijo.push_back(pila.top());
                        pila.pop();
                    }
                    if ( !pila.empty() )
                    { // Pop al caracter '('
                        pila.pop();
                    }
                    i_infijo++;
                    continue;
                }

                if ( prioridad (simbolo) == 0 ) // es operando
                {
                    postfijo.push_back( simbolo );
                }
                else
                {
                    if ( pila.empty() )
                    {
                        pila.push(simbolo);
                    }
                    else
                    {
                        while ( !pila.empty() && pila.top() != '(' 
                            && prioridad(simbolo) <= prioridad(pila.top()) )
                        {
                            postfijo.push_back(pila.top());
                            pila.pop();
                        }
                        pila.push(simbolo);
                    }
                }
                i_infijo++;
            }

            while ( !pila.empty() )
            {
                postfijo.push_back(pila.top());
                pila.pop();
            }
        }
        bool es_caracter (const char &simbolo)
        {
            return simbolo >= 'a' && simbolo <= 'z';
        }

        string preprocesar_infijo ( const string &infijo )
        {
            string nuevo_infijo = {infijo[0]};
            for ( int i = 1 ; i < infijo.size() ; i++ )
            {
                if ( es_caracter(infijo[i]) && es_caracter(infijo[i - 1])
                    || es_caracter(infijo[i]) && infijo[i - 1] == ')'
                    || es_caracter(infijo[i]) && infijo[i - 1] == '*'
                    || es_caracter(infijo[i]) && infijo[i - 1] == '+'
                    || infijo[i] == '(' && infijo[i - 1] == ')' )
                {
                    nuevo_infijo.push_back('.');
                }
                nuevo_infijo.push_back(infijo[i]);
            }
            return nuevo_infijo;
        }
        pair< pair<int,int>, vector<pair<int,char>> >
        generar_transiciones ( Arbol* raiz, int &fin )
        {
            if ( raiz == NULL ) return { {-1,-1}, {} };
            // Es una hoja
            if ( raiz->obtener_hijo_izquierdo() == nullptr 
                && raiz->obtener_hijo_derecho() == nullptr ) 
            {
                // Crea estados para inicio y fin
                int inicio = fin++, destino = fin++;
                // Agrega transición
                transiciones[{inicio, raiz->obtener_simbolo()}].insert(destino);
                // return inicio y transiciones finales
                return  {{inicio,destino}, { {inicio, raiz->obtener_simbolo()} }};
            }
            if ( raiz->obtener_simbolo() == '|' )
            {
                // Inicio y final de toda la plantilla
                int inicio = fin++, final_or = fin++;
                pair< pair<int,int>, vector<pair<int,char>> > izq, der;
                // Obtiene pimero y ultimos (transiciones) de sus hijos
                izq = generar_transiciones ( raiz->obtener_hijo_izquierdo(), fin );
                der = generar_transiciones ( raiz->obtener_hijo_derecho(), fin );
                
                // Obten el inicio del hijo izquierdo
                int inicio_arriba = izq.first.first;
                // obten el fin del hijo izquierdo
                int fin_arriba = izq.first.second;
                
                // transición de nuevo estado a inicio hijo izquierdo
                transiciones[ {inicio,'E'} ].insert(inicio_arriba);
                // transición de fin de hijo izquierdo a fin de la transicion
                transiciones[ {fin_arriba,'E'} ].insert(final_or);

                // Obten inicio y final del hijo derecho
                int inicio_abajo = der.first.first;
                int fin_abajo = der.first.second;

                // agrega las transiciones de inicio transicion a inicio 
                // hijo derecho
                transiciones[ {inicio,'E'} ].insert(inicio_abajo);
                transiciones[ {fin_abajo,'E'} ].insert(final_or);

                // regresa el inicio y las transiciones al final
                return { {inicio,final_or}, {{ fin_abajo, 'E' },{fin_arriba, 'E'}} };
            }
            if ( raiz->obtener_simbolo() == '.' )
            {
                pair< pair<int,int>, vector<pair<int,char>> > izq, der;
                // Obtiene pimero y ultimos (transiciones) de sus hijos
                izq = generar_transiciones ( raiz->obtener_hijo_izquierdo(), fin );
                der = generar_transiciones ( raiz->obtener_hijo_derecho(), fin );

                // Obten inicio y final de la transicion
                // Inicio tiene que ser igual al inicio izquierda
                // Final debe ser el final de la derecha
                int inicio = izq.first.first;
                int fin = der.first.second;
                
                // A las transiciones finales del hijo izquierdo
                // ponles el inicio del derecho
                int inicio_derecha = der.first.first;
                int final_izquierdo = izq.first.second;
                for ( auto tran_a_fin : izq.second )
                {
                    // elimina la transición a final del hijo izquierdo
                    transiciones[tran_a_fin].erase(final_izquierdo);
                    transiciones[tran_a_fin].insert( inicio_derecha );
                }
                // regresa el inicio y las transiciones al final
                return { {inicio, der.first.second}, {der.second[0]} };
            }
            if ( raiz->obtener_simbolo() == '*' )
            {
                // map (estado, char) -> set

                // Obtén nuevos estados para el inicio y fin de la plantilla
                int inicio = fin++, final_estrella = fin++;

                // Necesitas inicio y fin de tu hijo (Solo hay hijo izquierdo para estrella)
                pair< pair<int,int>, vector<pair<int,char>> > izq;
                // Obtiene pimero y ultimos (transiciones) de sus hijos
                izq = generar_transiciones ( raiz->obtener_hijo_izquierdo(), fin );

                // Obteniendo primero y ultimo del hijo
                int inicio_hijo = izq.first.first, final_hijo = izq.first.second;

                // Añade las transiciones aumentando fin
                transiciones[{inicio, 'E'}].insert( inicio_hijo );
                transiciones[{final_hijo, 'E'}].insert( final_estrella );
                transiciones[{inicio, 'E'}].insert( final_estrella );
                transiciones[{final_hijo, 'E'}].insert( inicio_hijo );
                // regresa el inicio y las transiciones al final
                return { {inicio, final_estrella}, {{inicio, 'E'},{final_hijo, 'E'}} };
            }
        }
};

