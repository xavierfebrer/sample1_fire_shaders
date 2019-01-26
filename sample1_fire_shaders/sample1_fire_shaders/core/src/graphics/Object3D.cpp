#include "../../../core/include/Object3D.h"
Object3D::Object3D() {
	mVAO = -1;
	mVBO = -1;
	mEBO = -1;
	texture = nullptr;
	texture2 = nullptr;

	mVertexData.clear();
	mIndexData.clear();

	shaderforDraw = nullptr;

	mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	mRotationAxis = glm::vec3(0.0f, 0.0f, 0.0f);
	mRotationDegrees = glm::vec3(0.0f, 0.0f, 0.0f);
	mScale = glm::vec3(1.0f, 1.0f, 1.0f);
}


Object3D::~Object3D() {
	glDeleteVertexArrays(sizeof(mVAO), &mVAO);
	glDeleteBuffers(sizeof(mVBO), &mVBO);
	glDeleteBuffers(sizeof(mEBO), &mEBO);
	mVertexData.clear();
	mIndexData.clear();
}

void Object3D::loadObjFromDisk(std::string file) {
	std::string line;
	bool readVBO = false;
	bool readEBO = false;
	FileManager File(file.c_str(), "r");
	if (File.isOpened()) {
		while (!File.isEOF()) {
			line = File.getLine();
			if (line[0] != '#') {		// Ignore comments
				std::string dash = line.substr(0, 3);
				if (dash == "VBO") {
					readVBO = true;
					readEBO = false;
					continue;
				}
				if (dash == "EBO") {
					readVBO = false;
					readEBO = true;
					continue;
				}
				std::vector<std::string> data = splitString(line, ',');
				if (readVBO) {
					for (unsigned int i = 0; i < data.size(); i++) {
						mVertexData.push_back(atof(data[i].c_str()));
					}
				}
				if (readEBO) {
					for (unsigned int i = 0; i < data.size(); i++) {
						mIndexData.push_back(atoi(data[i].c_str()));
					}
				}
			}
		}
		File.close();
	}
	prepareObjGL();
}

std::shared_ptr<Texture2D> Object3D::loadTextureFromDisk(std::string file, GLint location,
	bool gammaCorrection,
	GLint tMinFilter, GLint tMaxFilter,
	GLint tWrapS, GLint tWrapT) {
	std::shared_ptr<Texture2D> t = std::shared_ptr<Texture2D>(new Texture2D(file, location, gammaCorrection,
		tMinFilter, tMaxFilter, tWrapS, tWrapT));
	t->init();
	return t;
}

void Object3D::generateObj() {
	//VBO data
	int	bytesData = 32;
	GLfloat vertexData[32] = {
		// x,y,z			u,v(s,t)	color
		-3.5f, 0.0f,  3.5f,	0.0,0.0,	0.0, 0.0, 1.0,
		 3.5f, 0.0f,  3.5f,	1.0,0.0,	0.0, 0.0, 1.0,
		 3.5f, 0.0f, -3.5f,	1.0,1.0,	0.0, 0.0, 1.0,
		-3.5f, 0.0f, -3.5f,	0.0,1.0,	0.0, 0.0, 1.0,
	};

	//EBO data
	GLuint indexData[6] = {
		0, 1, 2,   0, 2, 3
	};

	for (int i = 0; i < bytesData; i++) {
		mVertexData.push_back(vertexData[i]);
	}
	for (int i = 0; i < 6; i++) {
		mIndexData.push_back(indexData[i]);
	}
	prepareObjGL();
}

void Object3D::prepareObjGL() {
	// Create VAO
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	{
		//Create VBO
		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(GLfloat), mVertexData.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		//Create IBO
		glGenBuffers(1, &mEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexData.size() * sizeof(GLuint), mIndexData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // position
		glEnableVertexAttribArray(1); // Texture coor
		glEnableVertexAttribArray(2); // Color Vertex
	}
	glBindVertexArray(0);
}

void Object3D::generateTexture() {
}

void Object3D::update(double deltaTime) {
}

void Object3D::render(double deltaTime) {
	//Sets texture
	if (texture) {
		texture->location = 0;
		texture->activeBindTexture();
		shaderforDraw->setInt("tex" + std::to_string(texture->location + 1), texture->location);
	}
	if (texture2) {
		texture2->activeBindTexture();
		shaderforDraw->setInt("tex" + std::to_string(texture2->location + 1), texture2->location);
	}
	//Set VAO
	prepareVAO();
	//Draw VAO
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, mIndexData.size(), GL_UNSIGNED_INT, NULL);
	//Diable VAO
	diableVAO();
	//Unbind program
	//glUseProgram(NULL);
}

void Object3D::prepareVAO() {
	//Set VAO
	glBindVertexArray(mVAO);
	//Do Matrix movements
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, mPosition);
	modelMatrix = glm::scale(modelMatrix, mScale);
	shaderforDraw->setMat4("model", modelMatrix);
}

void Object3D::diableVAO() {
	//Diable VAO
	glBindVertexArray(0);
}

void Object3D::setShader(const std::shared_ptr<ShaderProgram> & p_shader) {
	shaderforDraw = p_shader;
}


