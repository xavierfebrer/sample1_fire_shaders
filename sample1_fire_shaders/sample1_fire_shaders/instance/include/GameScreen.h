#pragma once

#include "BaseScreen.h"
#include "MainMenuScreen.h"
#include "EndScreen.h"
#include "../../core/include/ImageView.h"
#include "../../core/include/Button.h"
#include "GameWorld.h"
#include "../../core/include/LinearEase.h"

class GameScreen : public BaseScreen {
public:
	GameScreen(Game & game, int worldIndex, int levelIndex);
	virtual ~GameScreen() override;

	void create() override;
	void resume() override;
	void update(double deltaTime) override;
	void render(double deltaTime) override;
	void pause() override;

	bool isFinished();
private:
	GameWorld gameWorld;
	int worldIndex, levelIndex;

	void initViews();
	void goBack();

	class ButtonBackOnClickListener : public OnClickListener {
	private:
		GameScreen & screen;
	public:
		ButtonBackOnClickListener(GameScreen & screen) : OnClickListener(), screen(screen) {
		}
		virtual ~ButtonBackOnClickListener() override {
		}
		virtual void onClickDown(View & view) override {
		}
		virtual void onClickUp(View & view) override {
			screen.goBack();
		}
	};
};
