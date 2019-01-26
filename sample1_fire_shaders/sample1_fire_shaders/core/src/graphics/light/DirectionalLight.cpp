#include "..\..\..\include\DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction,
	glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: Light(ambient, diffuse, specular), direction(direction) {
}

DirectionalLight::~DirectionalLight() {
}
