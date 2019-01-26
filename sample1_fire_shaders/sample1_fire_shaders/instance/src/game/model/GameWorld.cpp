#include "../../../include/GameWorld.h"
#include "../../../include/Entity.h"
#include "../../../include/TestGL.h"

GameWorld::GameWorld(Game & game) : game(game) {
}

GameWorld::~GameWorld() {
}

void GameWorld::init() {
	addEntity(std::shared_ptr<TestGL>(new TestGL(*this)));
}

void GameWorld::update(double deltaTime) {
	addRemainingEntitiesToAdd();
	updateInput(deltaTime);
	updateEntities(deltaTime);
}

void GameWorld::render(double deltaTime) {
	for (int i = entities.size() - 1; i >= 0; --i) {
		entities[i]->render(deltaTime);
	}
}

int GameWorld::addEntity(const std::shared_ptr<Entity> & entity) {
	int countAdded = 0;
	entitiesToAdd.push_back(entity);
	++countAdded;
	return countAdded;
}

int GameWorld::removeEntity(const std::shared_ptr<Entity> & entity) {
	int countRemoved = 0;
	for (int i = entities.size() - 1; i >= 0; --i) {
		if (entity == entities[i]) {
			entities.erase(entities.begin() + i);
			++countRemoved;
		}
	}
	for (int i = entitiesToAdd.size() - 1; i >= 0; --i) {
		if (entity == entitiesToAdd[i]) {
			entitiesToAdd.erase(entitiesToAdd.begin() + i);
			++countRemoved;
		}
	}
	return countRemoved;
}

int GameWorld::addRemainingEntitiesToAdd() {
	int countAdded = 0;
	if (entitiesToAdd.size() > 0) {
		for (int i = entitiesToAdd.size() - 1; i >= 0; --i) {
			entities.push_back(entitiesToAdd[i]);
			(entitiesToAdd[i])->create();
			++countAdded;
		}
		entitiesToAdd.clear();
	}
	return countAdded;
}

void GameWorld::updateInput(double deltaTime) {
}

void GameWorld::updateEntities(double deltaTime) {
	for (int i = 0; i < entities.size(); ++i) {
		if (!checkRemoveFlatAndRemove(entities[i])) {
			entities[i]->update(deltaTime);
			if (checkRemoveFlatAndRemove(entities[i])) {
				--i;
			}
		} else {
			--i;
		}
	}
}

bool GameWorld::checkRemoveFlatAndRemove(const std::shared_ptr<Entity> & entity) {
	return entity->removed && removeEntity(entity) > 0;
}
