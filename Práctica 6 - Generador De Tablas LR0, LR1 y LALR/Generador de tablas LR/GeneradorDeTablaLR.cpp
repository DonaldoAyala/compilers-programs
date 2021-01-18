#include <iostream>
#include <utility>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>

#define REDUCCION 0
#define DESPLAZAMIENTO -1
#define MOVER -2
#define ACEPTAR -3

using namespace std;

class Produccion {
private:
	char cabeza;
	string cuerpo;

public:
	Produccion ( char cabeza_, string cuerpo_ ) {
		cabeza = cabeza_;
		cuerpo = cuerpo_;
	}

	bool contiene_simbolo ( char simbolo ) {
		for ( int i = 0 ; i < cuerpo.size() ; i++ ) {
			if ( cuerpo[i] == simbolo ) return true;
		}
		return false;
	}

	char obtener_cabeza () const {
		return cabeza;
	}

	string obtener_cuerpo() const {
		return cuerpo;
	}

	bool operator<(Produccion const& derecha) const {
		if ( cabeza < derecha.cabeza ) {
			return true;
		} else if ( cabeza > derecha.cabeza ) {
			return false;
		} else {
			return cuerpo < derecha.cuerpo;
 		}
	}
};

class ElementoLR1 {

private:
	Produccion produccion;
	int posicion_punto;
	char simbolo_anticipacion;

public:
	ElementoLR1(Produccion p, char simbolo) : produccion(p) {
		if ( p.obtener_cuerpo().size() == 1 && p.obtener_cuerpo()[0] == '3' ) {
			posicion_punto = 1;
		} else {
			posicion_punto = 0;
		}
		simbolo_anticipacion = simbolo;
	}
	
	char obtener_siguiente_simbolo () {
		string cuerpo = produccion.obtener_cuerpo();
		if ( posicion_punto < cuerpo.size() )
			return cuerpo[posicion_punto];
		else
			return '%';
	}

	void avanzar_punto () {
		posicion_punto++;
	}

	bool tiene_punto_al_final () {
		string cuerpo = produccion.obtener_cuerpo();
		return posicion_punto == cuerpo.size();
	}

	Produccion obtener_produccion () {
		return produccion;
	}

	int obtener_posicion_punto () {
		return posicion_punto;
	}

	char obtener_simbolo_anticipacion () {
		return simbolo_anticipacion;
	}

	string obtener_beta_mas_simbolo_anticipacion () {
		string cuerpo = produccion.obtener_cuerpo();
		string beta = "";
		if ( posicion_punto < cuerpo.size() - 1 ) {
			beta = cuerpo.substr(posicion_punto + 1, cuerpo.size() - posicion_punto);
		}
		beta.push_back(simbolo_anticipacion);
		return beta;
	}

	bool operator<(ElementoLR1 const& derecha) const {
		if ( produccion < derecha.produccion ) {
			return true;
		} else if ( derecha.produccion < produccion ) {
			return false;
		} else {
			if ( posicion_punto < derecha.posicion_punto ) {
				return true;
			} else if ( derecha.posicion_punto < posicion_punto ) {
				return false;
			} else {
				return simbolo_anticipacion < derecha.simbolo_anticipacion;
			}
		}
	}
};

class ElementoLR0 {
private:
	Produccion produccion;
	int posicion_punto;

public:
	ElementoLR0(Produccion p) : produccion(p) {
		if ( produccion.obtener_cuerpo().size() == 1 && produccion.obtener_cuerpo()[0] == '3' ) {
			posicion_punto = 1;
		} else {
			posicion_punto = 0;
		}
	}
	
	char obtener_siguiente_simbolo () {
		string cuerpo = produccion.obtener_cuerpo();
		if ( posicion_punto < cuerpo.size() )
			return cuerpo[posicion_punto];
		else
			return '%';
	}
	void avanzar_punto () {
		posicion_punto++;
	}

	bool tiene_punto_al_final () {
		string cuerpo = produccion.obtener_cuerpo();
		return posicion_punto == cuerpo.size();
	}

	Produccion obtener_produccion () {
		return produccion;
	}

	int obtener_posicion_punto () {
		return posicion_punto;
	}

