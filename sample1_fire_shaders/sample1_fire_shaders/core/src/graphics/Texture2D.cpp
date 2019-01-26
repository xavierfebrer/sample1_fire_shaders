#include "../../include/Texture2D.h"

Texture2D::Texture2D(const std::string & completePath,
	GLint location,
	bool gammaCorrection,
	GLint tMinFilter, GLint tMaxFilter,
	GLint tWrapS, GLint tWrapT,
	GLint level, GLenum type, bool flipOnLoad)
	: completePath(completePath), location(location),
	gammaCorrection(gammaCorrection),
	tMinFilter(tMinFilter), tMaxFilter(tMaxFilter),
	tWrapS(tWrapS), tWrapT(tWrapT),
	level(level), type(type),
	flipOnLoad(flipOnLoad),
	initialized(false) {
}

Texture2D::~Texture2D() {
}

bool Texture2D::init() {
	if (!initialized) {
		glGenTextures(1, &id);
		Util::util_stbi_set_flip_vertically_on_load(flipOnLoad);
		data = Util::util_stbi_load(std::string(completePath).c_str(), &width, &height, &nrChannels, 0);

		GLenum internalFormat;
		GLenum dataFormat;
		if (nrChannels == 1) {
			internalFormat = dataFormat = GL_RED;
		} else if (nrChannels == 3) {
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		} else if (nrChannels == 4) {
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		if (data) {
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0, dataFormat, type, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tWrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrapT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tMinFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tMaxFilter);
		} else {
			std::cout << "Texture failed to load at path: " << completePath << std::endl;
		}

		Util::util_stbi_image_free(data);

		initialized = true;
	}
	return initialized;
}

bool Texture2D::isInitialized() {
	return initialized;
}

void Texture2D::activeBindTexture() {
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, id);
}
