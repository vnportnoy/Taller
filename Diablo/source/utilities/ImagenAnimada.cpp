#include "ImagenAnimada.h"

/**
	Pre: El parametro path es una ruta no nula y los parametros alto y ancho son
	positivos.
	
	Post: Si se logra abrir el archivo y tomar memoria, la instancia se
	iniciliza de acuerdo a la imagen dada por el path.
**/
ImagenAnimada::ImagenAnimada(ImagenAnimada* imagen){

	if (imagen != NULL){
		this -> alto = imagen -> alto;
		this -> ancho = imagen -> ancho;
		
		delay = imagen -> delay;
		fpc = imagen -> fpc;

		//seteo la distancia entre frames
		this -> deltaFrame = imagen -> deltaFrame;
	
		//calculo de tiempos
		this -> tiempoProximoFrame = imagen -> tiempoProximoFrame;
	
		//creacion de un surfaceOrigen
		this -> surfaceOrigen = imagen -> surfaceOrigen;

		this -> surfaceActual = imagen -> surfaceActual;
		
		this -> maxColumnas = imagen -> maxColumnas;

		this -> maxFilas = imagen -> maxFilas;

		this -> filaActual = imagen -> filaActual;
		this -> columnaActual = imagen -> columnaActual;
		this -> colorKey = imagen -> colorKey;
		this -> compartida = imagen -> compartida;
	}
}

/**
	Pre: Condiciones sobre la entrada:
		
		path: no nulo, caso contrario se vuelca a NULL

		altoSprite y anchoSprite: mayores iguales a cero, caso contrario se
		vuelca a default

		fps: mayor que cero, caso contario se vuelca a default

		delay: mayor o igual a cero, caso contrario se vuelca a 0.

		colorKey: cualquier entero para representar un RGB de 32 bits.

	Post: Si se logra abrir el archivo y tomar memoria, la instancia se
	inicializa de acuerdo a la imagen dada por el path.
**/
ImagenAnimada::ImagenAnimada(const char* path , const int altoSprite , const int anchoSprite , 
				const int fps , const int delay , const int colorKey){
	
	//seteo de dimensiones sprite
	if (altoSprite >= 0){
		this -> setAlto(altoSprite);
	} else{
		this -> setAlto(Imagen::ALTO_DEFAULT);
	}
	if (anchoSprite >= 0){
		this -> setAncho(anchoSprite);
	} else{
		this -> setAncho(Imagen::ANCHO_DEFAULT);
	}
	
	//seteo de tiempos de delay y fps
	if (delay >= 0) {
		this -> delay = delay;
	} else {
		this -> delay = ImagenAnimada::DELAY_DEFAULT;
	}
	if (fps > 0) {
		this -> fpc = fps /((double)CLOCKS_PER_SEC);
	} else {
		this -> fpc = ImagenAnimada::FPS_DEFAULT/((double)CLOCKS_PER_SEC);
	}
	
	//seteo la distancia entre frames
	this -> deltaFrame = this -> calcularDeltaUpdate();
	
	//calculo de tiempos
	clock_t tiempoActual = clock();
	this -> tiempoProximoFrame = tiempoActual;
	
	//creacion de un surfaceOrigen
	this -> surfaceOrigen = new Surface();
	if (this -> surfaceOrigen != NULL) {
		//lectura de imagen de origen
		if (this -> surfaceOrigen -> load(path , colorKey)){
			//calculo cantidad de filas y columnas
			if (this-> getAncho() <= this -> surfaceOrigen -> width()) {
				this -> maxColumnas = this -> surfaceOrigen -> width() / this -> getAncho();
			} else {
				this -> setAncho(this -> surfaceOrigen -> width());
				this -> maxColumnas = 1;
			}
			if (this-> getAlto() <= this -> surfaceOrigen -> height()) {
				this -> maxFilas = this -> surfaceOrigen -> height() / this -> getAlto();
			} else {
				this -> setAlto(this -> surfaceOrigen -> height());
				this -> maxFilas = 1;
			}		
		} else {
			this -> maxFilas = 0;
			this -> maxColumnas = 0;
		}	
	}else {
		this -> maxFilas = 0;
		this -> maxColumnas = 0;
	}
	this -> filaActual = 0;
	this -> columnaActual = -1;
	this -> colorKey = colorKey;
	this -> compartida = false;
}

