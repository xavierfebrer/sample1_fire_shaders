#pragma once

#include "../../core/include/Util.h"
#include "../../core/include/Game.h"
#include "../../core/include/CollisionUtil.h"

class Entity;

class GameWorld {
public:
	Game & game;

	GameWorld(Game & game);
	virtual ~GameWorld();

	void init();
	void update(double deltaTime);
	void render(double deltaTime);
	int addEntity(const std::shared_ptr<Entity> & entity);
	int removeEntity(const std::shared_ptr<Entity> & entity);
private:
	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Entity>> entitiesToAdd;

	int addRemainingEntitiesToAdd();
	void updateInput(double deltaTime);
	void updateEntities(double deltaTime);
	bool checkRemoveFlatAndRemove(const std::shared_ptr<Entity> & entity);
};

