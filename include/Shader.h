#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	uint32_t ID;

	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

	void use();

	void setUniformBool(const std::string& name, bool value);
	void setUniformInt(const std::string& name, int value);
	void setUniformFloat(const std::string& name, float value);
	void setUniformVec2(const std::string& name, const glm::vec2& value);
	void setUniformVec3(const std::string& name, const glm::vec3& value);
	void setUniformVec4(const std::string& name, const glm::vec4& value);
	void setUniformMat2(const std::string& name, const glm::mat2& value);
	void setUniformMat3(const std::string& name, const glm::mat3& value);
	void setUniformMat4(const std::string& name, const glm::mat4& value);

private:
	void checkCompileErrors(uint32_t shader, std::string type);
};