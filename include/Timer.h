#pragma once

#include <Shader.h>
#include <GLFW/glfw3.h>

class Timer {
public:
	Timer();
	void update();
	void setUniforms(Shader& shader);
	float getTime();
	float getDeltaTime();
private:
	float time_;
	float dTime_;
};