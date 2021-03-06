#pragma once
#include <fstream>
#include <string>
#include "config_servidor.h"
#include "../source/utilities/config_juego.h"
//incluyo estructuras a controlar
using namespace std;
//se declaran los valores por defecto
const unsigned int anchoDef = 800;
const unsigned int altoDef = 600;
const unsigned int VelPerDef = 100; //velodidad personaje
const unsigned int scrollDef = 30;
const string imagenDef = "Path";
const unsigned int anchoElementoDef = 1;
const unsigned int altoElementoDef = 1;
const unsigned int pixelRefXDef = 1;
const unsigned int pixelRefYDef = 1;
const string nombreEntidadDef = "entidad desconocida";
const unsigned int fpsDef = 0;
const unsigned int delayDef = 0;
const string nombreEscenarioDef = "Lagos de Nepal";
const unsigned int sizeXDef = 100;
const unsigned int sizeYDef = 100;
const unsigned int posXDef = 10;
const unsigned int posYdef = 17;
const unsigned int spriteAltoDef = 10;
const unsigned int spriteAnchoDef = 10;
const bool esCaminableDef = true;
const string ipDef = "127.0.0.1";
const unsigned int puertoDef = 8080;
 

//hay que mover la implementacion del .h al .cpp, se hace de esta forma ya que el compilador tira error sino!!
class logErrores {
	private:
		ofstream conexion;
	public:
		//Constructor
		logErrores(string logPath);
		//escribe el error en el log
		void escribir(string linea);
		//escribe el error seguido del parametro en el log
		void escribir(string linea, string parametro){
			conexion << "-ERROR: "<< linea << ": " << parametro << "." << endl;
		}
		//escribe el error seguido del parametro en el log y su valor para parametros de tipo string
		void escribir(string linea, string parametro, string valor){
			conexion << "-ERROR: "<< linea << ": " << parametro << " con valor: " << valor << "." << endl;
		}
		//escribe el error seguido del parametro en el log y su valor para parametros de tipo int
		void escribir(string linea, string parametro, int valor){
			conexion << "-ERROR: "<< linea << ": " << parametro << " con valor: " << valor << "." << endl;
		}
		//cierra la conexion
		void cerrarConexion(){
			conexion.close();
		}
		//verfica errores de configuracion de red del cliente
		void verificar_errores(config_servidor& config_red, logErrores& logErrores){
			int puerto = config_red.get_puerto();
			//verifico puerto
			if ( (config_red.get_completo_puerto()) == false ){
				logErrores.escribir ("No se ingreso el puerto, se selecciona valor por defecto");
				config_red.set_puerto(puertoDef);
			}else if ( puerto < 0 ){
				logErrores.escribir ("El puerto ingresado es menor o igual que la unidad, se selecciona valor por defecto");
				config_red.set_puerto(puertoDef);		
			}
			
		}

