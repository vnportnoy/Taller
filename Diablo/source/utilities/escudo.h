#pragma	once

#include "item.h"

class Escudo: public Item {
	private:
		char energiaEscudo;
	public:
		//Constructor
		Escudo();

		Escudo(const std::string& name,
			const unsigned int wTiles , const unsigned int hTiles , bool caminable,
			const int pixel_ref_x , const int pixel_ref_y,
			Tile* tile , Mapa* mapa,
			ResMan& rm , const int colorKey):Item(name,wTiles,hTiles,caminable,pixel_ref_x,pixel_ref_y,tile,mapa,rm,colorKey) {
				energiaEscudo=20;
		}

		void chocarCon(Personaje* personaje) {
			personaje->chocarConEscudo(this);
		}

		char getEnergiaEscudo() { return this->energiaEscudo; }

};