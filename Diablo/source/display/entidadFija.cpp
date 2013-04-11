#include "entidadFija.h"
#include "tile.h"
#include "mapa.h"

	/*
		Pre:-
		 
		Post: Se ha creado la instancia con los valores por defecto.
	*/
	EntidadFija::EntidadFija(){
		// seteo del puntero a imagen
		this -> imagen = NULL;
		// Se setean los atributos a sus valores por defecto.
		this -> inicializarAtributosEnValoresDefault();
	}

	/*
		Pre:-
		 
		Post: Se ha inicializado la instancia segun el archivo y los par?etros.

		NOTA: ImagenEstatica
	*/
	EntidadFija::EntidadFija(const std::string& name, 
					const unsigned int wTiles , const unsigned int hTiles ,
					int pixel_ref_x ,int pixel_ref_y,
					Tile* tile, Mapa* mapa,
					ResMan& rm , const int colorKey){
		// seteo del puntero a imagen
		this -> imagen = NULL;
		//carga de imagen y configuraci�n inicial.
		init(name , wTiles , hTiles , pixel_ref_x , pixel_ref_y, tile, mapa , rm , colorKey);
	}
		
	/*
		Pre:-
		 
		Post: Se ha inicializado la instancia segun el archivo y los par?etros.

		NOTA: ImagenAnimada
	*/
	EntidadFija::EntidadFija(const std::string& name, 
			const unsigned int wTiles , const unsigned int hTiles , 
			const unsigned int fps , const unsigned int delay , 
			const unsigned int altoSprite , const unsigned int anchoSprite ,
			const int pixel_ref_x , const int pixel_ref_y,
			Tile* tile , Mapa* mapa,
			ResMan& rm , const int colorKey){
		// seteo del puntero a imagen
		this -> imagen = NULL;
		//carga de imagen y configuraci�n inicial.
		init(name , wTiles , hTiles , fps , delay , 
			altoSprite , anchoSprite ,
			pixel_ref_x , pixel_ref_y, 
			tile, mapa, rm , colorKey);
	}
	
	/*
		Pre:-
		 
		Post: Se ha inicializado la instancia segun el archivo y los par?etros.

		NOTA: ImagenAnimada
	*/
	void EntidadFija::init(const std::string& name, 
					const unsigned int wTiles , const unsigned int hTiles , 
					const unsigned int fps , const unsigned int delay , 
					const unsigned int  altoSprite , const unsigned int anchoSprite ,
					int pixel_ref_x ,int pixel_ref_y,
					Tile* tile , Mapa* mapa,
					ResMan& rm , const int colorKey) {
		// Se destruyen imagenes previas
		if (this -> imagen != NULL) {
			delete(this -> imagen);
			this -> imagen = NULL;
		}
		// Se setean los atributos a sus valores por defecto.
		this -> inicializarAtributosEnValoresDefault();
		// Se carga la nueva imagen
		this -> imagen	= new ImagenAnimada(name.c_str() , altoSprite , 
								anchoSprite , fps , delay , rm ,colorKey);
		// Seteo de nombre 
		this -> name = name;
		// Seteo del Surface
		this -> surf = this -> imagen -> getSurface();
		//Seteo de dimensiones
		this -> highInTiles = hTiles;
		this -> widthInTiles = wTiles;
		//pixel de referencia
		this -> pixel_ref_x = pixel_ref_x ;
		this -> pixel_ref_y = pixel_ref_y;
		//agrego entidad al tile
		if (tile != NULL) {
			tile -> deleteEntidad(this);
			tile -> addEntidad(this);
			//seteo posicion
			this -> posX = tile -> getX();
			this -> posY = tile -> getY();
		}
		//tile destino
		this -> tileDestino = tile;
		//tile ancla
		this -> tileAncla = tile;
		//deltaUpdatePosicion
		this -> velocidad = VELOCIDAD_DEFAULT;
		this -> deltaUpdatePosicion = Entidad::BASE_DE_TIEMPO/velocidad;
		//tiempo siguiente update
		this -> tiempoProximoUpdate = clock();
		//seteo como compartido
		this -> compartido = true;
	}

	/*
		Pre: La instancia ha sido creada.

		Post: Se ha configurado la instancia de acuerdo a a los parametros.

		NOTA: ImagenEstatica
	*/
	void EntidadFija::init(const std::string& name,  
					const unsigned int wTiles , const unsigned int hTiles , 
					int pixel_ref_x ,int pixel_ref_y , 
					Tile* tile , Mapa* mapa,
					ResMan& rm , const int colorKey){
		// Se destruyen imagenes previas
		if (this -> imagen != NULL) {
			delete(this -> imagen);
			this -> imagen = NULL;
		}
		// Se setean los atributos a sus valores por defecto.
		this -> inicializarAtributosEnValoresDefault();
		// Se carga la nueva imagen
		this -> imagen	= new ImagenEstatica( name.c_str() , rm , colorKey);
		// Seteo de nombre
		this -> name = name;
		// Seteo del Surface
		this -> surf = this -> imagen -> getSurface();
		//Seteo de dimensiones
		this -> highInTiles = hTiles;
		this -> widthInTiles = wTiles;
		//pixel de referencia
		this -> pixel_ref_x = pixel_ref_x ;
		this -> pixel_ref_y = pixel_ref_y;
		//agrego entidad al tile
		if (tile != NULL) {
			//seteo posicion
			this -> posX = tile -> getX();
			this -> posY = tile -> getY();
		}
		this -> actualizarTileAncla(tile , mapa);
	}
	
	/*
		Pre: La instancia ha sido creada.
		 
		Post: Se ha destruido la instancia liberando los recursos asociados.
	*/
	EntidadFija::~EntidadFija() {
		if (this -> imagen != NULL) {
			delete(this -> imagen);
			this -> imagen = NULL;
		}
		this -> surf = NULL;
		this -> tileDestino = NULL;
		this -> tileAncla = NULL;
	}
	
	// --------------------------------------------------------------------------

	/*
		Se borrara despues
	*/
	void EntidadFija::mover(Tile* tileDestino) {
		
	}
		
	// Se borrar despues
	void EntidadFija::update(Mapa* mapa) {
		
	}
	
	// --------------------------------------------------------------------------

	/*
		Pre: Los par�metros cumplen las siguiente condiciones:

			dest: Surface sobre el que se quiere pintar.

			camara: Camara correspondiente.

			mapa: mapa correspondiente

			tileX , tileY : Tile sobre el 

			NOTA: Cuidado al momento de hacer updates, ya que hay entidades que 
			ocupan varios Tiles. En sintesis, un update por entidad al momento
			de pintar toda la pantalla.

		Post: Se ha pintado la entidad en el surface dest seg�n la camara y el mapa.
		Si la entidad tiene una base rectangular de un s�lo Tile se pinta sin mayores 
		cuidados.
		En cambio si la entidad tiene una base superior a un tile se realiza un tratamiento
		especial.

	*/
	void EntidadFija::blit(SDL_Surface* dest , Camara* camara , Mapa* mapa,
						const unsigned int tileX ,	const unsigned int tileY){
		if ( (this -> imagen != NULL) && (this -> surf != NULL) &&
			(camara != NULL) ) {
			if(this -> surf -> getSDL_Surface() != NULL){
				if (tileAncla != NULL) {
					// Si tileX y tileY son iguales a las posiciones
					// del ancla se imprime
					if ((this -> tileAncla -> getX() == tileX) && 
						(this -> tileAncla -> getY() == tileY)){
						int posX;
						int posY;
						posX = this -> posX - (int)(camara -> getX()) - this -> pixel_ref_x;
						posY = this -> posY - (int)(camara -> getY()) - this -> pixel_ref_y;
						this -> surf -> blit(dest , posX , posY);		
					}
				}
			}
		}
	}

	/*
		posX y posY en pixeles, es una posicion cualquiera en el mapa
	*/
	void EntidadFija::actualizarTileAncla(Tile* tile , Mapa* mapa){
		Tile* tileAnclaSiguiente = this -> obtenerTileAncla(tile , mapa);
		if (tileAnclaSiguiente != NULL){			
			if(tileAnclaSiguiente != this -> tileAncla ) {
				if (tileAncla != NULL) {
					this -> tileAncla -> deleteEntidad(this);
				}
				this -> tileAncla = tileAnclaSiguiente;
				this -> tileAncla -> addEntidad(this);
			}
		} else{
			if (tile != NULL) {
				if (this -> tileAncla != NULL) {
					this -> tileAncla -> deleteEntidad(this);
				}
				this -> tileAncla = tile;
				this -> tileAncla -> addEntidad(this);
			}
		}
	}

	void EntidadFija::setTileActual(Tile* tile) {
		
	}

	/*
		posX y posY en pixeles, es una posicion cualquiera en el mapa
	*/
	Tile* EntidadFija::obtenerTileAncla(Tile* tile , Mapa* mapa){
		Tile* retorno = NULL;
		int posImagenX = tile -> getX() + this -> imagen -> getAncho() - this -> pixel_ref_x + 
						Entidad::MARGEN_ANCLA_X;
		int posImagenY = tile -> getY() + this -> imagen -> getAlto() - this -> pixel_ref_y + 
						Entidad::MARGEN_ANCLA_Y;
		retorno = mapa -> getTilePorPixeles(posImagenX,posImagenY);

		return retorno;
	}

