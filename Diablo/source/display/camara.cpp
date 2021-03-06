#include "camara.h"
#include "../constants/model.h"
#include "mapa.h"
#include <cmath>
#include <algorithm>

// Ctor
Camara::Camara() : x(0.0), y(0.0), w(0), h(0), vx(0.0), vy(0.0), margen(0) {
	// ...
}

// Dtor
Camara::~Camara() {
	// ..
}

// Settea el ancho de la camara y el ancho del margen de deslizamiento
void Camara::init(int w, int h, int marg_width, const Mapa& mapa) {
	this->w = w;
	this->h = h;
	this->margen = marg_width;
	int mapa_w, mapa_h;
	mapa.getSize(&mapa_w, &mapa_h);
	// Traducimos a pixeles, tomando el maximo
	mapa_wh = std::max(mapa_w, mapa_h);
	mapa_wh *= Tile::TILE_ANCHO;
}

// Cambia la posicion de la camara
void Camara::mover(float x, float y) {
	this->x = x;
	this->y = y;
}

// Getters para la posicion
float Camara::getX() const {
	return x;
}

float Camara::getY() const {
	return y;
}

// Getters para el tama�o
int Camara::getW() const {
	return w;
}

int Camara::getH() const {
	return h;
}

// Actualiza la posicion basado en la velocidad usando Euler nomas
void Camara::update() {
	x += vx * CONST_DT;
	y += vy * CONST_DT;
	// Mantenemos la camara dentro del mapa
	if(x > mapa_wh/2 - this->w/2) {
		x = mapa_wh/2 - this->w/2;
	}
	if(x < -mapa_wh/2 - this->w/2) {
		x = -mapa_wh/2 - this->w/2;
	}
	if(y < -this->h/2) {
		y = -this->h/2;
	}
	if(y > mapa_wh/2 - this->h/2) {
		y = mapa_wh/2 - this->h/2;
	}
}

// Actualiza la velocidad basado en la posicion del mouse
void Camara::update_speed(const SDL_Rect& mouse_coords) {
	bool anymod = false;
	// Primero detectamos si estamos dentro del margen de scrolleo
	// Despues se calcula un factor entre [0,1] para aplicar la velocidad de scrolleo de la camara
	if(mouse_coords.x < margen) {
		const float spd_factor = 1.0f - (mouse_coords.x / float(margen));
		vx = -spd_factor*DRAG_SPEED;
		anymod = true;
	}else if(mouse_coords.x > w-margen) {
		const float spd_factor = (mouse_coords.x - w + margen) / float(margen);
		vx = spd_factor*DRAG_SPEED;
		anymod = true;
	}
	if(mouse_coords.y < margen) {
		const float spd_factor = 1.0f - (mouse_coords.y / float(margen));
		vy = -spd_factor*DRAG_SPEED;
		anymod = true;
	}else if(mouse_coords.y > h-margen) {
		const float spd_factor = (mouse_coords.y - h + margen) / float(margen);
		vy = spd_factor*DRAG_SPEED;
		anymod = true;
	}
	if(!anymod) {
		vx = vy = 0.0f;
	}
}

void Camara::center(int x, int y) {
	mover(x - float(getW())/2, y - float(getH())/2);
}