#pragma
#include <set>
#include "Gramatica.cpp"

class CalculadorPrimero {
private:
	set<char> calculando_simbolo;
	set<string> calculando_cadena;

	set<char> primero_recursivo ( char a, Gramatica &g ) {
		set<char> simbolos_en_primero;
		// Aún no hemos calculado el simbolo a
		if ( calculando_simbolo.count(a) == 0 ) {
			calculando_simbolo.insert(a);
		} else {
			return simbolos_en_primero;
		}
		if ( g.tiene_terminal(a) || a == '3') { // 'a' Es un terminal o epsilon
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