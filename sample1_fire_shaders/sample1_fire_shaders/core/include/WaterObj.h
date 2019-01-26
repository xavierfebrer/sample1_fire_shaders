#pragma once

#include "Object3D.h"

class WaterObj : public Object3D {
public:
	WaterObj();
	~WaterObj();

	virtual void render(double deltaTime) override;
	virtual void setShader(const std::shared_ptr<ShaderProgram> & p_shader) override;

	void setTexture2(const std::shared_ptr<Texture2D> & t) { texture2 = t; };
	std::shared_ptr<Texture2D> getTexture2() { return texture2; };

	void setTexture3(const std::shared_ptr<Texture2D> & t) { texture3 = t; };
	std::shared_ptr<Texture2D> getTexture3() { return texture3; };

	void setTexture4(const std::shared_ptr<Texture2D> & t) { texture4 = t; };
	std::shared_ptr<Texture2D> getTexture4() { return texture4; };

	int getUniformCamPos() { return mUniformCamPos; };
	int getUniformLightColor() { return mUniformLightColor; };
	int getUniformLightPos() { return mUniformLightPos; };


private:
	std::shared_ptr<Texture2D> texture2;
	std::shared_ptr<Texture2D> texture3;
	std::shared_ptr<Texture2D> texture4;
	float mCurrentOffsetWave;
	int	  mUniformTex;
	int	  mUniformTex2;
	int	  mUniformTex3;
	int	  mUniformTex4;
	int   mUniformOffsetWave;
	int   mUniformCamPos;
	int	  mUniformLightColor;
	int   mUniformLightPos;
};