	bool operator<(ElementoLR0 const& derecha) const {
		if ( produccion < derecha.produccion ) {
			return true;
		} else if ( derecha.produccion < produccion ) {
			return false;
		} else {
			return posicion_punto < derecha.posicion_punto;
		}
	}
};

class ConjuntoElementosLR1 {

private:
	set<ElementoLR1> elementos;
	set<ElementoLR1> corazon;

public:
	void insertar_elemento ( ElementoLR1 elemento ) {
		elementos.insert(elemento);
	}

	void insertar_corazon ( ElementoLR1 elemento ) {
		corazon.insert(elemento);
		elementos.insert(elemento);
	}

	bool tiene_elemento ( ElementoLR1 elemento ) {
		return elementos.count(elemento);
	}

	int obtener_tamanio () {
		return elementos.size();
	}

	set<ElementoLR1> obtener_elementos() {
		return elementos;
	}

	set<ElementoLR1> obtener_corazon() {
		return elementos;
	}

	bool operator<(ConjuntoElementosLR1 const& derecha) const {
		if ( corazon < derecha.corazon ) {
			return true;
		} else if ( derecha.corazon < corazon ) {
			return false;
		} else { 
			return elementos < derecha.elementos;
		}
	}

	bool mismo_corazon ( ConjuntoElementosLR1 der ) {
		for ( auto l : corazon ) {
			Produccion produccion_izq = l.obtener_produccion();
			int punto_izq = l.obtener_posicion_punto();
			for ( auto r : der.corazon ) {
				Produccion produccion_der = r.obtener_produccion();
				int punto_der = r.obtener_posicion_punto();
				if ( produccion_izq < produccion_der || produccion_der < produccion_izq ) {
					return false;
				}
				if ( punto_izq != punto_der ) return false;
			}
		}
		return true;
	}

	void fusionar_con ( ConjuntoElementosLR1 c ) {
		for ( auto e : c.corazon ) {
			corazon.insert(e);
		}
		for ( auto e : c.elementos ) {
			elementos.insert(e);
		}
	}
};

class Gramatica {
private:
	char simbolo_inicial; // Debe ser letra mayúscula
	set<char> simbolos_terminales; // son letras minúsculas
	set<char> simbolos_no_terminales; // son letras mayúsculas
	vector<Produccion> producciones;

public:
	bool tiene_terminal(char simbolo) {
		if ( simbolos_terminales.count(simbolo) )
			return true;
		return false;
	}

	bool tiene_no_terminal(char simbolo) {
		if ( simbolos_no_terminales.count(simbolo) )
			return true;
		return false;
	}

	char obtener_simbolo_inicial () {
		return simbolo_inicial;
	}

	vector<Produccion> obtener_producciones_con_cabeza ( char no_terminal ) {
		vector<Produccion> producciones_con_cabeza;
		for ( int i = 0 ; i < producciones.size() ; i++ ) {
			if ( producciones[i].obtener_cabeza() == no_terminal ) {
				producciones_con_cabeza.push_back( producciones[i] );
			}
		}
		return producciones_con_cabeza;
	}

	vector<Produccion> obtener_producciones_con_no_terminal_en_cuerpo ( char no_terminal ) {
		vector<Produccion> producciones_con_no_terminal_en_cuerpo;
		for ( int i = 0 ; i < producciones.size() ; i++ ) {
			for ( auto simbolo : producciones[i].obtener_cuerpo() ) {
				if ( simbolo == no_terminal ) {
					producciones_con_no_terminal_en_cuerpo.push_back( producciones[i] );
				}
			}
		}
		return producciones_con_no_terminal_en_cuerpo;
	}

	void establecer_inicial ( char simbolo ) {

		if ( simbolo < 65 || simbolo > 90 )
			throw "No Terminal no válido: los no terminales son representados por mayúsculas\n";

		simbolo_inicial = simbolo;
		agregar_no_terminal( simbolo );
	}

	void agregar_terminal( char simbolo ) {
		if ( simbolo < 97 || simbolo > 122 )
			throw "Terminal no válido: los terminales son representados por minúsculas\n";
		simbolos_terminales.insert( simbolo );
	}

	void agregar_no_terminal( char simbolo ) {
		if ( simbolo < 65 || simbolo > 90 )
			throw "No Terminal no válido: los no terminales son representados por mayúsculas\n";
		simbolos_no_terminales.insert( simbolo );

	}

