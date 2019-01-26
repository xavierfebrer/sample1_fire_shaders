#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Util.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "FileManager.h"

class Object3D
{
public:
	Object3D();
	virtual ~Object3D();

	virtual void loadObjFromDisk(std::string file);
	virtual std::shared_ptr<Texture2D> loadTextureFromDisk(std::string file, GLint location,
		bool gammaCorrection = false,
		GLint tMinFilter = GL_LINEAR_MIPMAP_LINEAR, GLint tMaxFilter = GL_LINEAR,
		GLint tWrapS = GL_REPEAT, GLint tWrapT = GL_REPEAT);
	virtual void generateObj();
	virtual void generateTexture();
	virtual void update(double deltaTime);
	virtual void render(double deltaTime);
	virtual void prepareVAO();
	virtual void diableVAO();

	virtual void setShader(const std::shared_ptr<ShaderProgram> & p_shader);

	void setPosition(glm::vec3 point) { mPosition = point; };
	glm::vec3 getPosition() { return mPosition; };

	void setRotationAxis(glm::vec3 point) { mRotationAxis = point; };
	glm::vec3 getRotationAxis() { return mRotationAxis; };

	void setRotationDegrees(glm::vec3 point) { mRotationDegrees = point; };
	glm::vec3 getRotationDegrees() { return mRotationDegrees; };

	void setScale(glm::vec3 point) { mScale = point; };
	glm::vec3 getScale() { return mScale; };

	void setTexture(std::shared_ptr<Texture2D> t) { texture = t; };
	std::shared_ptr<Texture2D> getTexture() { return texture; };

	void setTexture2(std::shared_ptr<Texture2D> t) { texture2 = t; };
	std::shared_ptr<Texture2D> getTexture2() { return texture2; };

protected:

	void prepareObjGL();

	GLuint mVAO;
	GLuint mVBO;
	GLuint mEBO;
	std::shared_ptr<Texture2D> texture;
	std::shared_ptr<Texture2D> texture2;

	std::vector<GLfloat> mVertexData;
	std::vector<GLuint> mIndexData;

	std::shared_ptr<ShaderProgram> shaderforDraw;

	glm::vec3	mPosition;
	glm::vec3	mRotationAxis;
	glm::vec3	mRotationDegrees;
	glm::vec3	mScale;
};

