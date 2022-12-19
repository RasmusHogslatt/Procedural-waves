#pragma once

#include <glad/glad.h>

class FrameBuffer {
public:
	FrameBuffer(int width, int height);
	void bind();
	void unbind();
	GLuint getFBOID();
	GLuint getColorTextureID0();
	GLuint getColorTextureID1();
	GLuint getDepthStencilID();
	void checkCompleteness();
	int getWidth();
	int getHeight();
private:
	GLuint fboID;
	GLuint colorTextureID0;
	GLuint colorTextureID1;
	GLuint depthStencilTextureID;
	int width;
	int height;
};