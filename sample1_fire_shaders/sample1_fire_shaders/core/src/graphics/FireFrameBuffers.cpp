#include "../../include/FireFrameBuffers.h"

const int SCREEN_WIDTH = (1920.0*0.75);
const int SCREEN_HEIGHT = (1080.0*0.75);

FireFrameBuffers::FireFrameBuffers()
{
	initialiseReflectionFrameBuffer();
	initialiseRefractionFrameBuffer();
}


FireFrameBuffers::~FireFrameBuffers()
{
}

void FireFrameBuffers::cleanUp() {
	glDeleteFramebuffers(sizeof(reflectionFrameBuffer), &reflectionFrameBuffer);
	glDeleteTextures(sizeof(reflectionTexture), &reflectionTexture);
	glDeleteRenderbuffers(sizeof(reflectionDepthBuffer), &reflectionDepthBuffer);
	glDeleteFramebuffers(sizeof(refractionFrameBuffer), &refractionFrameBuffer);
	glDeleteTextures(sizeof(refractionTexture), &refractionTexture);
	glDeleteTextures(sizeof(refractionDepthTexture), &refractionDepthTexture);
}

void FireFrameBuffers::bindReflectionFrameBuffer() {//call before rendering to this FBO
	bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void FireFrameBuffers::bindRefractionFrameBuffer() {//call before rendering to this FBO
	bindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void FireFrameBuffers::unbindCurrentFrameBuffer() {//call to switch to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

int FireFrameBuffers::getReflectionTexture() {//get the resulting texture
	return reflectionTexture;
}

int FireFrameBuffers::getRefractionTexture() {//get the resulting texture
	return refractionTexture;
}

int FireFrameBuffers::getRefractionDepthTexture() {//get the resulting depth texture
	return refractionDepthTexture;
}

void FireFrameBuffers::initialiseReflectionFrameBuffer() {
	reflectionFrameBuffer = createFrameBuffer();
	reflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	unbindCurrentFrameBuffer();
}

void FireFrameBuffers::initialiseRefractionFrameBuffer() {
	refractionFrameBuffer = createFrameBuffer();
	refractionTexture = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	unbindCurrentFrameBuffer();
}

void FireFrameBuffers::bindFrameBuffer(int frameBuffer, int width, int height) {
	glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}

GLuint FireFrameBuffers::createFrameBuffer() {
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	//generate name for frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	//create the framebuffer
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//indicate that we will always render to color attachment 0
	return frameBuffer;
}

GLuint FireFrameBuffers::createTextureAttachment(int width, int height) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
	return texture;
}

GLuint FireFrameBuffers::createDepthTextureAttachment(int width, int height) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	return texture;
}

GLuint FireFrameBuffers::createDepthBufferAttachment(int width, int height) {
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	return depthBuffer;
}
