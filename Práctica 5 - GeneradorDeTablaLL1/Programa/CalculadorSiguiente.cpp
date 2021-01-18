#include <set>
#include "Gramatica.cpp"
#include "CalculadoraPrimero.cpp"


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
						simbolos = CalculadorPrimero :: primero( beta , g);
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



