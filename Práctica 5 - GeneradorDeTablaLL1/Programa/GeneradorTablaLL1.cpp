#include <fstream>
#include <set>
#include <vector>
#include <string>
#include <unordered_set>
#include <iostream>
#include <map>

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

class LectorDeGramatica {
private:
    string ruta;

    
	vector<string> dividir_cadena ( const string cadena, string separador ) {
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
    LectorDeGramatica ( string ruta_ ) {
        ruta = ruta_;
    }


    Gramatica leer_gramatica_de (string ruta)  {
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
		// Se hace un mapeo de los caracteres a las posiciones
		// de la tabla
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

class EscritorDeTabla {
private:
	string ruta;

public:
	EscritorDeTabla( string ruta_ ) {
		ruta = ruta_;
	}

	void escribir_tabla_en(const string & ruta, TablaLL1 tabla) {
		ofstream archivo (ruta);
		const string espacio = "    ";
        if ( archivo.is_open() ) {
			vector<char> columnas = tabla.obtener_simbolos_de_columnas();
			vector<char> filas = tabla.obtener_simbolos_de_filas();

			vector<pair<Produccion, int>> producciones = tabla.obtener_producciones_numeradas();
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

class GeneradorDeTablaLL1 {
public:
	static void generar_tablaLL1 ( string ruta_de_gramatica, string ruta_escritura ) {
		LectorDeGramatica lector(ruta_de_gramatica);
		EscritorDeTabla escritor(ruta_escritura);

		Gramatica g = lector.leer_gramatica_de( ruta_de_gramatica );
		vector<Produccion> producciones = g.obtener_producciones();
		TablaLL1 tabla(g.obtener_no_terminales(), g.obtener_terminales());

		// Enumerar las producciones
		vector<pair<Produccion, int>> producciones_numeradas;
		for ( int i = 0 ; i < producciones.size() ; i++ ) {
			producciones_numeradas.push_back( {producciones[i], i + 1} );
		}
		tabla.asignar_producciones_numeradas( producciones_numeradas );

		// Recorrer las producciones A -> alfa (#)
		for ( auto produccion_numerada : producciones_numeradas ) {
			// Calculando Primero(alfa)
			set<char> primero_alfa = CalculadorPrimero :: primero( produccion_numerada.first.obtener_cuerpo(), g );

			for ( auto simbolo : primero_alfa ) {
				if ( simbolo != '3' )
					tabla.agregar_entrada( produccion_numerada.first.obtener_cabeza(), simbolo, produccion_numerada.second );
			}
			if ( primero_alfa.count('3') ) {
				set<char> siguiente_A = CalculadorSiguiente :: siguiente( produccion_numerada.first.obtener_cabeza(), g );
				for ( auto simbolo : siguiente_A ) {
					tabla.agregar_entrada( produccion_numerada.first.obtener_cabeza(), simbolo, produccion_numerada.second );
				}
			}
		}
		escritor.escribir_tabla_en(ruta_escritura, tabla);
	}
	
	static TablaLL1 generar_tablaLL1 ( Gramatica g ) {

		vector<Produccion> producciones = g.obtener_producciones();
		TablaLL1 tabla(g.obtener_no_terminales(), g.obtener_terminales());

		// Enumerar las producciones
		vector<pair<Produccion, int>> producciones_numeradas;
		for ( int i = 0 ; i < producciones.size() ; i++ ) {
			producciones_numeradas.push_back( {producciones[i], i + 1} );
		}
		tabla.asignar_producciones_numeradas( producciones_numeradas );

		// Recorrer las producciones A -> alfa (#)
		for ( auto produccion_numerada : producciones_numeradas ) {
			// Calculando Primero(alfa)
			set<char> primero_alfa = CalculadorPrimero :: primero( produccion_numerada.first.obtener_cuerpo(), g );

			for ( auto simbolo : primero_alfa ) {
				if ( simbolo != '3' )
					tabla.agregar_entrada( produccion_numerada.first.obtener_cabeza(), simbolo, produccion_numerada.second );
			}
			if ( primero_alfa.count('3') ) {
				set<char> siguiente_A = CalculadorSiguiente :: siguiente( produccion_numerada.first.obtener_cabeza(), g );
				for ( auto simbolo : siguiente_A ) {
					tabla.agregar_entrada( produccion_numerada.first.obtener_cabeza(), simbolo, produccion_numerada.second );
				}
			}
		}
		return tabla;
	}
};



int main () {
	
	GeneradorDeTablaLL1 :: generar_tablaLL1("Gramatica.txt", "Tabla.txt");
	return 0;
}