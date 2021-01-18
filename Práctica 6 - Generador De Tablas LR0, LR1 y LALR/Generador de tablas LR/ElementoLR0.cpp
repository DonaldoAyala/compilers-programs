#include <string>
#include "Produccion.cpp"

using namespace std;

class ElementoLR0 {
private:
	Produccion produccion;
	int posicion_punto;

public:
	ElementoLR0(Produccion p) : produccion(p) {
		posicion_punto = 0;
	}
	
	char obtener_siguiente_simbolo () {
		string cuerpo = produccion.obtener_cuerpo();
		if ( posicion_punto < cuerpo.size() )
			return cuerpo[posicion_punto];
		else
			return '%';
	}
	void avanzar_punto () {
		posicion_punto++;
	}

	bool tiene_punto_al_final () {
		string cuerpo = produccion.obtener_cuerpo();
		return posicion_punto == cuerpo.size();
	}

	Produccion obtener_produccion () {
		return produccion;
	}

	int obtener_posicion_punto () {
		return posicion_punto;
	}

	bool operator<(ElementoLR0 const& derecha) const {
		if ( produccion < derecha.produccion ) {
			return true;
		} else if ( derecha.produccion < produccion ) {
			return false;
		} else {
			return posicion_punto < derecha.posicion_punto;
		}
	}
};