		//verfica errores de pantalla
		void verificar_errores(config_pantalla& pantalla, logErrores& logErrores){
			int alto = pantalla.get_alto();
			int ancho = pantalla.get_ancho();
			//verifico alto
			if ( (pantalla.get_completo_alto()) == false ){
				logErrores.escribir ("No se ingreso el alto de la pantalla, se selecciona valor por defecto");
				pantalla.set_alto(altoDef);
			}else if ( alto <= 0 ){
				logErrores.escribir ("El alto de la pantalla ingresado es menor o igual que la unidad, se selecciona valor por defecto") ;
				pantalla.set_alto(altoDef);			
			}
			//verifico ancho
			if ( (pantalla.get_completo_ancho()) == false ){
				logErrores.escribir ("No se ingreso el ancho de la pantalla, se selecciona valor por defecto");
				pantalla.set_ancho(anchoDef);
			}else if ( ancho <= 0 ){
				logErrores.escribir ("El ancho de la pantalla ingresado es menor o igual que la unidad, se selecciona valor por defecto");
				pantalla.set_ancho(anchoDef);			
			}

		}
		bool esNumerico(string valor){
			bool res = true;
			const char* a = valor.c_str();
			
			//verifico si es numerico si se puede convertir y si es negativo 
			if ( atoi(a)<0)
				return true;
			for (unsigned int i = 0 ; i<strlen(a);i++){
				char b= a[i];
				if(!isdigit(a[i])){
					res = false;
				}
			}
			return res;
		}
		//verfica errores de configuracion
		void verificar_errores(config_general& config, logErrores& logErrores){
			int velocidad = config.get_vel_personaje();
			int margen = config.get_margen_scroll();
			//verifico velocidad del personaje
			if ( (config.get_completo_velocidad()) == false ){
				logErrores.escribir ("No se ingreso la velocidad del personaje en la configuracion, se selecciona valor por defecto");
				config.set_vel_personaje(VelPerDef);
			}else if ( velocidad <= 0 ){
				logErrores.escribir ("La velocidad del personaje ingresada es menor o igual que la unidad, se selecciona valor por defecto");
				config.set_vel_personaje(VelPerDef);		
			}
			//verifico margen del scroll
			if ( (config.get_completo_margen()) == false ){
				logErrores.escribir ("No se ingreso el margen del scroll en la configuracion, se selecciona valor por defecto");
				config.set_margen_scroll(scrollDef);
			}else if ( velocidad <= 0 ){
				logErrores.escribir ("El margen del scroll ingresado es menor o igual que la unidad, se selecciona valor por defecto");
				config.set_margen_scroll(scrollDef);
		
			}
		}
		//verfica errores de entidades
		void verificar_errores(vector <config_entidad>& config, logErrores& logErrores){
			for ( unsigned int i=0; i < config.size(); i++){		
				//agarro los valores de la entidad
				config_entidad& unaEntidad = config[i];
				string imagen = unaEntidad.get_path_imagen();
				int anchoElemento = unaEntidad.get_ancho_base();
				int altoElemento = unaEntidad.get_alto_base();
				int pixelRefX = unaEntidad.get_pixel_ref_x();
				int pixelRefY = unaEntidad.get_pixel_ref_y();
				string nombre = unaEntidad.get_nombre();
				int fps = unaEntidad.get_fps();
				string path = unaEntidad.get_path_imagen();
				int delay = unaEntidad.get_delay();
				int altoSprite = unaEntidad.get_completo_alto_sprite();
				int anchoSprite = unaEntidad.get_completo_ancho_sprite();

				//valido cada atributo
				if ( unaEntidad.get_completo_nombre() == false){
					logErrores.escribir ("No se ingreso el nombre de una entidad "+ nombre +", se selecciona valor por defecto");
					unaEntidad.set_nombre(nombreEntidadDef);
				}
				if ( unaEntidad.get_completo_path() == false){
					logErrores.escribir ("No se ingreso el path de una entidad "+ nombre +", se selecciona valor por defecto");
					unaEntidad.set_path_imagen(imagenDef);
				}
				if ( unaEntidad.get_completo_ancho() == false){
					logErrores.escribir ("No se ingreso el ancho de una entidad "+ nombre +", se selecciona valor por defecto");
					unaEntidad.set_ancho_base(anchoElementoDef);
				}else if( anchoElemento <= 0 ){
					logErrores.escribir ("El ancho de una entidad "+ nombre +" ingresada es menor o igual que la unidad, se selecciona valor por defecto");
					unaEntidad.set_ancho_base(anchoElementoDef);				
				}
				if ( unaEntidad.get_completo_alto() == false){
					logErrores.escribir ("No se ingreso el alto de una entidad "+ nombre +", se selecciona valor por defecto");
					unaEntidad.set_alto_base(altoElementoDef);
				}else if( altoElemento <= 0 ){
					logErrores.escribir ("El alto de una entidad "+ nombre +" ingresada es menor o igual que la unidad, se selecciona valor por defecto");
					unaEntidad.set_ancho_base(anchoElementoDef);				
				}
				if ( unaEntidad.get_completo_pixel_X() == false){
					logErrores.escribir ("No se ingreso el pixel de referencia X de una entidad "+ nombre +", se selecciona valor por defecto");
					unaEntidad.set_pixel_ref_x(pixelRefXDef);
				}else if( pixelRefX < 0 ){
					logErrores.escribir ("El pixel de referencia X de una entidad "+ nombre +" ingresada es menor que la unidad, se selecciona valor por defecto");
					unaEntidad.set_pixel_ref_x(pixelRefXDef);			
				}
				if ( unaEntidad.get_completo_pixel_Y() == false){
					logErrores.escribir ("No se ingreso el pixel de referencia Y de una entidad "+ nombre +", se selecciona valor por defecto");
					unaEntidad.set_pixel_ref_y(pixelRefYDef);
				}else if( pixelRefY < 0 ){
					logErrores.escribir ("El pixel de referencia Y de una entidad "+ nombre +" ingresada es menor que la unidad, se selecciona valor por defecto");
					unaEntidad.set_pixel_ref_y(pixelRefYDef);			
				}
				if ( unaEntidad.get_completo_fps() == false){
						unaEntidad.set_fps(fpsDef);
				}else if( fps < 0 ){
					logErrores.escribir ("El fps de una entidad "+ nombre +" ingresada es menor que la unidad, se selecciona valor por defecto");
					unaEntidad.set_fps(fpsDef);		
				}
				if ( unaEntidad.get_completo_delay() == false){
					unaEntidad.set_delay(delayDef);
				}else if( delay < 0 ){
					logErrores.escribir ("El delay de una entidad "+ nombre +" ingresada es menor que la unidad, se selecciona valor por defecto");
					unaEntidad.set_delay(delayDef);		
				}
				if ( unaEntidad.get_completo_alto_sprite() == false){
					unaEntidad.set_alto_sprite(spriteAltoDef);
				}else if( altoSprite <= 0 ){
					logErrores.escribir ("El alto del sprite de una entidad "+ nombre +" ingresada es menor que la unidad, se selecciona valor por defecto");
					unaEntidad.set_alto_sprite(spriteAltoDef);	
				}
				if ( unaEntidad.get_completo_ancho_sprite() == false){
					unaEntidad.set_ancho_sprite(spriteAnchoDef);
				}else if( anchoSprite <= 0 ){
					logErrores.escribir ("El ancho del sprite de una entidad "+ nombre +" ingresada es menor que la unidad, se selecciona valor por defecto");
					unaEntidad.set_ancho_sprite(spriteAnchoDef);	
				}
				if ( unaEntidad.get_completo_caminable() == false){
					unaEntidad.set_caminable(esCaminableDef);
				}
			}
			return;
		}
		//verfica errores de entidades en juego, con el flag esProtagonista es para imprimir errores segun
		//sea protagonista o entidad
		vector <config_entidad_en_juego> verificar_errores_entidad_en_juego(vector <config_entidad_en_juego>& config, bool esProtagonista, string nombreEscenario, logErrores& logErrores){
			vector <config_entidad_en_juego> resultado;
			for (unsigned int i = 0; i < config.size(); i++){
				//asigno atributos
				config_entidad_en_juego& unaEntidad = config[i];
				string nombre = unaEntidad.get_nombre();
				int pos_x = unaEntidad.get_pos_x();
				int pos_y = unaEntidad.get_pos_y();
				
				//verifico nombre de la entidad/protagonista
				if (  unaEntidad.get_completo_nombre()  == false ){
					if (esProtagonista){
						logErrores.escribir("No se ingreso el nombre de un protagonista " );
					}else{
						logErrores.escribir("No se ingreso el nombre de una entidad " + nombre + "  en el escenario " + nombreEscenario + ", se selecciona valor por defecto");

					}
					unaEntidad.set_nombre(nombreEntidadDef);
				}
				//verifico la pos X entidad/protagonista
				if ( ( unaEntidad.get_completo_posX() ) == false ){
					if (esProtagonista){
						logErrores.escribir ("No se ingreso la posicion x de un protagonista " + nombre + "  en el escenario " + nombreEscenario + ", se selecciona valor por defecto");
					}else{
						logErrores.escribir ("No se ingreso la posicion x de una entidad " + nombre + "  en el escenario " + nombreEscenario + ", se selecciona valor por defecto");

					}
					unaEntidad.set_pos_x(posXDef);
				} else if( pos_x < 0 ) {
					if (esProtagonista){
						logErrores.escribir ("La posicion x de un protagonista " + nombre + " en el escenario " + nombreEscenario + "ingresada es menor que la unidad, se selecciona valor por defecto");
					}else{
						logErrores.escribir ("La posicion x de una entidad " + nombre + " en el escenario " + nombreEscenario + "ingresada es menor que la unidad, se selecciona valor por defecto");					
					}
					unaEntidad.set_pos_x(posXDef);
				}
				//verifico la pos Y entidad/protagonista
				if ( ( unaEntidad.get_completo_posY() ) == false ){
					if (esProtagonista){
						logErrores.escribir ("No se ingreso la posicion y de un protagonista " + nombre + "  en el escenario " + nombreEscenario + ", se selecciona valor por defecto");
					}else{
						logErrores.escribir ("No se ingreso la posicion y de una entidad " + nombre + "  en el escenario " + nombreEscenario + ", se selecciona valor por defecto");

					}
					unaEntidad.set_pos_y(posYdef);
				} else if( pos_y < 0 ) {
					if (esProtagonista){
						logErrores.escribir ("La posicion y de un protagonista " + nombre + " en el escenario " + nombreEscenario + "ingresada es menor que la unidad, se selecciona valor por defecto");
					}else{
						logErrores.escribir ("La posicion y de una entidad " + nombre + " en el escenario " + nombreEscenario + "ingresada es menor que la unidad, se selecciona valor por defecto");					
					}
					unaEntidad.set_pos_y(posYdef);
				}
				resultado.push_back(unaEntidad);
			}
			return resultado;
		}
		void crear_protagonista_defecto(config_escenario& escenario, vector<config_entidad> entidades){
			config_entidad_en_juego unProtagonista(nombreEntidadDef,posXDef,posYdef);
			escenario.agregar_protagonista(unProtagonista);
			
			config_entidad unaEntidadEnJuego (nombreEntidadDef,imagenDef,anchoElementoDef,altoElementoDef,pixelRefXDef,pixelRefYDef,fpsDef,delayDef);
			entidades.push_back(unaEntidadEnJuego);
		}
		void crear_entidad_defecto(vector<config_entidad>& entidades){
			//se crea entidad por defecto, si  es necesario se eliminara mas adelante
			config_entidad unaEntidad(nombreEntidadDef, imagenDef,anchoElementoDef,altoElementoDef,pixelRefXDef,pixelRefYDef, fpsDef, delayDef); 
			entidades.push_back(unaEntidad);

		}
		//verifica si una ip tiene un formato valido
		bool verificar_ip(string ip){
			//ip con formato xxx.xxx.xxx.xxx ( 0<=x<999)
			int index = 0;
			int count = 0;
			int pos[3]; // guarda las posiciones de los . en la ip 
			//recorro cadena
			while(index < ip.length()){
				string caracter = ip.substr(index, 1);
				//guardo ocurrencia
				if (caracter.compare(".") == 0){
					if (count < 3){
						pos[count] = index;
					}
					count ++;
				}
				index++;
			}
			if (count != 3)
				return false;
			string campo1,campo2,campo3,campo4; //guardo los campos de la ip
			campo1 = ip.substr(0,pos[0]);
			campo2 = ip.substr(pos[0]+1,pos[1]-pos[0]-1);
			campo3 = ip.substr(pos[1]+1,pos[2]-pos[1]-1);
			campo4 = ip.substr(pos[2]+1,ip.length()-pos[2]);
			//si algun campo esta vacio n oes valido
			if ((campo1.size() == 0) ||(campo2.size() == 0)||(campo3.size() == 0)||(campo4.size() == 0))
				return false;
			//si algun campo tiene mas de 3 caracteres no es valido
			if ((campo1.size() > 3) ||(campo2.size() > 3)||(campo3.size() > 3)||(campo4.size() > 3))
				return false;
			//valido que sean campos numericos 
			if ((!esNumerico(campo1)) || (!esNumerico(campo2))|| (!esNumerico(campo2))|| (!esNumerico(campo3))|| (!esNumerico(campo4)))
				return false;
			//verifico que tenga valor maximo correco correctos
			int valor1,valor2,valor3,valor4;
			valor1 = atoi(campo1.c_str());
			valor2 = atoi(campo2.c_str());
			valor3 = atoi(campo3.c_str());
			valor4 = atoi(campo4.c_str());
			if ( (valor1 < 0) || (valor1>999) || (valor2 < 0) || (valor2>999) || (valor3 < 0) || (valor3>999) || (valor4 < 0) || (valor4>999))
				return false;
			return true;
		}
		//verfica errores de escenario, se le pasan las entidades por si es necesario agregar una por defecto
		void verificar_errores(vector <config_escenario>& config, logErrores& logErrores,vector<config_entidad>& entidades){
			for (unsigned int i=0; i < config.size(); i++){
				//guardo atributos
				config_escenario& unEscenario = config[i];
				string nombre = unEscenario.get_nombre();
				int size_x = unEscenario.get_tam_x();
				int size_y = unEscenario.get_tam_y();

				//verifico nombre del personaje
				if ( ( unEscenario.get_completo_nombre() ) == false ){
					logErrores.escribir ("No se ingreso el nombre de un escenario, se selecciona valor por defecto");
					unEscenario.set_nombre(nombreEntidadDef);
				}
				//verifico el sizeX del scroll
				if ( ( unEscenario.get_completo_tamX() ) == false ){
					logErrores.escribir ("No se ingreso el size x de un escenario" + nombre + ", se selecciona valor por defecto");
					unEscenario.set_tam_x(anchoDef);
				}else if ( size_x < 0 ){
					logErrores.escribir ("El size x ingresado en un escenario " + nombre + " es menor que la unidad, se selecciona valor por defecto");
					unEscenario.set_tam_x(anchoDef);
				}
				//verifico el sizeY del scroll
				if ( ( unEscenario.get_completo_tamY() ) == false ){
					logErrores.escribir ("No se ingreso el size y de un escenario" + nombre + ", se selecciona valor por defecto");
					unEscenario.set_tam_y(anchoDef);
				}else if ( size_y < 0 ){
					logErrores.escribir ("El size y ingresado en un escenario " + nombre + " es menor que la unidad, se selecciona valor por defecto");
					unEscenario.set_tam_y(anchoDef);
				}
				//verifico los protagonistad
				if ( ( unEscenario.get_completo_protagonista() ) == false ){
					logErrores.escribir ("No se ingreso ningun protagonista en un escenario" + nombre + ", se selecciona uno por defecto");
					crear_protagonista_defecto(unEscenario,entidades);
					crear_entidad_defecto(entidades);
				}else if ( unEscenario.get_protagonitas().size() > 1){
					logErrores.escribir ("Se ingreso mas de un protagonista en un escenario" + nombre + ", se selecciona uno por defecto");				
					vector<config_entidad_en_juego> protagonistas_en_juego = verificar_errores_entidad_en_juego( unEscenario.get_protagonitas(), true, nombre, logErrores );
					unEscenario.set_protagonistas(protagonistas_en_juego);
				}else{
					//se ingreso un unico protagonsita	
					vector<config_entidad_en_juego> protagonistas_en_juego = verificar_errores_entidad_en_juego( unEscenario.get_protagonitas(), true, nombre, logErrores );
					unEscenario.set_protagonistas(protagonistas_en_juego);
				}


				//verifico las entidades
				vector<config_entidad_en_juego> entidades_en_juego = verificar_errores_entidad_en_juego( unEscenario.get_entidades(), false, nombre, logErrores );
				unEscenario.set_entidades(entidades_en_juego);
			}
			if (config.size() == 0){
				//no hay escenario seteo todo por defecto
				config_escenario unEscenario(nombreEscenarioDef,anchoDef,altoDef);
				crear_protagonista_defecto(unEscenario,entidades);
				crear_entidad_defecto(entidades);
				config.push_back(unEscenario);
			}
		}
		//verifica que no haya dos entidades iguales
		void verificar_unicidad_entidades(vector<config_entidad>& entidades, logErrores& logErrores){
			int cont = 0;
			unsigned int sizeOriginal =entidades.size();

			for (vector<config_entidad>::iterator it = entidades.begin();it != entidades.end(); ++it){
				for (unsigned int i = cont + 1; i < sizeOriginal;i++){
					config_entidad& unaEntidad = *it;
					config_entidad& otraEntidad = entidades[i];
					string unNombre = unaEntidad.get_nombre();
					string otroNombre = otraEntidad.get_nombre();
					if( unNombre == otroNombre ){
						logErrores.escribir("Se entontraron entidades con el mismo nombre " + unNombre + " se borra una");
						entidades.erase(it);
						i--;
						sizeOriginal--;
					}
				}
				cont++;
			}

		}
		//verifica si una entidad pertene a entidades
		bool verificar_existencia_entidad(string unaEntidad, vector<config_entidad>entidades){
			unsigned int size = entidades.size();
			bool encontrado = false;
			unsigned int indice = 0;
			while ( (!encontrado) && (indice<size) ){
				config_entidad otraEntidad = entidades[indice];
				if ( otraEntidad.get_nombre() == unaEntidad){
					encontrado = true;
				}
				indice++;
			}
			return encontrado;
		}
		//verifica si hay correspondencia entre las entidades en juego y las entidades declaradas (sirve para entidades y protagonistas)
		vector<config_entidad_en_juego> verificar_correspondencia_entidades (logErrores& logErrores,vector<config_entidad>& entidades, vector
			<config_entidad_en_juego>& entidadaesJuego)
		{
			vector<config_entidad_en_juego> vectorDefinitivo;

			for (vector<config_entidad_en_juego>::iterator it= entidadaesJuego.begin(); it != entidadaesJuego.end();++it){
				config_entidad_en_juego unaEntidadJuego = *it;
				bool existe = verificar_existencia_entidad(unaEntidadJuego.get_nombre(),entidades);
				//se copia en un nuevo vector ya que el iterador de vector funciona incorrectamente cuando se borra un elemento
				if (!existe){
					//si no existe una entidad declarada correspondiente no la copio y informo el error 
					logErrores.escribir("No existe la delaracion una entidad " + unaEntidadJuego.get_nombre() + " que se declaro en el escenario");
				} else {
					//si existe lo agrego al vector definitivo
					vectorDefinitivo.push_back(unaEntidadJuego);
				}
			}
			return vectorDefinitivo;
		}
		//verifica si hay correspondencia entre las entidades en juego y las entidades declaradas
		void verificar_correspondencia_entidades_en_escenario (logErrores& logErrores,vector<config_entidad>& entidades,
			config_escenario& unEscenario)
		{
			vector <config_entidad_en_juego> entidadesEnJuego = unEscenario.get_entidades();
			//verificamos si las entidades en juego corresponden con alguna entidad declarada
			vector<config_entidad_en_juego> entidadesDefinitivas;
			entidadesDefinitivas = verificar_correspondencia_entidades(logErrores,entidades,entidadesEnJuego);
			unEscenario.set_entidades(entidadesDefinitivas);
		}
		//verifica si hay correspondencia entre los protagonistas en juego y las entidades declaradas
		void verificar_correspondencia_protagonistas_en_escenario (logErrores& logErrores,vector<config_entidad>& entidades,
			config_escenario& unEscenario)
		{
			vector <config_entidad_en_juego> protagonistasEnJuego = unEscenario.get_protagonitas();
			//verificamos si las entidades en juego corresponden con alguna entidad declarada
			vector<config_entidad_en_juego> protagonitasDefinitivos;
			protagonitasDefinitivos = verificar_correspondencia_entidades(logErrores,entidades,protagonistasEnJuego);
			if (protagonitasDefinitivos.size() < 1){
				//creo protagonista por defecto, no se usa la funcion crearprotagonista ya que agrega uno al vector sirve solo
				//si no hay protagonistas
				config_entidad_en_juego unProtagonista(nombreEntidadDef,posXDef,posYdef);
				protagonitasDefinitivos.push_back(unProtagonista);
				config_entidad unaEntidadEnJuego (nombreEntidadDef,imagenDef,anchoElementoDef,altoElementoDef,pixelRefXDef,pixelRefYDef,fpsDef,delayDef);
				entidades.push_back(unaEntidadEnJuego);
			}
			unEscenario.set_protagonistas(protagonitasDefinitivos);
			
		}
		//verifica si hay correspondencia entre los elemento escenario y las entidades declaradas y devuelve los escenarios con esta verificacion
		void verificar_correspondencia_escenario (logErrores& logErrores,vector<config_entidad>& entidades, vector
			<config_escenario>& escenarios)
		{
			vector<config_escenario> escenariosDefinitivos;
			for (unsigned int i=0; i < escenarios.size();i++){
				//para cada escenario verificamos si las entidades y protagonistas en juego corresponden
				//con alguna entidad declarada
				config_escenario unEscenario = escenarios[i];
				vector<config_entidad_en_juego> entidadesEnJuego = unEscenario.get_entidades();
				vector<config_entidad_en_juego> protagonistasEnJuego = unEscenario.get_protagonitas();
				verificar_correspondencia_entidades_en_escenario(logErrores,entidades,unEscenario);
				verificar_correspondencia_protagonistas_en_escenario(logErrores,entidades,unEscenario);
				escenariosDefinitivos.push_back(unEscenario);
			}

			escenarios = escenariosDefinitivos;
			return;
		}
		//devuelve la entidad que tiene el nombre deseado
		config_entidad buscar_entidad(vector<config_entidad> entidades,string nombre){
			unsigned int size = entidades.size();
			bool encontrado = false;
			unsigned int indice = 0;
			while ( (!encontrado) && (indice<size) ){
				config_entidad otraEntidad = entidades[indice];
				if ( otraEntidad.get_nombre() == nombre){
					encontrado = true;
				}
				indice++;
			}
			return entidades[indice-1];
		}
		//verifica los limites de un escenario
		void verificar_limites_un_escenario (logErrores& logErrores,vector<config_entidad>& entidades,
			config_escenario& unEscenario)
		{
			vector <config_entidad_en_juego>& entidadesEnJuego = unEscenario.get_entidades();

			vector<config_entidad_en_juego> entidadesDefinitivas;
			for (unsigned int i = 0; i<entidadesEnJuego.size();i++){
				//busco la entidad correspondiente
				config_entidad_en_juego& unaEntidadEnJuego = entidadesEnJuego[i];
				config_entidad unaEntidadDeclarada = buscar_entidad(entidades,unaEntidadEnJuego.get_nombre());

				int pos_x = unaEntidadEnJuego.get_pos_x();
				int pos_y = unaEntidadEnJuego.get_pos_y();
				int ancho = unaEntidadDeclarada.get_ancho_base(); 
				int alto = unaEntidadDeclarada.get_alto_base();  
				int tam_mapa_x = unEscenario.get_tam_x()-1;// se resta 1 ya que va en el rango(0-ancho)
				int tam_mapa_y = unEscenario.get_tam_y()-1;// se resta 1 ya que va en el rango(0-alto)
				int ultimo_tile_x = pos_x + ancho - 1; //se le resta uno porque el tile pos_x cuenta como 1
				int ultimo_tile_y = pos_y + alto - 1;  //se le resta uno porque el tile pos_y cuenta como 1

				//verifico si se va del mapa tipo error sino la agrego a la lista definitiva
				if( (ultimo_tile_x > tam_mapa_x) || (ultimo_tile_y > tam_mapa_y) ){
					logErrores.escribir("Hay una entidad "+ unaEntidadDeclarada.get_nombre() +" declarada en el mapa que sale de los limites, se elimina la misma");
				}else{
					entidadesDefinitivas.push_back(unaEntidadEnJuego);
				}
			}
			unEscenario.set_entidades(entidadesDefinitivas);
		}

		//verifica si las entidades no salen del mapa
		void verificar_limites_entidades (logErrores& logErrores,vector<config_entidad>& entidades, vector
			<config_escenario>& escenarios)
		{
			for (unsigned int i=0; i < escenarios.size();i++){
				//para cada escenario verificamos si las entidades y protagonistas en juego corresponden
				//con alguna entidad declarada
				config_escenario& unEscenario = escenarios[i];
				verificar_limites_un_escenario(logErrores,entidades,unEscenario);
			}
			return;
		}
};



// Para declararlo como global (se define en main)
extern logErrores err_log;