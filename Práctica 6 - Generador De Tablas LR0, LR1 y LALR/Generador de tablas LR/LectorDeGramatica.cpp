#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

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