#include "../../include/Sample1FireShadersGame.h"
#include "../../include/LogoScreen.h"
#include "../../../core/include/Hack3dEngine.h"

int Sample1FireShadersGame::execute() {
	Sample1FireShadersGame game;
	if (!game.create()) {
		return -1;
	}
	game.loop();
	return 0;
}

Sample1FireShadersGame::Sample1FireShadersGame()
	: Game(),
	initialized(false), running(false), ended(false), currentScreen(nullptr), newScreen(nullptr) {
}

Sample1FireShadersGame::~Sample1FireShadersGame() {
	if (newScreen) {
		newScreen.reset();
	}
	if (currentScreen) {
		currentScreen->pause();
		currentScreen.reset();
	}
	Hack3dEngine::destroy();
}

bool Sample1FireShadersGame::create() {
	return init() && waitForReadyState() && startGame();
}

bool Sample1FireShadersGame::init() {
	if (!initialized) {
		initialized = false;
		running = false;
		ended = false;
		if (!Hack3dEngine::get()->init()) {
			return false;
		}
		initialized = true;
	}
	return initialized;
}

bool Sample1FireShadersGame::waitForReadyState() {
	Hack3dEngineReadiness engineReady;
	bool allReady = false;
	while (!allReady) {
		engineReady = Hack3dEngine::get()->isReady();
		if (engineReady == Hack3dEngineReadiness::ERROR_GENERIC
			|| engineReady == Hack3dEngineReadiness::ERROR_INPUT
			|| engineReady == Hack3dEngineReadiness::ERROR_AUDIO
			|| engineReady == Hack3dEngineReadiness::ERROR_GRAPHICS
			|| engineReady == Hack3dEngineReadiness::ERROR_ASSETS) {
			return false;
		}
		allReady = engineReady == Hack3dEngineReadiness::READY;	// add all the readiness states here
	}
	return true;
}

bool Sample1FireShadersGame::startGame() {
	running = true;
	Hack3dEngine* engine = Hack3dEngine::get();
	if (!engine->graphics->initWindow(
		Constants::DEFAULT_SCREEN_WIDTH, Constants::DEFAULT_SCREEN_HEIGHT,
		Constants::WINDOW_TITLE, Constants::WINDOW_POS_LEFT_X, Constants::WINDOW_POS_LEFT_Y)) {
		running = false;
		return false;
	}
	if (!engine->assets->loadAllResources()) {
		running = false;
		return false;
	}
	setScreen(std::make_unique<LogoScreen>(*this));
	return true;
}

void Sample1FireShadersGame::loop() {
	Hack3dEngine* engine = Hack3dEngine::get();
	double currentTime = engine->getTime();
	double prevTime = currentTime;
	double deltaTime;
	while (initialized && !ended) {
		if (running) {
			checkNewScreen();
			if (currentScreen) {
				currentTime = engine->getTime();
				deltaTime = std::min(currentTime - prevTime, Constants::MAX_DELTA_TIME_FRAME);
				prevTime = currentTime;
				update(deltaTime);
				render(deltaTime);
			}
		}
	}
}

void Sample1FireShadersGame::resume() {
	running = true;
	Hack3dEngine::get()->resume();
	if (currentScreen) {
		currentScreen->resume();
	}
}

void Sample1FireShadersGame::update(double deltaTime) {
	Hack3dEngine* engine = Hack3dEngine::get();
	engine->startUpdate(deltaTime);
	if (currentScreen) {
		currentScreen->update(deltaTime);
	}
	engine->endUpdate(deltaTime);
	if (engine->hasRequestExit()) { exit(); }
}

void Sample1FireShadersGame::render(double deltaTime) {
	Hack3dEngine* engine = Hack3dEngine::get();
	engine->graphics->startRender(deltaTime);
	if (currentScreen) {
		currentScreen->render(deltaTime);
	}
	engine->graphics->endRender(deltaTime);
}

void Sample1FireShadersGame::pause() {
	running = false;
	Hack3dEngine::get()->pause();
	if (currentScreen) {
		currentScreen->pause();
	}
}

void Sample1FireShadersGame::setScreen(std::unique_ptr<Screen> newScreen) {
	this->newScreen = std::move(newScreen);
}

void Sample1FireShadersGame::checkNewScreen() {
	if (newScreen) {
		if (currentScreen) {
			currentScreen->pause();
			currentScreen.reset();
		}
		currentScreen = std::move(newScreen);
		currentScreen->create();
		currentScreen->resume();
	}
}

void Sample1FireShadersGame::exit() {
	ended = true;
}
