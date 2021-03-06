#pragma once
#include "parserServer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "config_servidor.h"
#include "logErroresServer.h"
using namespace std;
const int errorConstante = -1;
//VARIABLES GLOBALES
//lee e imprime un archivo
/*void imprimir_documento(char* path){
		//se puede mejorar pero es solo una clase de test
		cout << endl << "Contenido del archivo:" << path << endl;
		char cadena[100];
		std::ifstream archivo(path);
		if (archivo.good()){
		
			while (!archivo.eof()){
				archivo >> cadena;
				cout<< cadena <<endl;
			}

		}
		archivo.close();
		return;
}*/
//VARIABLES GLOBALES



//lee e imprime un archivo
void imprimir_documento(char* path){
		//se puede mejorar pero es solo una clase de test
		cout << endl << "Contenido del archivo:" << path << endl;
		char cadena[100];
		std::ifstream archivo(path);
		if (archivo.good()){
		
			while (!archivo.eof()){
				archivo >> cadena;
				cout<< cadena <<endl;
			}

		}
		archivo.close();
		return;
}
void operator >> (const YAML::Node& node, config_general& config) {

   for(unsigned i=0;i<node.size();i++) {
      	for(YAML::Iterator it=node[i].begin();it!=node[i].end();++it) {
			//leo los atributos de la configuracion
			string clave,valor;
		
			it.first() >> clave;
			it.second() >> valor;
			//y los asigno
			if (clave == "vel_personaje"){
				config.completo_velocidad();
				if( err_log.esNumerico(valor) ){
					config.set_vel_personaje(atoi(valor.c_str()));
				}else{
					config.set_vel_personaje(errorConstante);
				}
			}else if (clave == "margen_scroll"){
				config.completo_Margen();
				if( err_log.esNumerico(valor) ){
					config.set_margen_scroll(atoi(valor.c_str()));
				}else{
					config.set_vel_personaje(errorConstante);
				}
			}else {
				err_log.escribir("atributo de configuracion erroeneo",clave,valor);
			}
		}
   }

}
void operator >> (const YAML::Node& node, config_pantalla& config) {

   for(unsigned i=0;i<node.size();i++) {
      	for(YAML::Iterator it=node[i].begin();it!=node[i].end();++it) {
			//leo los atributos de pantalla
			string clave;
			string valor;
			it.first() >> clave;
			it.second() >> valor;
			//y los asigno
			if (clave == "ancho"){
				//actualizo atributos
				config.completo_Ancho();
				if( err_log.esNumerico(valor) ){
					config.set_ancho(atoi(valor.c_str()));
				}else{
					config.set_ancho(errorConstante);
				}
			}else if (clave == "alto"){
				//actualizo atributos
				config.completo_Alto();
				if( err_log.esNumerico(valor) ){
					config.set_alto(atoi(valor.c_str()));
				}else{
					config.set_alto(errorConstante);
				}

			}else {
				err_log.escribir("atributo de pantalla erroeneo",clave , valor);
			}
		}
   }

}
bool ConvertirABool(string valor){
	//se transforma el valor a boolean
	bool res;
	if (valor == "true"){
		res = true;
	}else if(valor == "false"){
		res = false;;		
	}
	return res;
}
void operator >> (const YAML::Node& node, vector <config_entidad>& entidades) {
   //aca itero cada entidad	
    for(unsigned i=0;i<node.size();i++) {
	    config_entidad nuevaEntidad("","",-1,-1,-1,-1,-1,-1);
		bool creoEntidad = false; //verifica si se ingreso algun parametro valido
		//aca itero dentro de la entidad
		for(YAML::Iterator it=node[i].begin();it!=node[i].end();++it) {
			//leo los atributos de entidad
			string clave;
			string valor;
			it.first() >> clave;
			it.second() >> valor;
			//actualizo atributos
			//y los asigno
			if (clave == "nombre"){
				creoEntidad = true;
				nuevaEntidad.completo_nombre();
				nuevaEntidad.set_nombre(valor); 
			}else if (clave == "imagen"){
				creoEntidad = true;
				nuevaEntidad.completo_path();
				nuevaEntidad.set_path_imagen(valor);
			}else if (clave == "ancho_base"){
				creoEntidad = true;
				nuevaEntidad.completo_Ancho();
				if( err_log.esNumerico(valor) ){
					nuevaEntidad.set_ancho_base(atoi(valor.c_str()));
				}else{
					nuevaEntidad.set_ancho_base(errorConstante);
				}
			}else if (clave == "alto_base"){
				creoEntidad = true;
				nuevaEntidad.completo_Alto();
				if( err_log.esNumerico(valor) ){
					nuevaEntidad.set_alto_base(atoi(valor.c_str()));
				}else{
					nuevaEntidad.set_alto_base(errorConstante);
				}
			}else if (clave == "pixel_ref_x"){
				creoEntidad = true;
				nuevaEntidad.completo_pixelX();
				if( err_log.esNumerico(valor) ){
					nuevaEntidad.set_pixel_ref_x(atoi(valor.c_str()));
				}else{
					nuevaEntidad.set_pixel_ref_x(errorConstante);
				}
				nuevaEntidad.set_pixel_ref_x(atoi(valor.c_str()));
			}else if (clave == "pixel_ref_y"){
				creoEntidad = true;
				nuevaEntidad.completo_pixelY();
				if( err_log.esNumerico(valor) ){
					nuevaEntidad.set_pixel_ref_y(atoi(valor.c_str()));
				}else{
					nuevaEntidad.set_pixel_ref_y(errorConstante);
				}
			}else if (clave == "esDefault"){
				creoEntidad = true;
				bool value=false;
				if (valor=="true") {
					value=true;
				}
				nuevaEntidad.set_es_default(value);
			}else if (clave == "fps"){
				creoEntidad = true;
				nuevaEntidad.completo_fps();
				if( err_log.esNumerico(valor) ){
					nuevaEntidad.set_fps(atoi(valor.c_str()));
				}else{
					nuevaEntidad.set_fps(errorConstante);
				}
				nuevaEntidad.set_fps(atoi(valor.c_str()));
			}else if (clave == "delay"){
				creoEntidad = true;
				nuevaEntidad.completo_delay();
				if( err_log.esNumerico(valor) ){
					nuevaEntidad.set_delay(atoi(valor.c_str()));;
				}else{
					nuevaEntidad.set_delay(errorConstante);
				}
			}else if (clave == "alto_sprite"){
				creoEntidad = true;
				nuevaEntidad.completo_alto_sprite();
				if( err_log.esNumerico(valor) ){
					nuevaEntidad.set_alto_sprite(atoi(valor.c_str()));
				}else{
					nuevaEntidad.set_alto_sprite(errorConstante);
				}

			}else if (clave == "ancho_sprite"){
				creoEntidad = true;
				nuevaEntidad.completo_ancho_sprite();
				if( err_log.esNumerico(valor) ){
					nuevaEntidad.set_ancho_sprite(atoi(valor.c_str()));
				}else{
					nuevaEntidad.set_ancho_sprite(errorConstante);
				}
 			}else if (clave == "caminable"){
				if ( (valor == "true") || (valor == "false" )){
					bool caminable_Value = ConvertirABool(valor);
					creoEntidad = true;
					nuevaEntidad.completo_caminable();
					nuevaEntidad.set_caminable(caminable_Value);
				}else{
					err_log.escribir("Atributo caminable de " + nuevaEntidad.get_nombre() + " erroneo");
				}
			}else {
				err_log.escribir("atributo de entidad erroeneo",clave,valor);
			}
		}
		if (creoEntidad){
			//se ingreso por lo menos un atributo correcto
			entidades.push_back(nuevaEntidad);
		} else {
			//se ingreso una entidad con todos los atributos erroneos
				err_log.escribir("Se ingreso una entidad que no contenia ningun atributo valido. Se elimino la misma" );
		}
	}

}
void parsear_protagonistas(const YAML::Node& node, config_escenario& unEscenario){
	//aca itero por cada protagonista
    for(unsigned i=0;i<node.size();i++) {
		config_entidad_en_juego unProtagonista("",-1,-1);
		bool creoEntidad = false; //verifica si se ingreso algun parametro valido
		//aca itero dentro de un protagonista
		for(YAML::Iterator it=node[i].begin();it!=node[i].end();++it) {
			//leo los atributos del protagonista
			string clave;
			string valor;
			it.first() >> clave;
			it.second() >> valor;
			//y los asigno
			if (clave == "tipoEntidad"){
				creoEntidad = true;
				unProtagonista.set_nombre(valor); 
			}else if (clave == "x"){
				creoEntidad = true;
				if( err_log.esNumerico(valor) ){
					unProtagonista.set_pos_x(atoi(valor.c_str()));
				}else{
					unProtagonista.set_pos_x(errorConstante);
				}
			}else if (clave == "y"){
				creoEntidad = true;
				if( err_log.esNumerico(valor) ){
					unProtagonista.set_pos_y(atoi(valor.c_str()));
				}else{
					unProtagonista.set_pos_y(errorConstante);
				}
			}else {
				err_log.escribir( "atributo de protagonista erroeneo",clave,valor );
			}
		}

		if (creoEntidad){
			//se ingreso por lo menos un atributo correcto
			//agrego un protagonista al escenario
			unEscenario.agregar_protagonista(unProtagonista);
		} else {
			//se ingreso un protagonista con todos los atributos erroneos
			err_log.escribir("Se ingreso un protagonista en el escenario que no contenia ningun atributo valido. Se elimino la misma" );
		}
   }
}
void parsear_entidades(const YAML::Node& node, config_escenario& unEscenario){
	//aca itero por cada entidad
    for(unsigned i=0;i<node.size();i++) {
		config_entidad_en_juego unaEntidad("",-1,-1);
		bool creoEntidad = false; //verifica si se ingreso algun parametro valido
		//aca itero dentro de una entidad
		for(YAML::Iterator it=node[i].begin();it!=node[i].end();++it) {
			//leo los atributos del protagonista
			string clave;
			string valor;
			it.first() >> clave;
			it.second() >> valor;
			//y los asigno
			if (clave == "entidad"){
				creoEntidad = true;
				unaEntidad.set_nombre(valor); 
			}else if (clave == "x"){
				creoEntidad = true;
				if( err_log.esNumerico(valor) ){
					unaEntidad.set_pos_x( atoi(valor.c_str()) );
				}else{
					unaEntidad.set_pos_x(errorConstante);
				}
			}else if (clave == "y"){
				creoEntidad = true;
				if( err_log.esNumerico(valor) ){
					unaEntidad.set_pos_y( atoi(valor.c_str()) );
				}else{
					unaEntidad.set_pos_y(errorConstante);
				}
			}else {
				err_log.escribir( "atributo de entidad en escenario erroeneo",clave,valor );
			}
		}
		if (creoEntidad){
			//se ingreso por lo menos un atributo correcto
			//agrego una entidad al escenario
			unEscenario.agregar_entidad(unaEntidad);
		} else {
			//se ingreso un protagonista con todos los atributos erroneos
			err_log.escribir("Se ingreso una entidad en el escenario que no contenia ningun atributo valido. Se elimino la misma" );
		}
   }
}
void operator >> (const YAML::Node& node, vector <config_escenario>& escenarios) {
    //aca itero para cada escenario	
    for(unsigned i=0;i<node.size();i++) {
		//aca itero dentro del escenario
		config_escenario unEscenario("",-1,-1);
		for(YAML::Iterator it=node[i].begin();it!=node[i].end();++it) {
			string clave;
			string valor;
			it.first() >> clave;

			//guardo los atributos
			if ( clave == "nombre" ){
				it.second() >> valor;
				unEscenario.set_nombre(valor);
			} else if (clave == "size_x"){
				it.second() >> valor;
				if( err_log.esNumerico(valor) ){
					unEscenario.set_tam_x(atoi(valor.c_str()));
				}else{
					unEscenario.set_tam_x(errorConstante);
				}
			} else if (clave == "size_y"){
				it.second() >> valor;
				if( err_log.esNumerico(valor) ){
					unEscenario.set_tam_y(atoi(valor.c_str()));
				}else{
					unEscenario.set_tam_y(errorConstante);
				}
			} else if (clave == "protagonista"){
				parsear_protagonistas(it.second(),unEscenario);
			}else if (clave == "entidadesDef"){
				parsear_entidades(it.second(),unEscenario);
			}else{ 
				it.second() >> valor;
				err_log.escribir( "atributo de escenario erroeneo",clave,valor ); 
			}
		}
		//agrego un escenario
		escenarios.push_back(unEscenario);
    }
}

