#include "mapa.h"
#include "camara.h"
#include "../utilities/vec2.h"
#include "../utilities/coordenadas.h"
#include "../utilities/grafo.h"
#include "../net/PjeManager.h"

// Ctor
Mapa::Mapa() {
	w = h = 0;
}

// Hace un resize del mapa, es indefinido que pasa si el mapa no estaba en size 0
void Mapa::resize(int w, int h) {
	this->w = w;
	this->h = h;
	tiles.resize(w*h);
	assignTileCoords();
}

// Getter para cada tile
Tile* Mapa::getTile(int x, int y) {
	if(tileExists(x, y)) {
		return &(tiles[x*h + y]);
	}else{
		return NULL;
	}
}

// Chequea que el tile exista
bool Mapa::tileExists(int x, int y) const {
	return x >= 0 && y >= 0 && x < w && y < h;
}

// Getter para el vector de tiles, para recorrerlo entero
TileVec& Mapa::allTiles() {
	return tiles;
}

void Mapa::blit(SDL_Surface* dest, Camara& camara, Personaje* personaje) {
	// Primero calculamos cuantos tiles nos vamos a tener que mover horizontal y verticalmente
	const int tile_count_x = (camara.getW() / Tile::TILE_ANCHO) + 5;
	const int tile_count_y = 2*((camara.getH() / Tile::TILE_ALTO) + 5);
	// Calculamos el tile de la esquina superior izquierda
	vec2<int> curr_tile = MouseCoords2Tile(vec2<int>(0, 0), camara);
	// Lo movemos uno mas para el N y al E
	curr_tile = tileWalk(curr_tile, GDIR::E);
	curr_tile = tileWalk(curr_tile, GDIR::E);
	curr_tile = tileWalk(curr_tile, GDIR::NE);
	curr_tile = tileWalk(curr_tile, GDIR::N);
	// Con esta variable controlamos si cuando bajamos de fila vamos hacia el SO o hacia el el SE
	bool last_turn_SO = true;
	// Iteramos
	for(int y = 0;y < tile_count_y;y++) {
		vec2<int> fila_start = curr_tile;
		for(int x = 0;x < tile_count_x;x++) {
			// Si el tile en el que estamos parados es valido, lo blitteamos
			if(tileExists(curr_tile.x, curr_tile.y)) {
				getTile(curr_tile.x, curr_tile.y)->blit(dest, camara, personaje, this);
			}
			// Nos movemos para el oeste
			curr_tile = tileWalk(curr_tile, GDIR::O);
		}
		// Bajamos de fila
		if(last_turn_SO) {
			curr_tile = tileWalk(fila_start, GDIR::SE);
		}else{
			curr_tile = tileWalk(fila_start, GDIR::SO);
		}
		last_turn_SO = !last_turn_SO;
	}


	// Version que blittea recorriendo los tiles matricialmente
	//for(auto it = tiles.begin(); it != tiles.end(); ++it) {
		//it->blit(dest, camara);
	//}
}

void Mapa::clean() {
	for(auto it = tiles.begin(); it != tiles.end(); ++it) {
		it->clean();
	}
}


void Mapa::setEntidadesDibujadasFalse(Personaje* personaje){
	for(auto it = tiles.begin(); it != tiles.end(); ++it) {
		std::vector<Entidad*> entidadesTile = it->getEntidades();
		for(auto it2 = entidadesTile.begin(); it2 != entidadesTile.end(); ++it2){
			(*it2)->setDibujada(false , this , personaje);
		}
	}
}


void Mapa::assignTileCoords() {
	int x, y;
	for(x = 0;x < w;x++) {
		for(y = 0;y < h;y++) {
			Tile* tile = getTile(x,y);
			tile->setBlitCoords((x-y)*Tile::TILE_ANCHO/2, (x+y)*Tile::TILE_ALTO/2);
			tile->setLogicCoords(x,y);
		}
	}
}

// Devuelve el tile segun su posicion en el mapa dado por el pixel superior
Tile* Mapa::getTilePorPixeles(int pixelX , int pixelY) {
	Tile* retorno = NULL;
	int x = 0;
	int y = 0;
	int x0 = pixelX ;
	int y0 = pixelY - Tile::TILE_ALTO/2;
	x = y0 + ((x0 + Tile::TILE_ANCHO/2) / 2);
	y = y0 - ((x0 - Tile::TILE_ANCHO/2) / 2);
	x /= Tile::TILE_ALTO;
	y /= Tile::TILE_ALTO;
	if ((x < this -> w) && (x >= 0) && 
		(y < this -> h) && (y >= 0)){
		retorno = this -> getTile(x , y);
	}
	return retorno;
}

void Mapa::getSize(int* w, int* h) const {
	*w = this->w;
	*h = this->h;
}

