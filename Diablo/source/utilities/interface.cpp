#include "interface.h"
#include "../net/PjeManager.h"
#include "aux_func.h"
#include <sstream>
extern PjeManager pjm;
extern ResMan resman;

void Interface::blitInterface(SDL_Surface* screen) {
	int x,y,altura;
	altura=12;
	x=5;
	y=5;
	std::stringstream ss1;
	ss1 << "Energia: " << (int)pjm.getPjeLocal().getEnergia();
	resman.getFont()->blit(screen, x, y, ss1.str(), COLOR::WHITE);
	std::stringstream ss2;
	y+=altura;
	ss2 << "Escudo: " << (int)pjm.getPjeLocal().getEnergiaEscudo();
	resman.getFont()->blit(screen, x, y, ss2.str(), COLOR::WHITE);
	std::stringstream ss3;
	y+=altura;
	ss3 << "Magia: " << (int)pjm.getPjeLocal().getMagia();
	resman.getFont()->blit(screen, x, y, ss3.str(), COLOR::WHITE);
	std::stringstream ss4;
	y+=altura;
	ss4 << "Velocidad: " << pjm.getPjeLocal().getVelocidad();
	resman.getFont()->blit(screen, x, y, ss4.str(), COLOR::WHITE);
	std::stringstream ss5;
	y+=altura;
	ss5 << "Terremotos: " << (int)pjm.getPjeLocal().getTerremoto();
	resman.getFont()->blit(screen, x, y, ss5.str(), COLOR::WHITE);
	std::stringstream ss6;
	y+=altura;
	ss6 << "Hielos: " << (int)pjm.getPjeLocal().getHielo();
	resman.getFont()->blit(screen, x, y, ss6.str(), COLOR::WHITE);
}