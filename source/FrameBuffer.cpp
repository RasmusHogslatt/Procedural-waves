#pragma once

#include <FrameBuffer.h>
#include <iostream>

FrameBuffer::FrameBuffer(int width, int height) : width{width}, height{height}
{
	// Generate framebuffer
	glGenFramebuffers(1, &fboID);
	bind();

	// Generate color texture0
	glGenTextures(1, &colorTextureID0);
	glBindTexture(GL_TEXTURE_2D, colorTextureID0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Attach color texture to bound framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureID0, 0);
	glBindTexture(GL_TEXTURE_2D, 0);


	// Generate color texture1
	glGenTextures(1, &colorTextureID1);
	glBindTexture(GL_TEXTURE_2D, colorTextureID1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Attach color texture to bound framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, colorTextureID1, 0);
	glBindTexture(GL_TEXTURE_2D, 0);


	// Generate depth and stencil texture
	glGenTextures(1, &depthStencilTextureID);
	glBindTexture(GL_TEXTURE_2D, depthStencilTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Attach depth and stencil texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthStencilTextureID, 0);
	
	const GLenum colorbuffers[]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, colorbuffers);
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
}

void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBuffer::getColorTextureID0()
{
	return colorTextureID0;
}

GLuint FrameBuffer::getColorTextureID1()
{
	return colorTextureID1;
}

GLuint FrameBuffer::getDepthStencilID()
{
	return depthStencilTextureID;
}

GLuint FrameBuffer::getFBOID()
{
	return fboID;
}

void FrameBuffer::checkCompleteness()
{
	bind();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	unbind();
}

int FrameBuffer::getWidth()
{
	return width;
}

int FrameBuffer::getHeight()
{
	return height;
}
