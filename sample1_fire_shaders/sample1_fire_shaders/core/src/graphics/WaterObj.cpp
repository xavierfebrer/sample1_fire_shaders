#include "../../include/WaterObj.h"

#define WAVE_SPEED 0.01f

WaterObj::WaterObj() {
	mCurrentOffsetWave = 0.0f;
}

WaterObj::~WaterObj() {
}

void WaterObj::render(double deltaTime) {
	//Sets texture
	int texCount = 0;
	if (texture) {
		glActiveTexture(GL_TEXTURE0 + texCount);
		glBindTexture(GL_TEXTURE_2D, texture->id);
		shaderforDraw->setInt("texture" + std::to_string(texCount), texCount);
	}
	++texCount;
	if (texture2) {
		glActiveTexture(GL_TEXTURE0 + texCount);
		glBindTexture(GL_TEXTURE_2D, texture2->id);
		shaderforDraw->setInt("texture" + std::to_string(texCount), texCount);
	}
	++texCount;
	if (texture3) {
		glActiveTexture(GL_TEXTURE0 + texCount);
		glBindTexture(GL_TEXTURE_2D, texture3->id);
		shaderforDraw->setInt("texture" + std::to_string(texCount), texCount);
	}
	++texCount;
	if (texture4) {
		glActiveTexture(GL_TEXTURE0 + texCount);
		glBindTexture(GL_TEXTURE_2D, texture4->id);
		shaderforDraw->setInt("texture" + std::to_string(texCount), texCount);
	}
	mCurrentOffsetWave += WAVE_SPEED * deltaTime; // It's better to control with FrameTime 
	if (mCurrentOffsetWave > 1.0f) {
		mCurrentOffsetWave -= 1.0;
	}
	shaderforDraw->setFloat("moveDistord", mCurrentOffsetWave);

	//Set VAO
	prepareVAO();
	//Draw VAO
	glDrawElements(GL_TRIANGLES, mIndexData.size(), GL_UNSIGNED_INT, NULL);
	//Diable VAO
	diableVAO();
}

void WaterObj::setShader(const std::shared_ptr<ShaderProgram>& p_shader) {
	shaderforDraw = p_shader;
	/*mUniformModelM = glGetUniformLocation(mShaderforDraw->getID(), "model");
	mUniformTex = glGetUniformLocation(mShaderforDraw->getID(), "texture1");
	mUniformTex2 = glGetUniformLocation(mShaderforDraw->getID(), "texture2");
	mUniformTex3 = glGetUniformLocation(mShaderforDraw->getID(), "texture3");
	mUniformTex4 = glGetUniformLocation(mShaderforDraw->getID(), "texture4");
	mUniformOffsetWave = glGetUniformLocation(mShaderforDraw->getID(), "moveDistord");
	mUniformCamPos = glGetUniformLocation(mShaderforDraw->getID(), "camPos");
	mUniformLightColor = glGetUniformLocation(mShaderforDraw->getID(), "lightcolor");
	mUniformLightPos = glGetUniformLocation(mShaderforDraw->getID(), "lightPos");*/
}
