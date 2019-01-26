#pragma once

#include "BaseScreen.h"
#include "MainMenuScreen.h"

class EndScreen : public BaseScreen {
public:
	EndScreen(Game & game);
	virtual ~EndScreen() override;

	virtual void create() override;
	virtual void resume() override;
	virtual void update(double deltaTime) override;
	virtual void render(double deltaTime) override;
	virtual void pause() override;
protected:
	virtual void initViews() override;
private:
	void openMainMenuScreen();

	class ButtonBackOnClickListener : public OnClickListener {
	private:
		EndScreen & screen;
	public:
		ButtonBackOnClickListener(EndScreen & screen) : OnClickListener(), screen(screen) {
		}
		virtual ~ButtonBackOnClickListener() override {
		}
		virtual void onClickDown(View & view) override {
		}
		virtual void onClickUp(View & view) override {
			screen.openMainMenuScreen();
		}
	};
};
