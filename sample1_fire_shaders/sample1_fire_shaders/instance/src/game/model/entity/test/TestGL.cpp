#include "../../../../../include/TestGL.h"
#include "../../../../../../core/include/Hack3dEngine.h"

TestGL::TestGL(GameWorld & gameWorld) : Entity(gameWorld), camera(nullptr) {
}

TestGL::~TestGL() {
}

void TestGL::create() {
	glEnable(GL_DEPTH_TEST);
	// required to see transparency, sorted transparency rendering, first models then fires, otherwise models are not seen through the fires
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// prepare camera
	camera = std::make_shared<Camera>(((float)Constants::DEFAULT_SCREEN_WIDTH) / ((float) Constants::DEFAULT_SCREEN_HEIGHT));
	camera->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
	camera->setYaw(-90.0f);
	camera->setPitch(0.0f);
	camera->updateCameraVectors();

	// fires
	fireShader = std::make_shared<ShaderProgram>(
		std::make_shared<VertexShader>("assets/shader/custom_fire_1.vs"),
		std::make_shared<FragmentShader>("assets/shader/custom_fire_1.fs"));
	fireShader->init();
	// models and scene in general
	sceneShader = std::make_shared<ShaderProgram>(
		std::make_shared<VertexShader>("assets/shader/1.model_loading.vs"),
		std::make_shared<FragmentShader>("assets/shader/1.model_loading.fs"));
	sceneShader->init();

	// 1. load fires
	std::shared_ptr<FireObj> currentFire;

	currentFire = std::make_shared<FireObj>();
	currentFire->loadObjFromDisk("assets/FrontalQuad.txt");
	currentFire->setShader(fireShader);
	currentFire->setTexture(currentFire->loadTextureFromDisk("assets/textures/fire_shape_1.png", 0));
	currentFire->setTexture2(currentFire->loadTextureFromDisk("assets/textures/fire_shape_2.jpg", 1));
	currentFire->setPosition(glm::vec3(1.545, 0.0, -0.21));
	currentFire->setScale(glm::vec3(1.0, 1.0, 1.0));
	currentFire->fireHeight = 0.28f;
	currentFire->speed = 0.531f;
	currentFire->leftSmooth = 0.30f;
	currentFire->rightSmooth = 0.30f;
	currentFire->toon = false;
	currentFire->blackCorrectionThreshold = 0.2f;
	fires.push_back(currentFire);

	currentFire = std::make_shared<FireObj>();
	currentFire->loadObjFromDisk("assets/FrontalQuad.txt");
	currentFire->setShader(fireShader);
	currentFire->setTexture(currentFire->loadTextureFromDisk("assets/textures/fire_shape_1.png", 0));
	currentFire->setTexture2(currentFire->loadTextureFromDisk("assets/textures/fire_shape_2.jpg", 1));
	currentFire->setPosition(glm::vec3(5.1525, 0.3, 0.27));
	currentFire->setScale(glm::vec3(0.3, 0.3, 0.3));
	currentFire->fireHeight = 0.35f;
	currentFire->speed = 0.4f;
	currentFire->leftSmooth = 0.50f;
	currentFire->rightSmooth = 0.50f;
	currentFire->toon = true;
	currentFire->blackCorrectionThreshold = 0.1f;
	fires.push_back(currentFire);

	// 2. load models
	std::shared_ptr<Model> currentModel;

	currentModel = std::shared_ptr<Model>(new Model("assets/objects/85-fireplace/fireplace_lopoly.obj"));
	currentModel->extraTranslation = glm::vec3(0.0, 0.0, 0.0);
	currentModel->extraScale = glm::vec3(0.017f, 0.017f, 0.017f);
	models.push_back(std::shared_ptr<Model>(currentModel));

	currentModel = std::shared_ptr<Model>(new Model("assets/objects/fireplace_bricks/13111_fireplacebricks_v2_l2.obj"));
	currentModel->extraTranslation = glm::vec3(5.0, 0.0, 0.0);
	currentModel->extraScale = glm::vec3(0.02f, 0.02f, 0.02f);
	currentModel->extraRotation = glm::vec3(1.0f, 0.0f, 0.0f);
	currentModel->extraRotationAngle = glm::radians(-90.0f);
	models.push_back(std::shared_ptr<Model>(currentModel));
}

void TestGL::update(double deltaTime) {
	timer += deltaTime;
	camera->enableExperimentalCameraMovement(deltaTime);
	camera->updateCameraVectors();
	for (int i = 0; i < fires.size(); i++) {
		fires[i]->update(deltaTime);
	}
}

void TestGL::renderModels(double deltaTime) {
	sceneShader->use();
	sceneShader->setMat4("projection", camera->getProjectionMatrix());
	sceneShader->setMat4("view", camera->getViewMatrix());
	glm::mat4 model;
	for (int i = 0; i < models.size(); i++) {
		model = camera->getModelMatrix();
		model = glm::translate(model, models[i]->extraTranslation);
		model = glm::scale(model, models[i]->extraScale);
		if (models[i]->extraRotationAngle != 0.0f) {
			model = glm::rotate(model, models[i]->extraRotationAngle, models[i]->extraRotation);
		}
		sceneShader->setMat4("model", model);
		models[i]->draw(*sceneShader);
	}
}

void TestGL::renderFires(double deltaTime) {
	fireShader->use();
	fireShader->setMat4("projection", camera->getProjectionMatrix());
	fireShader->setMat4("view", camera->getViewMatrix());
	for (int i = 0; i < fires.size(); i++) {
		fireShader->setFloat("time", timer);
		fireShader->setFloat("fireHeight", fires[i]->fireHeight);
		fireShader->setFloat("speed", fires[i]->speed);
		fireShader->setFloat("leftSmooth", fires[i]->leftSmooth);
		fireShader->setFloat("rightSmooth", fires[i]->rightSmooth);
		fireShader->setFloat("toon", fires[i]->toon);
		fireShader->setFloat("blackCorrectionThreshold", fires[i]->blackCorrectionThreshold);
		fires[i]->render(deltaTime);
	}
}

void TestGL::renderScene(double deltaTime) {
	//Clear color buffer & Z buffer
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 1. Render the models
	renderModels(deltaTime);
	// 2. Render the fire, so the blending works and creates a nice effect.
	renderFires(deltaTime);
}

void TestGL::render(double deltaTime) {
	renderScene(deltaTime);
}

void TestGL::collision(Entity * other) {
}
