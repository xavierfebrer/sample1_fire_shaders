#include "../../../../include/EndScreen.h"

EndScreen::EndScreen(Game & game)
	: BaseScreen(game) {
}

EndScreen::~EndScreen() {
}

void EndScreen::create() {
	BaseScreen::create();
}

void EndScreen::initViews() {
}

void EndScreen::resume() {
	BaseScreen::resume();
}

void EndScreen::update(double deltaTime) {
	BaseScreen::update(deltaTime);
}

void EndScreen::render(double deltaTime) {
	BaseScreen::render(deltaTime);
}

void EndScreen::pause() {
	BaseScreen::pause();
}

void EndScreen::openMainMenuScreen() {
	game.setScreen(std::make_unique<MainMenuScreen>(game));
}
