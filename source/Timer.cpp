#pragma once

#include <Timer.h>

Timer::Timer() : time_{0.0f}, dTime_{0.0f}
{
	glfwSetTime(time_);
}

void Timer::update()
{
	float lastTime = time_;
	time_ = glfwGetTime();
	dTime_ = time_ - lastTime;
}

void Timer::setUniforms(Shader& shader)
{
	shader.use();
	shader.setUniformFloat("uTime", time_);
	shader.setUniformFloat("uDeltaTime", dTime_);
}

float Timer::getTime()
{
	return time_;
}

float Timer::getDeltaTime()
{
	return dTime_;
}
