#pragma once

#include <imgui.h>
#include <Mesh.h>
#include <glad/glad.h>
#include <random>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
	vertices_ = vertices;
	indices_ = indices;

	setupMesh();
}

void Mesh::draw(Shader& shader, int mode)
{
	// draw mesh
	shader.use();
	glBindVertexArray(VAO);
	if (mode == 0) {
		glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	}
	else if (mode == 1) {
		glDrawElements(GL_POINTS, indices_.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

void Mesh::generate_gridMesh(int size)
{
	vertices_.clear();
	indices_.clear();
	Vertex v;
	for (int j = 0; j <= size; ++j)
	{
		for (int i = 0; i <= size; ++i)
		{
			v.position.x = ((float)i / (float)size) - 0.5f;
			v.position.y = 0.0f;
			v.position.z = ((float)j / (float)size) - 0.5f;
			v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			v.uv = glm::vec2(static_cast<float>(i) / static_cast<float>(size), static_cast<float>(j) / static_cast<float>(size));
			vertices_.push_back(v);
		}
	}

	for (int j = 0; j < size; ++j)
	{
		for (int i = 0; i < size; ++i)
		{
			int row1 = j * (size + 1);
			int row2 = (j + 1) * (size + 1);

			// triangle 1
			indices_.push_back(row1 + i);
			indices_.push_back(row1 + i + 1);
			indices_.push_back(row2 + i + 1);

			// triangle 2
			indices_.push_back(row1 + i);
			indices_.push_back(row2 + i + 1);
			indices_.push_back(row2 + i);
		}
	}
	setupMesh();
}

void Mesh::generate_points(int count, float domain)
{
	vertices_.clear();
	indices_.clear();
	Vertex v;
	for (int i = 0; i < count; ++i)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> distribution(-domain, domain);
		v.position.x = distribution(gen);
		v.position.y = 0.0f;
		v.position.z = distribution(gen);
		v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
		v.uv = glm::vec2(v.position.x, v.position.z);
		vertices_.push_back(v);
		indices_.push_back(i);
	}

	setupMesh();
}

void Mesh::setupMesh()
{
	if (vertices_.size() != 0 && indices_.size() != 0) {

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint32_t),
			&indices_[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}
}
