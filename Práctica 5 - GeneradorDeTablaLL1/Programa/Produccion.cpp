#include <string>

using namespace std;

class Produccion {
private:
	char cabeza;
	string cuerpo;
public:
	Produccion ( char cabeza_, string cuerpo_ ) {
		cabeza = cabeza_;
	}

	bool contiene_simbolo ( char simbolo ) {
		for ( int i = 0 ; i < cuerpo.size() ; i++ ) {
			if ( cuerpo[i] == simbolo ) return;
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