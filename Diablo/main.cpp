// SDL Headers
#include "SDL.h"
// Our headers
#include "source/utilities/timer.h"
#include "source/utilities/surface.h"
#include "source/utilities/aux_func.h"
#include "source/utilities/PruebasParser.h"
#include "source/utilities/parser.h"
#include "source/display/entidad.h"
#include "source/utilities/Personaje.h"
#include "source/display/entidadFija.h"
#include "source/display/camara.h"
#include "source/display/mapa.h"
#include "source/display/resman.h"
#include "source/constants/model.h"
#include "source/utilities/Test.h"
#include "source/utilities/coordenadas.h"

int main(int argc, char* args[]) {

	//Prueba parser YAML
	//parser_test();
	
	//parseo un nivel
	config_juego juego = parsear("../resources/levels/nivel1.yaml");
	config_pantalla* pantalla = juego.get_pantalla();
	vector <config_entidad> entidades = juego.get_entidades();
	config_general configuracion = juego.get_configuracion();
	vector <config_escenario> escenarios = juego.get_escenarios();



	system("PAUSE");
	// Ventana de prueba
	SDL_Surface* screen;
    SDL_Init(SDL_INIT_EVERYTHING);
	// Para confinar el mouse a la ventana
	//SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_WarpMouse(800/2, 600/2);
	// Init the window
	screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
	// Camara
	Camara camara;
	//camara.init(800, 600, 50);
	camara.init(pantalla->get_ancho(), pantalla->get_alto(), 50);

	// Mapa
	ResMan resman;
	resman.init();

	std::vector<Entidad*> entidades_cargadas;
	for (auto it = entidades.begin(); it != entidades.end(); ++it){
		resman.addRes((*it).get_nombre(), (*it).get_path_imagen(), Imagen::COLOR_KEY);
		Entidad entidad;
		if((*it).get_fps() == -1){
			entidad = Entidad ((*it).get_nombre(), (*it).get_ancho_base(), (*it).get_alto_base(), (*it).get_pixel_ref_x(), (*it).get_pixel_ref_y(),
								NULL, resman, Imagen::COLOR_KEY);
		}else{
			entidad = Entidad ((*it).get_nombre(), (*it).get_ancho_base(), (*it).get_alto_base(), (*it).get_fps(), (*it).get_delay(),
								(*it).get_pixel_ref_x(), (*it).get_pixel_ref_y(), NULL, resman, Imagen::COLOR_KEY);		
		}
		entidades_cargadas.push_back(&entidad);
	}

	/*
	resman.addRes("tierra", "../resources/tile___ERROR_TEST.bmp", 255);
	resman.addRes("cemento", "../resources/tile2.bmp", 255);
	resman.addRes("agua", "../resources/tileAgua.bmp", Imagen::COLOR_KEY);
	resman.addRes("soldado", "../resources/Soldado.bmp", Imagen::COLOR_KEY);
	resman.addRes("casa", "../resources/Casa400x400.bmp", Imagen::COLOR_KEY);
	*/

/*
	Entidad tierra_test("tierra", 1 , 1 , 
							   0 , 0 , 
							   NULL,
							   resman , Surface::RGB_VERDE);
	Entidad cem_test("cemento", 1 , 1 , 
							 0 , 0 , 
							  NULL,
							  resman , Surface::RGB_VERDE);
	Entidad agua("agua" ,
							1 , 1 , 
							5 , 300, 
							50, 100 ,
							0 , 0 , 
							NULL , resman ,
							Imagen::COLOR_KEY);
	Personaje personaje("soldado" ,
							1 , 1 , 
							50 , 5, 
							100, 100 ,
							20,
							0 , 70 ,
							NULL , resman ,
							Imagen::COLOR_KEY);
*/
	Entidad entidadDefecto();

	Mapa mapa;
	// Mapa de size random
	mapa.resize(escenarios[0].get_tam_x(), escenarios[0].get_tam_x());
	vector<config_entidad_en_juego> entidades_en_juego = escenarios[0].get_entidades();
	// Llenamos el mapa con entidad tierra
	
	for(auto it = entidades_en_juego.begin(); it != entidades_en_juego.end(); ++it){
		Entidad entidad_elegida;
		for(auto it2 = entidades_cargadas.begin(); it2 != entidades_cargadas.end(); ++it2){
			if((*it).get_nombre() == (*it2)->get_nombre()){
				mapa.getTile((*it).get_pos_x(), (*it).get_pos_y())->addEntidad(*it2);		
			}
		}
	}

	for(auto it = mapa.allTiles().begin();it != mapa.allTiles().end(); ++it) {
		if((*it).get_entidades().size() == 0) {
			it->addEntidad((Entidad*)&entidadDefecto);
		}
	}

/*	mapa.getTile(1,1) ->addEntidad(&personaje);
	EntidadFija casa("casa", 4 , 4 , 
							 140 , 190 , 
							 mapa.getTile(10,10) , &mapa ,
							 resman , Imagen::COLOR_KEY);
	// Aca muestra como se agregan a mano
	//mapa.getTile(0, 0).addEntidad(cem_test);
	//mapa.getTile(0, 1).addEntidad(cem_test);
	//mapa.getTile(1, 0).addEntidad(tierra_test);
*/
	double curr_time = SDL_GetTicks();
    double accum = 0.0;
	bool quit = false;

	// Para guardar los eventos
	SDL_Event event;

	vec2<int> tw_test;

	while(!quit) {

		// Input handling (esto despues se movera a donde corresponda)
		while(SDL_PollEvent(&event)) {
			// Detectar escape o quit
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT) {
				quit = true;
			}
			// Para probar el tilewalk
			vec2<int> test_next(-1, -1);
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_KP2) {
				test_next = tileWalk(tw_test, GDIR::S);
			}
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_KP8) {
				test_next = tileWalk(tw_test, GDIR::N);
			}
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_KP4) {
				test_next = tileWalk(tw_test, GDIR::E);
			}
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_KP6) {
				test_next = tileWalk(tw_test, GDIR::O);
			}
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_KP7) {
				test_next = tileWalk(tw_test, GDIR::NE);
			}
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_KP9) {
				test_next = tileWalk(tw_test, GDIR::NO);
			}
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_KP1) {
				test_next = tileWalk(tw_test, GDIR::SE);
			}
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_KP3) {
				test_next = tileWalk(tw_test, GDIR::SO);
			}
			// Hacer el movimiento si es valido
			if(mapa.tileExists(test_next.x, test_next.y)) {
				mapa.getTile(tw_test.x, tw_test.y)->clean();
				mapa.getTile(tw_test.x, tw_test.y)->addEntidad((Entidad*)&entidadDefecto);
				mapa.getTile(test_next.x, test_next.y)->clean();
				mapa.getTile(test_next.x, test_next.y)->addEntidad((Entidad*)&entidadDefecto);
				tw_test = test_next;
			}
			// Detectar mouse motion
			if(event.type == SDL_MOUSEMOTION) {
				// Update para la camara
				camara.update_speed(makeRect(event.motion.x, event.motion.y));
			}
			// Mouse clicks
			if(event.type == SDL_MOUSEBUTTONDOWN) {
				if(event.button.button == SDL_BUTTON_LEFT) {
					vec2<int> tile_res = MouseCoords2Tile(vec2<int>(event.button.x, event.button.y), camara);
					std::cout << event.button.x << ";" << event.button.y <<  " || Tile: " << tile_res.x << ";" << tile_res.y << "\n";
					if(mapa.tileExists(tile_res.x, tile_res.y)) {
						//mapa.getTile(tile_res.x, tile_res.y)->clean();
						//mapa.getTile(tile_res.x, tile_res.y)->addEntidad(&cem_test);
						personaje.mover(mapa.getTile(tile_res.x,tile_res.y));
					}
				}
			}
		}

		// Cuenterio para hacer el timestep (CONST_DT) independiente de los FPS
		// (ver http://gafferongames.com/game-physics/fix-your-timestep/)
		double new_t = SDL_GetTicks();
		double frame_time = new_t - curr_time;
		if(frame_time > 250) {
			// note: max frame time to avoid spiral of death
			frame_time = 250; 
		}
		curr_time = new_t;
		accum += frame_time;

		// Aca se hace el timestep, aka avanzar la fisica usando Euler en un delta t fijo
		while(accum >= CONST_DT) {
			
			camara.update();

			accum -= CONST_DT;
		}

		// Aca se hace el blitteo
		// Despues se movera a donde corresponda
		SDL_FillRect(screen, NULL, 0);
		// Draw el mapa
		mapa.blit(screen, camara);
		// Actualizar la pantalla
		SDL_Flip(screen);
		//ACtualizacion entidades
		agua.update(&mapa);
		personaje.update(&mapa);
	}
	
	mapa.clean();
	resman.clean();

    Test::test();

	SDL_Quit();
	 
    return 0;    
}