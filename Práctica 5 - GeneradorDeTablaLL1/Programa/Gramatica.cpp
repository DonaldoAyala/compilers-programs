#pragma
#include <string>
#include <unordered_set>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>

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

	char obtener_cabeza () {
		return cabeza;
	}

	string obtener_cuerpo() {
		return cuerpo;
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