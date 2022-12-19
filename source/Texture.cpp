#pragma once

#include <Texture.h>

Texture::Texture(int unit, int width, int height)
{
	glGenTextures(1, &id_);
	activate(unit);
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::activate(int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	unit_ = unit;
}

void Texture::setUniformLocation(int location)
{
	glUniform1i(id_, location);
}
