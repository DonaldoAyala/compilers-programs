#include <iostream>
#include <vector>
#include <set>

using namespace std;

class TablaLL1 {
private:
	map<char,map<char,set<int>>> tabla;
	vector<char> simbolos_de_columnas;
	vector<char> simbolos_de_filas;
	vector<pair<Produccion, int>> producciones_numeradas;

public:

	TablaLL1 ( vector<char> no_terminales, vector<char> terminales ) {
		simbolos_de_columnas = terminales;
		simbolos_de_columnas.push_back( '$' );
		simbolos_de_filas = no_terminales;
	}

	set<int> obtener_entrada(char fila, char columna) {
		return tabla[fila][columna];
	}

	void agregar_entrada( char fila, char columna, int numero_produccion ) {
		tabla[fila][columna].insert( numero_produccion );
	}

	void asignar_producciones_numeradas( vector<pair<Produccion, int>> producciones_numeradas ) {
		this -> producciones_numeradas = producciones_numeradas;
	}

	vector<pair<Produccion, int>> obtener_producciones_numeradas() {
		return producciones_numeradas;
	}

	vector<char> obtener_simbolos_de_columnas () {
		return simbolos_de_columnas;
	}
	vector<char> obtener_simbolos_de_filas () {
		return simbolos_de_filas;
	}
};