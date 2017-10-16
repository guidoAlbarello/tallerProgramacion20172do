#include "EstadoMenu.h"

const std::string MenuState::s_menuID = "MENU";
void MenuState::update() {
	// rellenar
}
void MenuState::render() {
	// rellenar
}
bool MenuState::onEnter(Renderer* renderer) {
	//iniciarMenu
	return true;
}
bool MenuState::onExit() {
	//cerrarmenu
	return true;
}