	void agregar_produccion( char cabeza, string cuerpo ) {
		if ( cabeza < 65 || cabeza > 90 )
			throw "Cabeza de producción no válida: La cabeza debe ser un no terminal, es decir, una mayúscula\n";
		agregar_no_terminal(cabeza);
		for ( int i = 0 ; i < cuerpo.size() ; i++ ) {
			if ( cuerpo[i] >= 65 && cuerpo[i] <= 90 ) { // es mayúscula <-> no terminal
				agregar_no_terminal( cuerpo[i] );
			} else if ( cuerpo[i] >= 97 && cuerpo[i] <= 122 ) { // es minúscula <-> terminal
				agregar_terminal( cuerpo[i] );	
			} else if ( cuerpo[i] == '3' ){
				continue; // épsilon no se agrega a los no terminales ni a los terminales
			} else {
				throw "Simbolo no válido: los símbolos válidos pertenecen a {A,B,...,Z,a,b,...z,3(épsilon)}\n";
			}

		}
		producciones.push_back( Produccion(cabeza, cuerpo) );
	}

	vector<Produccion> obtener_producciones () {
		return producciones;
	}

	vector<char> obtener_no_terminales () {
		vector<char> no_terminales;
		for ( auto x : simbolos_no_terminales ) {
			no_terminales.push_back(x);
		}
		return no_terminales;
	}

	vector<char> obtener_terminales () {
		vector<char> terminales;
		for ( auto x : simbolos_terminales ) {
			terminales.push_back(x);
		}
		return terminales;
	}
};

class CalculadorPrimero {
private:
	set<char> calculando_simbolo;
	set<string> calculando_cadena;

	set<char> primero_recursivo ( char a, Gramatica g ) {
		set<char> simbolos_en_primero;
		// Aún no hemos calculado el simbolo a
		if ( calculando_simbolo.count(a) == 0 ) {
			calculando_simbolo.insert(a);
		} else {
			return simbolos_en_primero;
		}
		if ( !g.tiene_no_terminal(a) ) { // 'a' Es un terminal o epsilon
			simbolos_en_primero.insert( a );
		} else { // 'a' es no terminal
			vector<Produccion> producciones = g.obtener_producciones_con_cabeza(a);
			for ( auto produccion : producciones ) {
				if ( produccion.obtener_cuerpo() == "3" ) {
					simbolos_en_primero.insert( '3' );
				} else {
					set<char> simbolos = primero_recursivo ( produccion.obtener_cuerpo(), g );
					simbolos_en_primero.insert( simbolos.begin(), simbolos.end() );
				}
			}
		}
		return simbolos_en_primero;
	}

	set<char> primero_recursivo ( string s, Gramatica g ) {
		set<char> simbolos_en_primero;
		if ( calculando_cadena.count(s) == 0 ) {
			calculando_cadena.insert( s );
		} else {
			return simbolos_en_primero;
		}
		for ( int i = 0 ; i < s.size() ; i++ ) {
			set<char> simbolos = primero_recursivo( s[i] , g );

			if ( simbolos.count('3') ) {
				simbolos.erase( '3' );
				simbolos_en_primero.insert( simbolos.begin(), simbolos.end() );
			} else {
				simbolos_en_primero.insert( simbolos.begin(), simbolos.end() );
				break;
			}

			if ( i == s.size() - 1 ) {
				simbolos_en_primero.insert( '3' );
			}
		}
		return simbolos_en_primero;
	}
public:
	set<char> primero( char a, Gramatica g ) {
		calculando_simbolo.clear();
		calculando_cadena.clear();
		set<char> simbolos_en_primero = primero_recursivo(a, g);
		return simbolos_en_primero;
	}

	set<char> primero( string s, Gramatica g ) {
		calculando_simbolo.clear();
		calculando_cadena.clear();
		set<char> simbolos_en_primero = primero_recursivo(s, g);
		return simbolos_en_primero;
	}
};

