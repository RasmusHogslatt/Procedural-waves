#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <Shader.h>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Mesh {
public:
	Mesh(std::vector<Vertex> vertices = std::vector<Vertex>(), std::vector<uint32_t> indices = std::vector<uint32_t>());
	void draw(Shader& shader, int mode);
	std::vector<Vertex> vertices_;
	std::vector<uint32_t> indices_;

	void generate_gridMesh(int size);
	void generate_points(int count, float domain);
private:
	uint32_t VAO, VBO, EBO;

	void setupMesh();
};