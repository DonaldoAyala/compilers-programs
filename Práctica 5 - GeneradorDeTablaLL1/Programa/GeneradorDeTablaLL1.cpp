#include <bits/stdc++.h>

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