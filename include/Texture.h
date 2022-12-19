#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Texture {
public:
	Texture(int unit, int width, int height);
	void bind();
	void activate(int unit);
	void setUniformLocation(int location);
private:
	GLuint id_;
	int unit_;
};