#pragma once

#include <string>

class config_entidad {

	private:
		std::string nombre,path_imagen;
		int ancho_base,alto_base,pixel_ref_x,pixel_ref_y,fps,delay;
		bool completoNombre, completoPath, completoAncho, completoAlto, completoPixelX, completoPixelY, completoFps, completoDelay;

	public:
		//Constructor
		//i: inicial
		config_entidad(std::string nombre_i, std::string path_imagen_i, int ancho_base_i, int alto_base_i, int pixel_ref_x_i, int pixel_ref_y_i, int fps_i, int delay_i);

		//Getters
		std::string get_nombre() { return nombre;}

		std::string get_path_imagen() { return path_imagen;}

		int get_ancho_base() { return ancho_base;}

		int get_alto_base() { return alto_base;}

		int get_pixel_ref_x() { return pixel_ref_x;}

		int get_pixel_ref_y() { return pixel_ref_y;}

		int get_fps() { return fps;}

		int get_delay() { return delay;}

		//gets de los flags de control
		bool get_completo_nombre() { return completoNombre;}
		
		bool get_completo_path() { return completoPath;}
		
		bool get_completo_alto() { return completoAlto;}
		
		bool get_completo_ancho() { return completoAncho;}
		
		bool get_completo_pixel_X() { return completoPixelX;}

		bool get_completo_pixel_Y() { return completoPixelY;}
		
		bool get_completo_fps() { return completoFps;}

		bool get_completo_delay() { return completoDelay;}
		
		//Setters
		void set_nombre(std::string newValue) { nombre = newValue;}

		void set_path_imagen(std::string newValue) { path_imagen = newValue;}
		void set_ancho_base(int newValue) { ancho_base = newValue;}

		void set_alto_base(int newValue) { alto_base = newValue;}

		void set_pixel_ref_x(int newValue) { pixel_ref_x = newValue;}

		void set_pixel_ref_y(int newValue) { pixel_ref_y = newValue;}

		void set_fps(int newValue) { fps = newValue;}

		void set_delay(int newValue) { delay = newValue;}
		
		//metodos para completar validaciones
				//metodos para completar validaciones
		void completo_nombre(){
			completoNombre = true;
		}
		void completo_path(){
			completoPath = true;
		}
		void completo_Alto(){
			completoAlto = true;
		}
		void completo_Ancho(){
			completoAncho = true;
		}
				//metodos para completar validaciones
		void completo_pixelX(){
			completoPixelX = true;
		}
		void completo_pixelY(){
			completoPixelY = true;
		}
		void completo_fps(){
			completoFps = true;
		}
		void completo_delay(){
			completoDelay = true;
		}
};