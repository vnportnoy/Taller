#include <cassert>
#include <iostream>
#include <fstream>
#include "resman.h"

// Ctor
ResMan::ResMan() {
	err_surface = NULL;
}

// Dtor
ResMan::~ResMan() {
	//Se podria agregar un clean al destructor para evitar la assertion
	//this -> clean();
	// Checkeo que se hayan liberado todas las surfaces
	assert(surface_map.size() == 0);
}

void ResMan::init() {
	// Carga la surface de error 
	err_surface = new Surface;
	// Creamos el archivo tmp con la imagen de error
	std::string tmp_path = "err.tmp.bmp";
	std::fstream tmp_file(tmp_path, std::fstream::out | std::fstream::binary | std::fstream::trunc);
	tmp_file.write((const char*)err_img_bmp, sizeof(err_img_bmp));
	tmp_file.close();
	// Leemos y cargamos
	bool res = err_surface->load(tmp_path);
	// Verifica si cargo bien o no
	if(!res) {
		std::cerr << "Error loading error surface\n";
		delete err_surface;
		err_surface = NULL;
	}
}

// Agrega el recurso con nombre name ubicado en el path dado por path
// Devuelve true o false y loggea a std::cerr en caso de error
bool ResMan::addRes(const std::string& name, const std::string& path) {
	// Checkea que el resource con ese nombre no exista
	if(surface_map.find(name) != surface_map.end()) {
		std::cerr << "Error, resource <" << name << "> already exists\n";
		return false;
	}
	// Caso contrario intenta cargarlo
	Surface* tmp = new Surface;
	bool load_res = tmp->load(path);
	// Verifica si cargo bien o no
	if(!load_res) {
		std::cerr << "Error loading resource <" << name << ">\n";
		return false;
	}else{
		// Guardamos en caso de exito
		surface_map[name] = tmp;
	}
	return true;
}

// Agrega el recurso con nombre name ubicado en el path dado por path
// Devuelve true o false y loggea a std::cerr en caso de error
// Version con color key
bool ResMan::addRes(const std::string& name, const std::string& path, const int colorKey) {
	// Checkea que el resource con ese nombre no exista
	if(surface_map.find(name) != surface_map.end()) {
		std::cerr << "Error, resource <" << name << "> already exists\n";
		return false;
	}
	// Caso contrario intenta cargarlo
	Surface* tmp = new Surface;
	bool load_res = tmp->load(path, colorKey);
	// Verifica si cargo bien o no
	if(!load_res) {
		std::cerr << "Error loading resource <" << name << ">\n";
		return false;
	}else{
		// Guardamos en caso de exito
		surface_map[name] = tmp;
	}
	return true;
}

// Devuelve un puntero a la Surface asociada con name
// En caso de error devuelve la surface de error (TODO)
Surface* ResMan::getRes(const std::string& name) const {
	if(surface_map.find(name) != surface_map.end()) {
		return surface_map.find(name)->second;
	}else{
		std::cerr << "Error, resource <" << name << "> requested\n";
		return err_surface; // Devuelve la surface de error precargada
	}
}

// Libera todas las surfaces - se tiene que llamar antes de salir
void ResMan::clean() {
	for(auto it = surface_map.begin(); it != surface_map.end(); ++it) {
		// Liberamos la memoria asociada 
		it->second->destroy();
		// Borramos del heap
		delete it->second;
	}
	// Borramos todos los elementos ya que los punteros estan liberados
	surface_map.clear();
	// Liberamos la err_surface
	if(err_surface != NULL) {
		err_surface->destroy();
		delete err_surface;
	}
}