#pragma once
//#include "../utilities/Personaje.h"
#include "tile.h"
#include <vector>
#include "SDL.h"
class Personaje;
class Tile;
using namespace std;
typedef std::vector<Tile> TileVec;
#include "../utilities/grafo.h"
class Camara;
class PjeManager;
class Mapa {
	private:
	TileVec tiles; // Vector de tiles para el mapa
	int w, h;
	// Assigna a los tiles las coordenadas
	// Esta funcion es privada y se llama automaticamente cuando se hace un resize del mapa
	// (en teoria deberia ser una sola vez, cuando ser carga el mapa)
	grafo grafoVertices;
	void assignTileCoords();

	public:
	// Ctor
	Mapa();
	// Hace un resize del mapa, es indefinido que pasa si el mapa no estaba en size 0
	void resize(int w, int h);
	// Getter para cada tile
	Tile* getTile(int x, int y);
	// Verifica si el tile existe
	bool tileExists(int x, int y) const;
	// Getter para el vector de tiles, para recorrerlo entero
	TileVec& allTiles();
	// Blittea todo el mapa
	void blit(SDL_Surface* dest, Camara& camara , Personaje* personaje);
	//setea entidades dibujadas en false
	void setEntidadesDibujadasFalse(Personaje* personaje);
	// Libera los recursos
	void clean();
	// Devuelve el tile segun su posicion en el mapa dado por el pixel superior
	Tile* getTilePorPixeles(int pixelX , int pixelY);
	// Getters para tama�o
	void getSize(int* x, int* y) const;
	//carga el grafo con la informacion del mapa en ese momento
	void cargarGrafo();
	//actualiza unicamente un vertice
	void Mapa::actualizarGrafoVertice(int tileX,int tileY);
	//actualiza el grafo con la informacion del mapa luego de moverse de un tile a otro
	void Mapa::actualizarGrafo(int pos_x, int pos_y);
	//devuelve el camino minimo
	vector <pair<int,int>> getCaminoMinimo(Tile* tileOrigen, Tile* tileDestino);
};