/**
	Pre: Condiciones sobre la entrada:

		altoSprite y anchoSprite: mayores iguales a cero, caso contrario se
		vuelca a default

		fps: mayor que cero, caso contario se vuelca a default

		delay: mayor o igual a cero, caso contrario se vuelca a 0.

		colorKey: cualquier entero para representar un RGB de 32 bits.

	Post: Si se logra abrir el archivo y tomar memoria, la instancia se
	inicializa de acuerdo a la imagen dada por el path.
**/
ImagenAnimada::ImagenAnimada(const char* name , const int altoSprite , 
				const int anchoSprite , const int fps , const int delay , ResMan& rm , 
				const int colorKey){
	
	//seteo de dimensiones sprite
	if (altoSprite >= 0){
		this -> setAlto(altoSprite);
	} else{
		this -> setAlto(Imagen::ALTO_DEFAULT);
	}
	if (anchoSprite >= 0){
		this -> setAncho(anchoSprite);
	} else{
		this -> setAncho(Imagen::ANCHO_DEFAULT);
	}
	
	//seteo de tiempos de delay y fps
	if (delay >= 0) {
		this -> delay = delay;
	} else {
		this -> delay = ImagenAnimada::DELAY_DEFAULT;
	}
	if (fps > 0) {
		this -> fpc = fps/((double)CLOCKS_PER_SEC);
	} else {
		this -> fpc = ImagenAnimada::FPS_DEFAULT/((double)CLOCKS_PER_SEC);
	}
	
	//seteo la distancia entre frames
	this -> deltaFrame = this -> calcularDeltaUpdate();
	
	//calculo de tiempos
	clock_t tiempoActual = clock();
	this -> tiempoProximoFrame = tiempoActual;
	
	//creacion de un surfaceOrigen
	this -> surfaceOrigen = new Surface();
	if (this -> surfaceOrigen != NULL) {
		this -> surfaceOrigen = rm.getRes(name);
		//lectura de imagen de origen
		if (this ->surfaceOrigen != NULL){
			//calculo cantidad de filas y columnas
			if (this-> getAncho() <= this -> surfaceOrigen -> width()) {
				this -> maxColumnas = this -> surfaceOrigen -> width() / this -> getAncho();
			} else {
				this -> setAncho(this -> surfaceOrigen -> width());
				this -> maxColumnas = 1;
			}
			if (this-> getAlto() <= this -> surfaceOrigen -> height()) {
				this -> maxFilas = this -> surfaceOrigen -> height() / this -> getAlto();
			} else {
				this -> setAlto(this -> surfaceOrigen -> height());
				this -> maxFilas = 1;
			}		
		} else {
			this -> maxFilas = 0;
			this -> maxColumnas = 0;
		}	
	}else {
		this -> maxFilas = 0;
		this -> maxColumnas = 0;
	}
	this -> filaActual = 0;
	this -> columnaActual = -1;
	this -> colorKey = colorKey;
	this -> compartida = true;
}

/**
	Pre: La instancia ha sido creada.

	Post: Se liberan los recursos asociados a la instancia.
**/
ImagenAnimada::~ImagenAnimada() {
	if (!(this -> compartida)) {	
		this -> surfaceOrigen -> destroy();
		delete(this -> surfaceOrigen);
		this -> surfaceOrigen = NULL;
	}
	this -> surfaceActual.destroy();
}

/*
	Pre: Se han inicilizado todos los parametros.

	Post: Se ha actualizado surfaceActual
*/
void ImagenAnimada::nextSprite() {
	clock_t tiempoActual = clock();
	if (this -> tiempoProximoFrame <= tiempoActual) {
		this -> surfaceActual.nuevoSurfaceConfigurado(this -> getAlto() ,
					this -> getAncho() , SDL_GetVideoInfo() , colorKey);
		if ((this -> maxColumnas > 0) && (this -> maxFilas > 0) ) {
			clock_t deltaTiempo = tiempoActual - this -> tiempoProximoFrame + deltaFrame;
			this -> columnaActual = this -> columnaActual +  ceil((double)deltaTiempo * fpc);
			this -> tiempoProximoFrame = tiempoActual + this -> deltaFrame;
			if (this -> columnaActual >= this -> maxColumnas) {
				this -> columnaActual = 0;
				this -> filaActual++;
				if (this -> filaActual >= this -> maxFilas) {
					this -> filaActual = 0;
				}
				this -> tiempoProximoFrame += this -> delay;
			}
			SDL_Rect rect;
			rect.h = this -> getAlto();
			rect.w = this ->getAncho();
			rect.x = this -> columnaActual * this -> getAncho();
			rect.y = this -> filaActual * this -> getAlto();
			if (this -> surfaceOrigen != NULL){
				this -> surfaceOrigen-> blitGris( (this -> surfaceActual).getSDL_SurfaceGris() , 
												0 , 0, rect);				
				this -> surfaceOrigen-> blit( (this -> surfaceActual).getSDL_Surface() , 
												0 , 0, rect);
			}
		}
	}
}

/**
	Pre: La instancia ha sido creada.

	Post: Se retorna un puntero a una imagen en memoria de video, cuya destruccion
	es responsabilidad de esta instancia.
		
**/
Surface* ImagenAnimada::getSurface() {
	this -> nextSprite();
	return & (this -> surfaceActual);
}


/**
	Pre: La instancia ha sido creada.

	Post: Se retorna la cantidad de FPS.
		
**/
int ImagenAnimada::getFPS() {
	return (this -> fpc*((double)CLOCKS_PER_SEC));
}

/**
	Pre: La instancia ha sido creada.

	Post: Se retorna el Delay.
		
**/
int ImagenAnimada::getDelay() {
	return this -> delay;
}

bool ImagenAnimada::isImagenAnimada(){
	return true;
}

/*
	Pre: -

	Post: Se ha actualizado surfaceActual
*/
clock_t ImagenAnimada::calcularDeltaUpdate(){
	clock_t deltaUpdateCalculado = 0;
	double deltaCuadro = 0;
	while (deltaCuadro < 1){
		deltaCuadro = deltaUpdateCalculado * fpc;
		deltaUpdateCalculado++;
	}
	return deltaUpdateCalculado;
}