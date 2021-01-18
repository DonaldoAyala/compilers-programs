#include <string>

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

	char obtener_cabeza () const {
		return cabeza;
	}

	string obtener_cuerpo() const {
		return cuerpo;
	}
	bool operator<(Produccion const& derecha) const {
		if ( cabeza < derecha.cabeza ) {
			return true;
		} else if ( cabeza > derecha.cabeza ) {
			return false;
		} else {
			return cuerpo < derecha.cuerpo;
 		}
	}
};