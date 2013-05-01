#include <iostream>
#include <cassert>

#include "surface.h"
#include "../utilities/logErrores.h"
#include <sstream>

Surface::Surface() {
	image = NULL;
}

void Surface::nuevoSurfaceConfigurado(const int alto , const int ancho , const SDL_VideoInfo* vi ,
									const int colorKey) {
	if (this -> image != NULL) {
		SDL_FreeSurface(image);
		this -> image = NULL;
	}
	this -> image = SDL_CreateRGBSurface (SDL_HWSURFACE, ancho , alto , 
					vi->vfmt->BitsPerPixel , vi->vfmt->Rmask, vi->vfmt->Gmask,
					vi->vfmt->Bmask , vi->vfmt->Amask);
	Uint32 ckey = SDL_MapRGB(image -> format , 0, colorKey, 0);
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, ckey);
}

void Surface::nuevoSurfaceConfigurado(const int alto , const int ancho , const SDL_VideoInfo* vi) {
	if (this -> image != NULL) {
		SDL_FreeSurface(image);
		this -> image = NULL;
	}
	this -> image = SDL_CreateRGBSurface (SDL_HWSURFACE, ancho , alto , 
					vi->vfmt->BitsPerPixel , vi->vfmt->Rmask, vi->vfmt->Gmask,
					vi->vfmt->Bmask , vi->vfmt->Amask);
	Uint32 ckey = SDL_MapRGB(image -> format , 0,Surface::BMP_TRANSPARENCIA , 0);
	SDL_SetColorKey(image , SDL_SRCCOLORKEY , ckey);
}

Surface::~Surface() {
	if(image != NULL) {
		SDL_FreeSurface(image);
	}
	this -> image = NULL;
}

bool Surface::load(const std::string& filename) {
	// Load the image in the surface
	SDL_Surface *temp_surf = IMG_Load(filename.c_str());
	// Check for errors
	if(temp_surf != NULL) {
		// Optimized format
		image = SDL_DisplayFormat(temp_surf);
		SDL_FreeSurface(temp_surf);
		// Apply color key
		Uint32 ckey = SDL_MapRGB(image -> format , 0, Surface::BMP_TRANSPARENCIA, 0);
		SDL_SetColorKey(image, SDL_SRCCOLORKEY, ckey);
		return true;
	}else{
		std::stringstream ss;
		ss << "Error loading " << filename << ": " << IMG_GetError() << "\n";
		err_log.escribir(ss.str());
		return false;
	}
}

bool Surface::load(const std::string& filename, const int colorKey) {
	// Load the image in the surface
	SDL_Surface *temp_surf = IMG_Load(filename.c_str());
	// Check for errors
	if(temp_surf != NULL) {
		// Optimized format
		image = SDL_DisplayFormat(temp_surf);
		SDL_FreeSurface(temp_surf);
		// Apply color key
		Uint32 ckey = SDL_MapRGB(image -> format , 0, colorKey, 0);
		SDL_SetColorKey(image, SDL_SRCCOLORKEY , ckey);
		return true;
	}else{
		std::stringstream ss;
		ss << "Error loading " << filename << ": " << IMG_GetError() << "\n";
		err_log.escribir(ss.str());
		return false;
	}
}

void Surface::destroy() {
	if(image != NULL) {
		SDL_FreeSurface(image);
		this -> image = NULL;
	}
}

void Surface::blit(SDL_Surface* destination, int x, int y) {
	//load a destination SDL_Rect
	SDL_Rect destino;
	destino.x = x;
	destino.y = y;

	//Blitting
	SDL_BlitSurface(image, NULL, destination, &destino);
}

void Surface::blit(SDL_Surface* destination, int x, int y, SDL_Rect& clip) {
	assert(image != NULL);
	//load destination SDL_Rect
	SDL_Rect destino;
	destino.x = x;
	destino.y = y;

	//Blitting
	SDL_BlitSurface(image, &clip, destination, &destino);
}

int Surface::height() const {
	assert(image != NULL);
	return image->h;
}

int Surface::width() const {
	assert(image != NULL);
	return image->w;
}

SDL_Surface* Surface::getSDL_Surface() {
	return this -> image;
}

void Surface::setSurface(SDL_Surface* nuevaSurface) {
	SDL_FreeSurface(this->image);
	this->image=nuevaSurface;
}

