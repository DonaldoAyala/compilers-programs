#include <utility>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include <string.h>


#define REDUCCION 0
#define DESPLAZAMIENTO 1
#define MOVER 2
#define ACEPTAR 3

using namespace std;

class AlgoritmoSubconjuntosLR0 {
private:
	// {conjunto, simbolo} -> {accion, conjunto}
	vector<pair<pair<int,char>, pair<int,int>>> acciones;
	set<set<ElementoLR0>> conjuntos;
	map<set<ElementoLR0>, int> numero_de_conjunto;
	int numero_actual;

public:
	void ejecutar_algoritmo (Gramatica g, map<Produccion, int> producciones_numeradas) {
		numero_actual = 1;
		// extendemos la gramática
		string nuevo_cuerpo = {g.obtener_simbolo_inicial()};
		g.agregar_produccion( 'Z', nuevo_cuerpo );
		g.establecer_inicial('Z');
		// obtenemos producciones del nuevo inicial 'Z'
		vector<Produccion> inicial = g.obtener_producciones_con_cabeza ( g.obtener_simbolo_inicial() );

		queue<set<ElementoLR0>> pendientes;
		// obtenemos cerradura la nueva produccion Z->antiguo_simbolo_inicial
		set<ElementoLR0> actual;
		actual.insert( ElementoLR0(inicial[0]) );
		actual = cerradura( actual, g );
		pendientes.push(actual);
		conjuntos.insert(actual);
		numero_de_conjunto[actual] = numero_actual++;

		while ( !pendientes.empty() ) {
			actual = pendientes.front();
			pendientes.pop();

			// Buscamos un elementolr0 para avanzar
			for ( auto elemento : actual ) {
				// Encontramos un elemento lr0 para avanzar
				if ( !elemento.tiene_punto_al_final () ) {
					// Obtenemos el símbolo con el que se va a avanzar
					char simbolo_siguiente = elemento.obtener_siguiente_simbolo();
					// Juntamos todos los elementos se mueven con ese símbolo
					set<ElementoLR0> a_mover = mover( actual, simbolo_siguiente );
					// Aplicamos cerradura
					set<ElementoLR0> nuevo_conjunto = cerradura( a_mover, g );
					// Agregamos el conjunto al conjunto de conjuntos si no está
					if ( conjuntos.count(nuevo_conjunto) == 0 ) {
						conjuntos.insert(nuevo_conjunto);
						numero_de_conjunto[nuevo_conjunto] = numero_actual++;
						// Metemos el nuevo conjunto a la pila para moverlo después
						pendientes.push(nuevo_conjunto);
					}
					// Agregamos acciones de mover y desplazamiento según corresponda
					if ( g.tiene_no_terminal(simbolo_siguiente) ) {
						acciones.push_back( { 
							{numero_de_conjunto[actual], simbolo_siguiente},
							{MOVER, numero_de_conjunto[nuevo_conjunto]}
						} );
					} else {
						acciones.push_back( { 
							{numero_de_conjunto[actual], simbolo_siguiente},
							{DESPLAZAMIENTO, numero_de_conjunto[nuevo_conjunto]}
						} );
					}
					// Buscamos acciones de aceptar o reducción
					for ( auto elementolr0 : nuevo_conjunto ) {
						if ( elementolr0.tiene_punto_al_final() ) {
							// Es la acción de aceptar
							if ( elementolr0.obtener_produccion().obtener_cabeza() == g.obtener_simbolo_inicial() ) {
								acciones.push_back( { 
									{numero_de_conjunto[nuevo_conjunto], '$'},
									{ACEPTAR, numero_de_conjunto[nuevo_conjunto]}
								} );
							} else { // Es la acción de reducción
								Produccion produccion_punto_final = elementolr0.obtener_produccion();
								char cabeza = produccion_punto_final.obtener_cabeza();
								set<char> siguiente = CalculadorSiguiente :: siguiente( cabeza, g );
								// Agregamos las acciones
								for ( auto simbolo : siguiente ) {
									acciones.push_back( {
										{numero_de_conjunto[nuevo_conjunto], simbolo},
										{REDUCCION, producciones_numeradas[produccion_punto_final]}
									} );
								}
							}
						}
					}
				}
			}
		}
	}

	// void es provisional hasta encontrar que tipo regresar
	vector<pair<pair<int,char>, string>> obtener_acciones () {
		vector<pair<pair<int,char>, string>> acciones_traducidas;
		for ( auto accion : acciones ) {
			string accion_str;
			switch (accion.second.first) {
				case MOVER:
					accion_str = to_string(accion.second.second);
				break;
				case DESPLAZAMIENTO:
					accion_str = "d"+to_string(accion.second.second);
				break;
				case REDUCCION:
					accion_str = "r"+to_string(accion.second.second);
				break;
				case ACEPTAR:
					accion_str = "OK";
				break;
			}
			acciones_traducidas.push_back({
					{accion.first.first, accion.first.second},
					accion_str
				});
		}
		return acciones_traducidas;
	}

	set<ElementoLR0> cerradura (set<ElementoLR0> c, Gramatica g) {
		set<ElementoLR0> conjunto_cerradura;
		// Obtenemos los elementos del conjunto al que se le aplicará la cerradura
		for ( auto x : c ) {
			conjunto_cerradura.insert( x );
		}
		int inicial = -1, final = conjunto_cerradura.size();
		// Mientras haya cambios en el conjunto (aumente el tamaño del conjunto)
		while ( inicial != final ) {
 			inicial = conjunto_cerradura.size();
			// conjunto auxiliar contiene los elementos que se agregan en la iteracion del for
			set<ElementoLR0> conjunto_auxiliar;
			// iteramos sobre los elementos actuales del conjunto
			for ( auto elemento : conjunto_cerradura ) {
				// El elemento se puede mover
				if ( !elemento.tiene_punto_al_final() ) {
					char siguiente_simbolo = elemento.obtener_siguiente_simbolo();
					// el siguiente simbolo es un no terminal
					if ( g.tiene_no_terminal(siguiente_simbolo) ) {
						// obtenemos las producciones para avanzar
						vector<Produccion> a_agregar = g.obtener_producciones_con_cabeza(siguiente_simbolo);
						// Agregamos los elementos lr0 a el conjunto_cerradura
						for ( auto produccion : a_agregar ) {
							conjunto_auxiliar.insert(ElementoLR0(produccion));
						}
					}
				}
			}
			for ( auto x : conjunto_auxiliar ) {
				conjunto_cerradura.insert(x);
			}
			final = conjunto_cerradura.size();
		}
		return conjunto_cerradura;
	}

	set<ElementoLR0> mover (set<ElementoLR0> c, char simbolo) {
		set<ElementoLR0> conjunto_mover;
		for ( auto elemento_avanzar : c ) {
			if ( elemento_avanzar.obtener_siguiente_simbolo() == simbolo ) {
				// Movemos el elemento y lo insertamos al conjunto
				elemento_avanzar.avanzar_punto();
				conjunto_mover.insert( elemento_avanzar );
			}
		}
		return conjunto_mover;
	}

	int obtener_numero_subconjuntos () {
		return conjuntos.size();
	}
};