class CalculadorSiguiente {
private:
	static set<char> siguiente_recursiva (char c, Gramatica g, set<char> calculando ) {
		set<char> simbolos_en_siguiente;
		if ( g.obtener_simbolo_inicial() == c ) { // Regla 1
			simbolos_en_siguiente.insert( '$' );
		}
		vector<Produccion> producciones = g.obtener_producciones_con_no_terminal_en_cuerpo( c );
		for ( auto produccion : producciones ) {
			string cuerpo = produccion.obtener_cuerpo();
			set<char> simbolos;
			if ( cuerpo[ cuerpo.size() - 1 ] == c ) { // Es del tipo A -> alfa X
				if ( calculando.count( c ) == 0 ) {
					calculando.insert( c );
					simbolos = siguiente_recursiva( produccion.obtener_cabeza(), g , calculando); // Siguiente(A)
					simbolos_en_siguiente.insert( simbolos.begin(), simbolos.end() );
				}
			} else { // Es del tipo A -> alfa X beta
				for ( int i = 0 ; i < cuerpo.size() ; i++ ) { // Recorremos simbolos de beta
					if ( cuerpo[i] == c ) { // si se encuentra un simbolo X
						string beta = cuerpo.substr(i + 1, cuerpo.size() - i); // extraemos beta
						CalculadorPrimero p;
						simbolos = p.primero( beta , g);
						if ( simbolos.count('3') ) { // epsilon está en Primero(beta)
							if ( calculando.count( c ) == 0 ) {
								calculando.insert( c );
								simbolos = siguiente_recursiva(produccion.obtener_cabeza(), g, calculando);
							}
						}
						simbolos_en_siguiente.insert( simbolos.begin(), simbolos.end() );
					}
				} // termina for
			} //termina else
		} // termina for
		return simbolos_en_siguiente;
	}

public:
	static set<char> siguiente (char c, Gramatica g) {
		set<char> simbolos_en_siguiente, calculando;
		simbolos_en_siguiente = siguiente_recursiva( c, g, calculando );
		return simbolos_en_siguiente;
	}
};

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
	vector<pair<pair<int,char>, string>> obtener_acciones_traducidas () {
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

class AlgoritmoSubconjuntosLR1 {

private:
	// {{conjunto, simbolo} -> {accion, conjunto}}
	vector<pair<pair<int,char>, pair<int,int>>> acciones;
	set<ConjuntoElementosLR1> conjuntos;
	map<ConjuntoElementosLR1, int> numero_de_conjunto;
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
		queue<ConjuntoElementosLR1> por_visitar;
		// obtenemos cerradura la nueva produccion Z->antiguo_simbolo_inicial
		ConjuntoElementosLR1 actual;
		actual.insertar_corazon( ElementoLR1(inicial[0], '$') );
		actual = cerradura( actual, g );
		por_visitar.push(actual);
		conjuntos.insert(actual);
		numero_de_conjunto[actual] = numero_actual++;

		// mientras tengamos conjuntos sin haber analizado
		while ( !por_visitar.empty() ) {
			actual = por_visitar.front();
			por_visitar.pop();
			// Buscamos un elementolr1 para avanzar
			for ( auto elemento : actual.obtener_elementos() ) {
				// Encontramos un elemento lr1 para avanzar
				if ( !elemento.tiene_punto_al_final () ) {
					// Obtenemos el símbolo con el que se va a avanzar
					char simbolo_siguiente = elemento.obtener_siguiente_simbolo();
					// Obtenemos el conjunto ir_a
					ConjuntoElementosLR1 nuevo_conjunto = ir_a( actual, simbolo_siguiente, g );
					// Agregamos el conjunto al conjunto de conjuntos si no está
					if ( conjuntos.count(nuevo_conjunto) == 0 ) {
						conjuntos.insert(nuevo_conjunto);
						// Asignamos un número al nuevo conjunto
						numero_de_conjunto[nuevo_conjunto] = numero_actual++;
						// Metemos el nuevo conjunto a la pila para analizarlo después
						por_visitar.push(nuevo_conjunto);
					}
					
					// Guardamos acciones de mover y desplazamiento según corresponda
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
					for ( auto elementolr0 : nuevo_conjunto.obtener_elementos() ) {
						if ( elementolr0.tiene_punto_al_final() ) {
							// Es la acción de aceptar
							if ( elementolr0.obtener_produccion().obtener_cabeza() == g.obtener_simbolo_inicial() ) {
								acciones.push_back( { 
									{numero_de_conjunto[nuevo_conjunto], '$'},
									{ACEPTAR, numero_de_conjunto[nuevo_conjunto]}
								} );
							// Es la acción de reducción
							} else {
								Produccion produccion_punto_final = elementolr0.obtener_produccion();
								char simbolo_anticipacion = elementolr0.obtener_simbolo_anticipacion();
								acciones.push_back( {
									{numero_de_conjunto[nuevo_conjunto], simbolo_anticipacion},
									{REDUCCION, producciones_numeradas[produccion_punto_final]}
								} ); 
							}

						}
					}
				}
			}
		}
	}

	vector<pair<pair<int,char>, pair<int,int>>> obtener_acciones () {
		return acciones;
	}
	
	map<ConjuntoElementosLR1, int> obtener_numero_de_conjunto () {
		return numero_de_conjunto;
	}

	set<ConjuntoElementosLR1> obtener_conjuntos () {
		return conjuntos;
	}

	vector<pair<pair<int,char>, string>> obtener_acciones_traducidas () {
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

	ConjuntoElementosLR1 cerradura (ConjuntoElementosLR1 c, Gramatica g) {
		ConjuntoElementosLR1 conjunto_cerradura;
		// Obtenemos los elementos del conjunto al que se le aplicará la cerradura
		conjunto_cerradura = c;
		int inicial = -1, final = conjunto_cerradura.obtener_tamanio();
		// Mientras haya cambios en el conjunto (aumente el tamaño del conjunto)
		while ( inicial != final ) {
 			inicial = conjunto_cerradura.obtener_tamanio();
			// conjunto auxiliar contiene los elementos que se agregan en la iteracion del for
			set<ElementoLR1> conjunto_auxiliar;
			// iteramos sobre los elementos actuales del conjunto
			for ( auto elemento : conjunto_cerradura.obtener_corazon() ) {
				// El elemento se puede avanzar
				if ( !elemento.tiene_punto_al_final() ) {
					char siguiente_simbolo = elemento.obtener_siguiente_simbolo();
					// el siguiente simbolo es un no terminal
					if ( g.tiene_no_terminal(siguiente_simbolo) ) {
						// obtenemos las producciones para avanzar
						vector<Produccion> a_agregar = g.obtener_producciones_con_cabeza(siguiente_simbolo);
						// Agregamos los elementos lr0 a el conjunto_cerradura
						string beta_anticipacion = elemento.obtener_beta_mas_simbolo_anticipacion();
						CalculadorPrimero p;
						set<char> conjunto_primero = p.primero( beta_anticipacion, g );
						for ( auto produccion : a_agregar ) {
							for ( auto simbolo : conjunto_primero ) {
								conjunto_auxiliar.insert(ElementoLR1(produccion, simbolo));
							}
						}
					}
				}
			}
			for ( auto x : conjunto_auxiliar ) {
				conjunto_cerradura.insertar_elemento(x);
			}
			final = conjunto_cerradura.obtener_tamanio();
		}
		return conjunto_cerradura;
	}

	ConjuntoElementosLR1 ir_a (ConjuntoElementosLR1 c, char simbolo, Gramatica g) {
		ConjuntoElementosLR1 conjunto_ir_a;
		for ( auto elemento_avanzar : c.obtener_elementos() ) {
			if ( elemento_avanzar.obtener_siguiente_simbolo() == simbolo ) {
				// Movemos el elemento y lo insertamos al conjunto
				elemento_avanzar.avanzar_punto();
				conjunto_ir_a.insertar_corazon(elemento_avanzar);
			}
		}
		return cerradura(conjunto_ir_a, g);
	}

	int obtener_numero_subconjuntos () {
		return conjuntos.size();
	}
};

class LectorDeGramatica {
private:
    static vector<string> dividir_cadena ( const string cadena, string separador ) {
            string valor = "";
            vector<string> valores;
            for ( int i = 0 ; i < cadena.size()  ; i++ ) {
                if ( cadena.substr(i, separador.size() ) == separador) {
                    if ( valor.size() > 0 ) {
                        valores.push_back(valor);
                        valor = "";
                        i += separador.size() - 1;
                    }
                } else {
                    if ( cadena[i] != 13 )
                        valor.push_back( cadena[i] );
                }
            }
            if ( valor.size() > 0 ) {
                valores.push_back(valor);
            }
            return valores;
        }
public:

    static Gramatica leer_gramatica_de (string ruta)  {
        Gramatica nueva_gramatica;
        fstream archivo (ruta);
        string linea;
        if ( archivo.is_open() ) {
            // Leer simbolo inicial
            getline(archivo, linea);
            nueva_gramatica.establecer_inicial( linea[0] );

            char cabeza;
            string cuerpo;
            vector<string> formato_produccion;
            while ( getline(archivo, linea) ) {
                formato_produccion = dividir_cadena( linea, "->" );
                
                cabeza = formato_produccion[0][0];
                
                cuerpo = formato_produccion[1];

                nueva_gramatica.agregar_produccion( cabeza, cuerpo );
            }
        }
        return nueva_gramatica;
    }
};

class TablaLR {
private:
	char simbolo_inicial_gramatica;
	map<int,map<char,set<string>>> tabla;
	vector<char> simbolos_de_columnas;
	vector<int> simbolos_de_filas;
	map<Produccion, int> producciones_numeradas;

public:
	TablaLR ( vector<char> no_terminales, vector<char> terminales, int conjuntos, map<Produccion, int> producciones, char inicial ) {
		simbolo_inicial_gramatica = inicial;
		simbolos_de_columnas = terminales;
		simbolos_de_columnas.push_back( '$' );
		simbolos_de_columnas.insert( end(simbolos_de_columnas), begin(no_terminales), end(no_terminales) );
		for ( int i = 1 ; i <= conjuntos ; i++ ) {
			simbolos_de_filas.push_back( i );
		}
		producciones_numeradas.insert(producciones.begin(), producciones.end());
	}

	char obtener_simbolo_inicial_gramatica() {
		return simbolo_inicial_gramatica;
	}

	set<string> obtener_entrada(int fila, char columna) {
		return tabla[fila][columna];
	}

	void agregar_entrada( int fila, char columna, string accion ) {
		tabla[fila][columna].insert( accion );
	}

	map<Produccion, int> obtener_producciones_numeradas() {
		return producciones_numeradas;
	}

	vector<char> obtener_simbolos_de_columnas () {
		return simbolos_de_columnas;
	}
	
	vector<int> obtener_simbolos_de_filas () {
		return simbolos_de_filas;
	}
};

class EscritorDeTablaLR {
public:
    void escribir_tabla_en (const string & ruta, TablaLR tabla) {
        ofstream archivo (ruta);
        const string espacio = "    ";
        if ( archivo.is_open() ) {
            vector<char> columnas = tabla.obtener_simbolos_de_columnas();
            vector<int> filas = tabla.obtener_simbolos_de_filas();

            archivo << "Símbolo inicial: " << tabla.obtener_simbolo_inicial_gramatica() << "\n";

            map<Produccion, int> producciones = tabla.obtener_producciones_numeradas();
            for ( auto produccion : producciones) {
                archivo << produccion.first.obtener_cabeza();
                archivo << "->";
                archivo << produccion.first.obtener_cuerpo();
                archivo << "\t(" << produccion.second << ")\n";
            }

            // Se escriben los encabezados de la tabla
            archivo << "\t" ;
            for ( int i = 0 ; i < columnas.size() ; i++ ) {
                archivo << columnas[i] << "\t";
            }
            archivo << "\n";
            // Se escriben las filas
            for ( int i = 0  ; i < filas.size() ; i++ ) {
				// Verificamos si la fila tiene contenido
				bool escribir_fila = false;
				for ( int j = 0 ; j < columnas.size() ; j++ ) {
                    if ( tabla.obtener_entrada( filas[i], columnas[j] ).size() > 0 ) {
						escribir_fila = true;
                    }
                }
				// Si no se escribira nada en la fila, no escribimos la fila
				if ( !escribir_fila ) { continue; }
				// Se escribe el simbolo correspondiente a la fila
                archivo << filas[i] << "\t";

                // Se escribe el contenido de la tabla
                for ( int j = 0 ; j < columnas.size() ; j++ ) {
                    if ( tabla.obtener_entrada( filas[i], columnas[j] ).size() > 0 ) {
                        // Se escriben todos los elementos de cada entrada
                        for ( auto x : tabla.obtener_entrada( filas[i], columnas[j] ) ) {
                            archivo << x << " ";
                        }
                    }
                    archivo << "\t";
                }
                archivo << "\n";
            }
        }
    }
};

class GeneradorDeTablaLR0 {
private:
	map<Produccion, int> enumerar_producciones( Gramatica g ) {
		map<Produccion, int> producciones_numeradas;
		vector<Produccion> producciones = g.obtener_producciones();
		int numeracion = 1;
		for ( auto produccion : producciones ) {
			producciones_numeradas[produccion] = numeracion++;
		}
		return producciones_numeradas;
	}

public:
	TablaLR generar_tablalr0 (string ruta_lectura) {
		Gramatica g = LectorDeGramatica::leer_gramatica_de(ruta_lectura);
		map<Produccion, int> producciones_numeradas = enumerar_producciones(g);
		AlgoritmoSubconjuntosLR0 algoritmo_subconjuntos;
		algoritmo_subconjuntos.ejecutar_algoritmo( g, producciones_numeradas );
		vector<pair<pair<int,char>, string>> acciones = algoritmo_subconjuntos.obtener_acciones_traducidas();
		TablaLR tabla(g.obtener_no_terminales(), g.obtener_terminales(), algoritmo_subconjuntos.obtener_numero_subconjuntos(), producciones_numeradas, g.obtener_simbolo_inicial());
		for ( auto accion : acciones ) {
			tabla.agregar_entrada( accion.first.first, accion.first.second, accion.second );
		}
		return tabla;
	}
};

class GeneradorDeTablaLR1 {
private:
	map<Produccion, int> enumerar_producciones( Gramatica g ) {
		map<Produccion, int> producciones_numeradas;
		vector<Produccion> producciones = g.obtener_producciones();
		int numeracion = 1;
		for ( auto produccion : producciones ) {
			producciones_numeradas[produccion] = numeracion++;
		}
		return producciones_numeradas;
	}

public:
	TablaLR generar_tablalr1 (string ruta_lectura) {
		Gramatica g = LectorDeGramatica::leer_gramatica_de(ruta_lectura);
		map<Produccion, int> producciones_numeradas = enumerar_producciones(g);
		AlgoritmoSubconjuntosLR1 algoritmo_subconjuntos;
		algoritmo_subconjuntos.ejecutar_algoritmo( g, producciones_numeradas );
		vector<pair<pair<int,char>, string>> acciones = algoritmo_subconjuntos.obtener_acciones_traducidas();
		TablaLR tabla(g.obtener_no_terminales(), g.obtener_terminales(), algoritmo_subconjuntos.obtener_numero_subconjuntos(), producciones_numeradas, g.obtener_simbolo_inicial());
		for ( auto accion : acciones ) {
			tabla.agregar_entrada( accion.first.first, accion.first.second, accion.second );
		}
		return tabla;
	}
};

class GeneradorDeTablaLALR {
private:
	int numero_de_subconjuntos_fusionados;

	map<Produccion, int> enumerar_producciones( Gramatica g ) {
		map<Produccion, int> producciones_numeradas;
		vector<Produccion> producciones = g.obtener_producciones();
		int numeracion = 1;
		for ( auto produccion : producciones ) {
			producciones_numeradas[produccion] = numeracion++;
		}
		return producciones_numeradas;
	}

	vector<pair<pair<int,char>, string>> obtener_acciones_lalr ( 
		set<ConjuntoElementosLR1> conjuntos,
		map<ConjuntoElementosLR1, int> numeros_conjuntos,
		vector<pair<pair<int,char>, pair<int,int>>> acciones
	) {
		// Iteramos sobre los conjuntos eligiendo 1
		set<set<ConjuntoElementosLR1>> a_fusionar;
		set<ConjuntoElementosLR1> usados;
		set<ConjuntoElementosLR1> auxiliar;
		for ( auto conjunto_fijo : conjuntos ) {
			auxiliar.clear();
			if ( usados.count(conjunto_fijo) == 0 ) { // No se ha fusionado
				auxiliar.insert(conjunto_fijo);
				usados.insert(conjunto_fijo);
				for ( auto conjunto_iterador : conjuntos ) {
					if ( usados.count(conjunto_iterador) == 0 && conjunto_fijo.mismo_corazon(conjunto_iterador) ) {
						auxiliar.insert( conjunto_iterador );
						usados.insert(conjunto_iterador);
					}
				}
				a_fusionar.insert(auxiliar);
			}
		}

		// Generamos las nuevas acciones
		set<ConjuntoElementosLR1> fusionados;
		map<int,int> nueva_numeracion;
		map<ConjuntoElementosLR1, int> nuevos_conjuntos_numerados;
		for ( auto fusionar : a_fusionar ) {
			ConjuntoElementosLR1 actual;
			int numero_antiguo = -1;
			for ( auto conjunto : fusionar ) {
				actual.fusionar_con( conjunto );
				if ( numero_antiguo == -1 ) {
					numero_antiguo = numeros_conjuntos[conjunto];
				}
				nueva_numeracion[numeros_conjuntos[conjunto]] = numero_antiguo;
			}
			nuevos_conjuntos_numerados[actual] = numero_antiguo;
			fusionados.insert( actual );
		}
		numero_de_subconjuntos_fusionados = fusionados.size();
		set<pair<pair<int,char>, pair<int,int>>> nuevas_acciones;
		for ( auto accion : acciones ) {
			if ( accion.second.first == REDUCCION ) {
				nuevas_acciones.insert(
				 {{
					nueva_numeracion[accion.first.first],accion.first.second
				 },{
					accion.second.first, accion.second.second
				 }} 
				);
			} else {
				nuevas_acciones.insert(
				 {{
					nueva_numeracion[accion.first.first],accion.first.second
				 },{
					accion.second.first, nueva_numeracion[accion.second.second]
				 }} 
				);
			}
			
		}
		// ( conjunto, caracter -> accion, conjunto )
		vector<pair<pair<int,char>, string>> acciones_traducidas;
		for ( auto accion : nuevas_acciones ) {
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

public:
	TablaLR generar_tablalalr (string ruta_lectura) {
		Gramatica g = LectorDeGramatica::leer_gramatica_de(ruta_lectura);
		map<Produccion, int> producciones_numeradas = enumerar_producciones(g);
		// Ejecutamos algoritmo de subconjuntos
		AlgoritmoSubconjuntosLR1 algoritmo_subconjuntos;
		algoritmo_subconjuntos.ejecutar_algoritmo( g, producciones_numeradas );
		// Obtenemos la información del algoritmo de los subconjuntos
		set<ConjuntoElementosLR1> conjuntos = algoritmo_subconjuntos.obtener_conjuntos();
		map<ConjuntoElementosLR1, int> numero_conjunto = algoritmo_subconjuntos.obtener_numero_de_conjunto();
		// ( conjunto, caracter -> accion, conjunto )
		vector<pair<pair<int,char>, pair<int,int>>> acciones = algoritmo_subconjuntos.obtener_acciones();
		vector<pair<pair<int,char>, string>> acciones_traducidas = obtener_acciones_lalr(conjuntos, numero_conjunto, acciones);
		// Escribimos las entradas en la tabla
		TablaLR tabla_lalr(g.obtener_no_terminales(), g.obtener_terminales(), algoritmo_subconjuntos.obtener_numero_subconjuntos(), producciones_numeradas, g.obtener_simbolo_inicial());
		for ( auto accion : acciones_traducidas ) {
			tabla_lalr.agregar_entrada( accion.first.first, accion.first.second, accion.second );
		}
		return tabla_lalr;
	}
};

class GeneradorDeTablaLR {
private:
	GeneradorDeTablaLR0 generador_lr0;
	GeneradorDeTablaLR1 generador_lr1;
	GeneradorDeTablaLALR generador_lalr;
	EscritorDeTablaLR escritor;
public:
	void generar_tablalr0 ( string ruta_lectura, string ruta_escritura ) {
		escritor.escribir_tabla_en(ruta_escritura, generador_lr0.generar_tablalr0(ruta_lectura));
	}

	void generar_tablalr1 ( string ruta_lectura, string ruta_escritura ) {
		escritor.escribir_tabla_en(ruta_escritura, generador_lr1.generar_tablalr1(ruta_lectura));
	}

	void generar_tablalalr ( string ruta_lectura, string ruta_escritura ) {
		escritor.escribir_tabla_en(ruta_escritura, generador_lalr.generar_tablalalr(ruta_lectura));
	}
};

int main () {
	GeneradorDeTablaLR generador_tabla;
	generador_tabla.generar_tablalalr ("Gramatica.txt", "TablaLR.txt");
	return 0;
}