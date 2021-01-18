#include <iostream>
#include <vector>
#include <set>

using namespace std;

class TablaLR0 {
private:
	char simbolo_inicial;
	map<int,map<char,set<string>>> tabla;
	vector<char> simbolos_de_columnas;
	vector<int> simbolos_de_filas;
	map<Produccion, int> producciones_numeradas;

public:
	TablaLR0 ( vector<char> no_terminales, vector<char> terminales, int conjuntos, map<Produccion, int> producciones, char inicial ) {
		simbolo_inicial = inicial;
		simbolos_de_columnas = terminales;
		simbolos_de_columnas.push_back( '$' );
		simbolos_de_columnas.insert( end(simbolos_de_columnas), begin(no_terminales), end(no_terminales) );
		for ( int i = 1 ; i <= conjuntos ; i++ ) {
			simbolos_de_filas.push_back( i );
		}
		producciones_numeradas.insert(producciones.begin(), producciones.end());
	}

	char obtener_simbolo_inicial() {
		return simbolo_inicial;
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