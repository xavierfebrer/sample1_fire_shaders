#include "..\..\..\include\GeometryShader.h"

GeometryShader::GeometryShader(const std::string & completeShaderPath) : Shader(completeShaderPath) {
}

GeometryShader::~GeometryShader() {
}

bool GeometryShader::init() {
	if (!initialized && compileShader(readShaderSource(), GL_GEOMETRY_SHADER)) {
		initialized = true;
	}
	return initialized;
}