//verifica que no se haya enviado algun nodo ppal incorrecto
void verificar_tags_ppales(YAML::Node& nodoPpal){
	for(YAML::Iterator it=nodoPpal.begin();it!=nodoPpal.end();++it) {
		std::string clave, valor;
		it.first() >> clave;
		if ( (clave != "pantalla") && (clave != "configuracion") &&  (clave != "escenarios") && (clave != "entidades") ){
			err_log.escribir( "Se ingreso nodo principal erroneo ", clave );
		}
	}
	return;
}

void operator >> (const YAML::Node& node, config_servidor& config) {

   for(unsigned i=0;i<node.size();i++) {
      	for(YAML::Iterator it=node[i].begin();it!=node[i].end();++it) {
			//leo los atributos de pantalla
			string clave;
			string valor;
			it.first() >> clave;
			it.second() >> valor;
			//y los asigno
			if (clave == "puerto"){
				//actualizo atributos
				if( err_log.esNumerico(valor) ){
					config.set_puerto(atoi(valor.c_str()));
				}else{
					config.set_puerto(errorConstante);
				}

			}else {
				err_log.escribir("atributo de red erroeneo",clave , valor);
			}
		}
   }

}
//verifica que no se haya enviado algun nodo ppal incorrecto
void server_verificar_tags_ppales_red(YAML::Node& nodoPpal){
	for(YAML::Iterator it=nodoPpal.begin();it!=nodoPpal.end();++it) {
		std::string clave, valor;
		it.first() >> clave;
		if ( (clave != "red")){
			err_log.escribir( "Se ingreso nodo principal erroneo ", clave );
		}
	}
	return;
}

