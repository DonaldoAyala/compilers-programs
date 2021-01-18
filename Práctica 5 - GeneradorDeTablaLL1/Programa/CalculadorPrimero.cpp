#pragma
#include <set>
#include "Gramatica.cpp"

class CalculadorPrimero {
public:
	static set<char> primero ( char a, Gramatica &g ) {
		set<char> simbolos_en_primero;
		if ( g.tiene_terminal(a) || a == '3') { // 'a' Es un terminal
			simbolos_en_primero.insert( a );
		} else { // 'a' es no terminal
			vector<Produccion> producciones = g.obtener_producciones_con_cabeza(a);
			for ( auto produccion : producciones ) {
				if ( produccion.obtener_cuerpo() == "3" ) {
					simbolos_en_primero.insert( '3' );
				} else {
					set<char> simbolos = primero( produccion.obtener_cuerpo(), g );
					simbolos_en_primero.insert( simbolos.begin(), simbolos.end() );
				}
			}
		}
		return simbolos_en_primero;
	}

	static set<char> primero ( string s, Gramatica g ) {
		set<char> simbolos_en_primero;
		for ( int i = 0 ; i < s.size() ; i++ ) {
			set<char> simbolos = primero( s[i] , g );

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
};