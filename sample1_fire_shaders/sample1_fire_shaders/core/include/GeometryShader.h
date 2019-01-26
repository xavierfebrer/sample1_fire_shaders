#pragma once

#include "Shader.h"

class GeometryShader : public Shader {
public:
	GeometryShader(const std::string & completeShaderPath);
	virtual ~GeometryShader() override;

	virtual bool init() override;
};