config_servidor parsear_red_server(char* path){
	//ejemplos basado en: https://code.google.com/p/yaml-cpp/wiki/HowToParseADocument
	//declaro variables resultado
	config_servidor configuracion(8080);

	try{
		//abro el documento y parseo el nodo
		std::ifstream archivo(path);
		YAML::Parser parser(archivo);
		YAML::Node doc;
		parser.GetNextDocument(doc);
	
		//parseo configuracion de red
		if(const YAML::Node *pName = doc.FindValue("red")) {
			doc["red"] >> configuracion;
		}else{
			err_log.escribir("No existe el valor red para conectarse con el server");
		}
		server_verificar_tags_ppales_red(doc);
		err_log.verificar_errores(configuracion,err_log);
	} catch(exception& ){
		//dejo en un estado valido si fallo
		err_log.verificar_errores(configuracion,err_log);
	}
	return configuracion;
}
config_juego parser_nivel(char* path){


	//abro el documento y parseo el nodo
	std::ifstream archivo(path);
	YAML::Parser parser(archivo);
	YAML::Node doc;
	parser.GetNextDocument(doc);
	//declaro variables resultado
	config_juego juego;
	config_pantalla pantalla(-1,-1);
	vector <config_entidad> entidades;
	config_general config(-1,-1);
	vector <config_escenario> escenarios;

	//parseo pantalla
	if(const YAML::Node *pName = doc.FindValue("pantalla")) {
		doc["pantalla"] >> pantalla;
	}else{
		err_log.escribir("No existe el valor pantalla");
	}
	//parseo escenarios
	if(const YAML::Node *pName = doc.FindValue("escenarios")) {
		doc["escenarios"] >> escenarios;
	}else{
		err_log.escribir("No existe el valor escenarios");
	}
	//parseo entidades
	if(const YAML::Node *pName = doc.FindValue("entidades")) {
		doc["entidades"] >> entidades;
	}else{
		err_log.escribir( "No existe el valor entidades." );
	}
	//parseo configuracion
	if(const YAML::Node *pName = doc.FindValue("configuracion")) {
		doc["configuracion"] >> config;
	}else{
		err_log.escribir("No existe el valor configuracion" );
	}

	//verificar errores 
	verificar_tags_ppales(doc);
	err_log.verificar_errores(pantalla,err_log);//visto
	err_log.verificar_errores(config,err_log);//visto
	err_log.verificar_errores(entidades,err_log);//visto
	err_log.verificar_errores(escenarios, err_log, entidades);//visto
	err_log.verificar_unicidad_entidades (entidades, err_log);//visto
	err_log.verificar_correspondencia_escenario(err_log,entidades, escenarios);//visto
	err_log.verificar_limites_entidades(err_log,entidades,escenarios);
	//asigno atributos al juego
 	juego.set_pantalla(pantalla);
	juego.set_escenarios(escenarios);
	juego.set_entidades(entidades);
	juego.set_configuracion(config);

	//cerramos la conexion
	archivo.close();

	return juego;
}

