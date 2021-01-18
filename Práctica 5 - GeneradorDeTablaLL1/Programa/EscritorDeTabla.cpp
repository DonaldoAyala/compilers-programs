#include <string>
#include <vector>
#include <iostream>
#include <fstream>

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