//carga el grafo con la informacion del mapa en ese momento
void Mapa::cargarGrafo(){
	int altoMapa = this->h;
	int anchoMapa = this->w;
	double raizDeDos = 1.4142;
	double pesoNoCaminable = 500;

	this->grafoVertices = grafo(altoMapa,anchoMapa);

	for (auto it= allTiles().begin(); it != allTiles().end(); ++it){
		int tileX = it->getU();
		int tileY = it->getV();

		if((tileX-1)>=0){
			//izquierda
			Tile* tileArista = getTile(tileX-1,tileY);
			if(tileArista->isCaminable()){
				grafoVertices.agregar_arista(tileX,tileY,tileX-1,tileY,1);			
			}else{
				grafoVertices.agregar_arista(tileX,tileY,tileX-1,tileY,pesoNoCaminable);								
			}
		}
		if((tileY-1)>=0){
			//arriba
			Tile* tileArista = getTile(tileX,tileY-1);
			if(tileArista->isCaminable()){
				grafoVertices.agregar_arista(tileX,tileY,tileX,tileY-1,1);			
			}else{
				grafoVertices.agregar_arista(tileX,tileY,tileX,tileY-1,pesoNoCaminable);								
			}
		}
		if((tileX+1)<=anchoMapa-1){
			//derecha
			Tile* tileArista = getTile(tileX+1,tileY);
			if(tileArista->isCaminable()){
				grafoVertices.agregar_arista(tileX,tileY,tileX+1,tileY,1);			
			}else{
				grafoVertices.agregar_arista(tileX,tileY,tileX+1,tileY,pesoNoCaminable);								
			}
		}
		if((tileY+1)<=altoMapa-1){
			//abajo
			Tile* tileArista = getTile(tileX,tileY+1);
			if(tileArista->isCaminable()){
				grafoVertices.agregar_arista(tileX,tileY,tileX,tileY+1,1);			
			}else{
				grafoVertices.agregar_arista(tileX,tileY,tileX,tileY+1,pesoNoCaminable);								
			}
		}
		if(((tileX-1)>=0) && ((tileY-1)>=0)){
			//arriba izquierda
			Tile* tileArista = getTile(tileX-1,tileY-1);
			if(tileArista->isCaminable()){
				grafoVertices.agregar_arista(tileX,tileY,tileX-1,tileY-1,raizDeDos);			
			}else{
				grafoVertices.agregar_arista(tileX,tileY,tileX-1,tileY-1,pesoNoCaminable*raizDeDos);								
			}
		}
		if(((tileX-1)>=0) && ((tileY+1)<=altoMapa-1)){
			//abajo izquierda
			Tile* tileArista = getTile(tileX-1,tileY+1);
			if(tileArista->isCaminable()){
				grafoVertices.agregar_arista(tileX,tileY,tileX-1,tileY+1,raizDeDos);			
			}else{
				grafoVertices.agregar_arista(tileX,tileY,tileX-1,tileY+1,pesoNoCaminable*raizDeDos);								
			}
		}
		if(((tileX+1)<=anchoMapa-1) && ((tileY-1)>=0)){
			//arriba derecha
			Tile* tileArista = getTile(tileX+1,tileY-1);
			if(tileArista->isCaminable()){
				grafoVertices.agregar_arista(tileX,tileY,tileX+1,tileY-1,raizDeDos);			
			}else{
				grafoVertices.agregar_arista(tileX,tileY,tileX+1,tileY-1,pesoNoCaminable*raizDeDos);								
			}
		}
		if(((tileX+1)<=anchoMapa-1) && ((tileY+1)<=altoMapa-1)){
			//abajo derecha
			Tile* tileArista = getTile(tileX+1,tileY+1);
			if(tileArista->isCaminable()){
				grafoVertices.agregar_arista(tileX,tileY,tileX+1,tileY+1,raizDeDos);			
			}else{
				grafoVertices.agregar_arista(tileX,tileY,tileX+1,tileY+1,pesoNoCaminable*raizDeDos);								
			}
		}
	}
}
//actualiza unicamente un vertice
void Mapa::actualizarGrafoVertice(int tileX,int tileY){
	int altoMapa = this ->h ;
	int anchoMapa = this->w;
	double raizDeDos = 1.4142;
	double pesoNoCaminable = 500;



	if((tileX-1)>=0){
		//izquierda
		Tile* tileArista = getTile(tileX-1,tileY);
		if(tileArista->isCaminable()){
			grafoVertices.actualizar_arista(tileX,tileY,tileX-1,tileY,1);			
		}else{
			grafoVertices.actualizar_arista(tileX,tileY,tileX-1,tileY,pesoNoCaminable);								
		}
	}
	if((tileY-1)>=0){
		//arriba
		Tile* tileArista = getTile(tileX,tileY-1);
		if(tileArista->isCaminable()){
			grafoVertices.actualizar_arista(tileX,tileY,tileX,tileY-1,1);			
		}else{
			grafoVertices.actualizar_arista(tileX,tileY,tileX,tileY-1,pesoNoCaminable);								
		}
	}
	if((tileX+1)<=anchoMapa-1){
		//derecha
		Tile* tileArista = getTile(tileX+1,tileY);
		if(tileArista->isCaminable()){
			grafoVertices.actualizar_arista(tileX,tileY,tileX+1,tileY,1);			
		}else{
			grafoVertices.actualizar_arista(tileX,tileY,tileX+1,tileY,pesoNoCaminable);								
		}
	}
	if((tileY+1)<=altoMapa-1){
		//abajo
		Tile* tileArista = getTile(tileX,tileY+1);
		if(tileArista->isCaminable()){
			grafoVertices.actualizar_arista(tileX,tileY,tileX,tileY+1,1);			
		}else{
			grafoVertices.actualizar_arista(tileX,tileY,tileX,tileY+1,pesoNoCaminable);								
		}
	}
	if(((tileX-1)>=0) && ((tileY-1)>=0)){
		//arriba izquierda
		Tile* tileArista = getTile(tileX-1,tileY-1);
		if(tileArista->isCaminable()){
			grafoVertices.actualizar_arista(tileX,tileY,tileX-1,tileY-1,raizDeDos);			
		}else{
			grafoVertices.actualizar_arista(tileX,tileY,tileX-1,tileY-1,pesoNoCaminable*raizDeDos);								
		}
	}
	if(((tileX-1)>=0) && ((tileY+1)<=altoMapa-1)){
		//abajo izquierda
		Tile* tileArista = getTile(tileX-1,tileY+1);
		if(tileArista->isCaminable()){
			grafoVertices.actualizar_arista(tileX,tileY,tileX-1,tileY+1,raizDeDos);			
		}else{
			grafoVertices.actualizar_arista(tileX,tileY,tileX-1,tileY+1,pesoNoCaminable*raizDeDos);								
		}
	}
	if(((tileX+1)<=anchoMapa-1) && ((tileY-1)>=0)){
		//arriba derecha
		Tile* tileArista = getTile(tileX+1,tileY-1);
		if(tileArista->isCaminable()){
			grafoVertices.actualizar_arista(tileX,tileY,tileX+1,tileY-1,raizDeDos);			
		}else{
			grafoVertices.actualizar_arista(tileX,tileY,tileX+1,tileY-1,pesoNoCaminable*raizDeDos);								
		}
	}
	if(((tileX+1)<=anchoMapa-1) && ((tileY+1)<=altoMapa-1)){
		//abajo derecha
		Tile* tileArista = getTile(tileX+1,tileY+1);
		if(tileArista->isCaminable()){
			grafoVertices.actualizar_arista(tileX,tileY,tileX+1,tileY+1,raizDeDos);			
		}else{
			grafoVertices.actualizar_arista(tileX,tileY,tileX+1,tileY+1,pesoNoCaminable*raizDeDos);								
		}
	}
	
}
//actualiza el grafo con la informacion del mapa luego de moverse de un tile a otro
void Mapa::actualizarGrafo(int pos_x, int pos_y){
	//actualizo origen y adyacencias
	actualizarGrafoVertice(pos_x,pos_y);
	//actualizo adyacencias de x
	if (tileExists(pos_x + 1 ,pos_y)){
		//derecha
		actualizarGrafoVertice(pos_x + 1,pos_y);
	}
	if (tileExists(pos_x - 1 ,pos_y)){
		//izquierda
		actualizarGrafoVertice(pos_x - 1,pos_y);
	}
	if (tileExists(pos_x ,pos_y -1)){
		//arriba
		actualizarGrafoVertice(pos_x,pos_y -1);
	}
	if (tileExists(pos_x ,pos_y + 1)){
		//abajo
		actualizarGrafoVertice(pos_x,pos_y + 1);
	}
	if (tileExists(pos_x + 1 ,pos_y -1)){
		//derecha arriba
		actualizarGrafoVertice(pos_x + 1,pos_y -1);
	}
	if (tileExists(pos_x + 1 ,pos_y + 1)){
		//derecha abajo
		actualizarGrafoVertice(pos_x + 1,pos_y + 1);
	}
	
	if (tileExists(pos_x -1 ,pos_y + 1)){
		//izquierda abajo
		actualizarGrafoVertice(pos_x - 1,pos_y + 1);
	}
	if (tileExists(pos_x -1 ,pos_y - 1)){
		//izquierda arriba
		actualizarGrafoVertice(pos_x - 1,pos_y - 1);
	}
	

}
vector <pair<int,int> > Mapa::getCaminoMinimo(Tile* tileOrigen, Tile* tileDestino){
	if (tileOrigen == NULL || tileDestino == NULL){
		std::vector<pair<int, int>> vacio;
		return vacio;
	}
	if(tileOrigen == tileDestino){
		std::vector<pair<int, int>> vacio;
		return vacio;
	}
	/*
	if(!tileDestino->isCaminable()){
		std::vector<pair<int, int>> vacio;
		return vacio;
	}
	*/
	int tileOrigenX = tileOrigen->getU();
	int tileOrigenY = tileOrigen->getV();
	int tileDestinoX = tileDestino->getU();
	int tileDestinoY = tileDestino->getV();
	vector <pair<int,int>> camino = grafoVertices.camino(tileOrigenX,tileOrigenY,tileDestinoX,tileDestinoY);
	return camino;
}