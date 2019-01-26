#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Entity.h"

#include "../../core/include/ShaderProgram.h"
#include "../../core/include/VertexShader.h"
#include "../../core/include/FragmentShader.h"
#include "../../core/include/Camera.h"
#include "../../core/include/Light.h"
#include "../../core/include/Material.h"
#include "../../core/include/Model.h"
#include "../../core/include/Texture2D.h"
#include "../../core/include/CubeMap.h"
#include "../../core/include/WaterObj.h"
#include "../../core/include/FireObj.h"
#include "../../core/include/WaterFrameBuffers.h"
#include "../../core/include/FireFrameBuffers.h"
#include "../include/Constants.h"

class TestGL : public Entity {
public:
	TestGL(GameWorld & gameWorld);
	virtual ~TestGL() override;

	virtual void create() override;
	virtual void update(double deltaTime) override;
	void renderModels(double deltaTime);
	void renderFires(double deltaTime);
	virtual void render(double deltaTime) override;
	virtual void collision(Entity* other) override;
private:
	std::shared_ptr<Camera> camera;
	std::shared_ptr<ShaderProgram> fireShader;
	std::shared_ptr<ShaderProgram> sceneShader;
	double timer = 0;

	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<FireObj>> fires;

	void renderScene(double deltaTime);
};