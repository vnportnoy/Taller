#pragma	once

#include "item.h"

class Bombas: public Item {
	private:
		char bombasGanadas;
	public:
		//Constructor
		Bombas();

		Bombas(const std::string& name,
			const unsigned int wTiles , const unsigned int hTiles , bool caminable,
			const int pixel_ref_x , const int pixel_ref_y,
			Tile* tile , Mapa* mapa,
			ResMan& rm , const int colorKey):Item(name,wTiles,hTiles,caminable,pixel_ref_x,pixel_ref_y,tile,mapa,rm,colorKey) {
		bombasGanadas = 5;
		}

		void chocarCon(Personaje* personaje) {
			personaje->chocarConBombas(this);
		}

		char getBombasGanadas() { return this->bombasGanadas; }

};