config_juego parsear_juego(char* path){
	//ejemplos basado en: https://code.google.com/p/yaml-cpp/wiki/HowToParseADocument
	
	config_juego juego;
	try{
		juego = parser_nivel(path);
	}catch (exception& e){
		err_log.escribir("YAML exception catched. El archivo de configuracion no es YAML valido." );
		err_log.escribir(e.what());
		
		//verifico que si hubo un error y no se que se lleno, los datos sean validos y se inicie el juego igual
		//verificar errores 
		config_pantalla pantalla;
		vector <config_entidad> entidades;
		config_general config(-1,-1);
		vector <config_escenario> escenarios;
		err_log.verificar_errores(pantalla,err_log);
		err_log.verificar_errores(config,err_log);
		err_log.verificar_errores(entidades,err_log);
		err_log.verificar_errores(escenarios, err_log, entidades);
		err_log.verificar_unicidad_entidades (entidades, err_log);
		err_log.verificar_correspondencia_escenario(err_log,entidades, escenarios);
		err_log.verificar_limites_entidades(err_log,entidades,escenarios);
		//asigno atributos al juego
 		juego.set_pantalla(pantalla);
		juego.set_escenarios(escenarios);
		juego.set_entidades(entidades);
		juego.set_configuracion(config);
	}

	return juego;
}
