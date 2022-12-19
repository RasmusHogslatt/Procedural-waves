#pragma once

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <Mesh.h>
#include <Shader.h>
#include <Camera.h>
#include <ResourcePath.h>
#include <FrameBuffer.h>
#include <Texture.h>
#include <Timer.h>

#include <glm/gtx/euler_angles.hpp>

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	int width = 2400;
	int height = 1080;
	GLFWwindow* window = glfwCreateWindow(width, height, "ImGui + GLFW", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);


	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	// CREATE SHADERS //
	std::string vPath = RESOURCE_PATH + "shaders/vs1.vert";
	std::string fPath = RESOURCE_PATH + "shaders/fs1.frag";
	std::string gPath = RESOURCE_PATH + "shaders/gs1.geom";
	Shader shader1(vPath.c_str(), fPath.c_str(), gPath.c_str());

	vPath = RESOURCE_PATH + "shaders/vs2.vert";
	fPath = RESOURCE_PATH + "shaders/fs2.frag";
	gPath = RESOURCE_PATH + "shaders/gs2.geom";
	Shader shader2(vPath.c_str(), fPath.c_str());
	// STOP CREATE SHADERS //

	// CREATE MESHES //
	Mesh mesh = Mesh();
	int size = 1000;
	mesh.generate_gridMesh(size);

	Mesh points = Mesh();
	int numberOfBoxes = 10;
	points.generate_points(numberOfBoxes, 0.5f);
	// STOP CREATE MESH //

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// UNIFORMS
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= glm::eulerAngleX(45.0f);
	
	shader1.setUniformMat4("M", modelMatrix);
	shader2.setUniformMat4("M", modelMatrix);

	// CAMERA //
	Camera camera = Camera();
	Camera cameraTop = Camera();
	cameraTop.Position = glm::vec3(0, 1.5f, 0); cameraTop.Pitch = -90.0f; cameraTop.updateCameraVectors();
	glm::mat4 viewMatrix = camera.GetViewMatrix();
	glm::mat4 projectionMatrix = camera.GetProjectionMatrix();
	glm::mat4 topProjectionMatrix = glm::ortho(-10.0f, 1.0f, .0f, 1.0f, .0f, 10.0f);
	glm::mat4 topViewMatrix = cameraTop.GetViewMatrix();
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(viewMatrix * modelMatrix));
	shader1.setUniformMat4("V", viewMatrix);
	shader1.setUniformMat4("P", projectionMatrix);
	shader1.setUniformMat4("N", normalMatrix);
	shader1.setUniformFloat("near", camera.Near);
	shader1.setUniformFloat("far", camera.Far);
	shader2.setUniformMat4("V", viewMatrix);
	shader2.setUniformMat4("P", projectionMatrix);
	shader2.setUniformMat4("N", normalMatrix);
	shader2.setUniformFloat("near", camera.Near);
	shader2.setUniformFloat("far", camera.Far);
	// STOP CAMERA //


	FrameBuffer fbo0(1600, 900);
	fbo0.checkCompleteness();
	
	Timer timer;
	glm::vec3 waveColor1 = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 waveColor2 = glm::vec3(0.0f, 0.82f, 1.0f);
	glm::vec4 waveSettings0 = glm::vec4(0.3f, 0.1f, 0.4f, 0.2f);
	glm::vec4 waveSettings1 = glm::vec4(0.25f, 0.5f, 0.40f, 0.25f);
	glm::vec4 waveSettings2 = glm::vec4(0.6f, 0.2f, 0.45f, 0.25f);
	glm::vec4 waveSettings3 = glm::vec4(0.5f, 0.1f, 0.5f, 0.3f);
	glm::vec3 boxColor = glm::vec3(0.2941f, 0.2f, 0.0f);
	float boxShininess = 10.0f;
	float boxSize = 0.04f;
	float waveShininess = 10.0f;
	glm::vec3 lightPosition1 = glm::vec3(25.0f, 50.0f, 25.0f);
	glm::vec3 lightPosition2 = glm::vec3(-25.0f, 50.0f, 25.0f);
	glm::vec3 lightPosition3 = glm::vec3(25.0f, 50.0f, -25.0f);
	glm::vec3 lightPosition4 = glm::vec3(-25.0f, 50.0f, -25.0f);
	float fbmScale = 0.15f;
	int fbmOctaves = 12;
	int activeLights = 4;
	float lightStrength = 0.25f;
	shader1.setUniformVec3("boxColor", boxColor);
	shader1.setUniformVec3("lightPos1", lightPosition1);
	shader1.setUniformVec3("lightPos2", lightPosition2);
	shader1.setUniformVec3("lightPos3", lightPosition3);
	shader1.setUniformVec3("lightPos4", lightPosition4);
	shader1.setUniformVec3("waveColor1", waveColor1);
	shader1.setUniformVec3("waveColor2", waveColor2);
	shader1.setUniformFloat("boxShininess", boxShininess);
	shader1.setUniformFloat("boxSize", boxSize);
	shader1.setUniformVec4("uWave0", waveSettings0);
	shader1.setUniformVec4("uWave1", waveSettings1);
	shader1.setUniformVec4("uWave2", waveSettings2);
	shader1.setUniformVec4("uWave3", waveSettings3);
	shader1.setUniformFloat("fbmScale", fbmScale);
	shader1.setUniformFloat("lightStrength", lightStrength);
	shader1.setUniformInt("activeLights", activeLights);
	shader1.setUniformInt("OCTAVES", fbmOctaves);
	shader2.setUniformFloat("waveShininess", waveShininess);
	shader2.setUniformFloat("fbmScale", fbmScale);
	shader2.setUniformVec3("lightPos1", lightPosition1);
	shader2.setUniformVec3("lightPos2", lightPosition2);
	shader2.setUniformVec3("lightPos3", lightPosition3);
	shader2.setUniformVec3("lightPos4", lightPosition4);
	shader2.setUniformVec4("uWave0", waveSettings0);
	shader2.setUniformVec4("uWave1", waveSettings1);
	shader2.setUniformVec4("uWave2", waveSettings2);
	shader2.setUniformVec4("uWave3", waveSettings3);
	shader2.setUniformInt("activeLights", activeLights);
	shader2.setUniformVec3("waveColor1", waveColor1);
	shader2.setUniformVec3("waveColor2", waveColor2);
	shader2.setUniformFloat("lightStrength", lightStrength);
	shader2.setUniformInt("OCTAVES", fbmOctaves);
	bool wireframeMode = true;

	// Main while loop
	while (!glfwWindowShouldClose(window)) {
		// Update variables
		timer.update();
		timer.setUniforms(shader1);
		timer.setUniforms(shader2);

		glEnable(GL_DEPTH_TEST);
		if (!wireframeMode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glCullFace(GL_FRONT);
		}

		// Bind FBO
		fbo0.bind();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* FIRST PASS */
		modelMatrix = glm::eulerAngleY(timer.getTime()*0.2f);

		shader1.setUniformMat4("M", modelMatrix);
		shader1.setUniformMat4("V", camera.GetViewMatrix());
		shader1.setUniformMat4("P", camera.GetProjectionMatrix());
		shader1.setUniformFloat("near", camera.Near);
		shader1.setUniformFloat("far", camera.Far);
		
		/* SECOND PASS*/
		shader2.setUniformMat4("M", modelMatrix);
		shader2.setUniformMat4("V", camera.GetViewMatrix());
		shader2.setUniformMat4("P", camera.GetProjectionMatrix());
		shader2.setUniformFloat("near", camera.Near);
		shader2.setUniformFloat("far", camera.Far);

		mesh.draw(shader2, 0);
		points.draw(shader1, 1);
		
		// STOP DRAW GL HERE //
		fbo0.unbind();

		glfwGetWindowSize(window, &width, &height);
		ImGui::SetNextWindowSize(ImVec2(static_cast<float>(width), static_cast<float>(height))); // ensures ImGui fits the GLFW window
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("MainWindow");

		ImVec2 renderPosition = ImVec2(610, 50);
		ImGui::GetWindowDrawList()->AddImage(
			(void*)fbo0.getColorTextureID0(), renderPosition,
			ImVec2(fbo0.getWidth() + renderPosition.x, fbo0.getHeight() + renderPosition.y), ImVec2(0, 1), ImVec2(1, 0));

		// GUI STUFF //
		ImGui::BeginChild("Properties", ImVec2(renderPosition.x-10, 1000), true);
		ImGui::Checkbox("Fill/Wireframe", &wireframeMode);
		camera.updateGUI();
		
		ImGui::NewLine();
		ImGui::Text("WATER SETTINGS");
		
		if (ImGui::SliderInt("Water resolution", &size, 1, 5000)) {
			mesh.generate_gridMesh(size);
		}
		ImGui::SliderFloat4("0: Dx Dy Steep Freq", &waveSettings0[0], 0.0, 1.0);
		ImGui::SliderFloat4("1: Dx Dy Steep Freq", &waveSettings1[0], 0.0, 1.0);
		ImGui::SliderFloat4("2: Dx Dy Steep Freq", &waveSettings2[0], 0.0, 1.0);
		ImGui::SliderFloat4("3: Dx Dy Steep Freq", &waveSettings3[0], 0.0, 1.0);
		ImGui::SliderFloat("FBM scale", &fbmScale, 0.0f, 2.0f);
		ImGui::SliderInt("FBM octaves", &fbmOctaves, 1, 100);
		ImGui::SliderFloat("Wave shininess", &waveShininess, 0.1f, 100.0f);
		ImGui::ColorEdit3("Wave color 1", &waveColor1[0]);
		ImGui::ColorEdit3("Wave color 2", &waveColor2[0]);

		ImGui::NewLine();
		ImGui::Text("LIGHT SETTINGS");

		ImGui::SliderInt("Active lights", &activeLights, 1, 4);
		ImGui::SliderFloat("Light strength", &lightStrength, 0.0f, 1.0f);
		ImGui::SliderFloat3("Light position 1", &lightPosition1[0], -50.0, 50.0);
		ImGui::SliderFloat3("Light position 2", &lightPosition2[0], -50.0, 50.0);
		ImGui::SliderFloat3("Light position 3", &lightPosition3[0], -50.0, 50.0);
		ImGui::SliderFloat3("Light position 4", &lightPosition4[0], -50.0, 50.0);

		ImGui::NewLine();
		ImGui::Text("BOX SETTINGS");
		if (ImGui::SliderInt("Number of boxes", &numberOfBoxes, 0, 100)) {
			points.generate_points(numberOfBoxes, 0.5f);
		}
		ImGui::ColorEdit3("Box color", &boxColor[0]);
		ImGui::SliderFloat("Box size", &boxSize, 0.000001f, 1.0f);
		ImGui::SliderFloat("Box shininess", &boxShininess, 0.1f, 100.0f);

		// END GUI STUFF //

		// Update uniforms
		shader1.setUniformVec3("boxColor", boxColor);
		shader1.setUniformVec3("lightPos1", lightPosition1);
		shader1.setUniformVec3("lightPos2", lightPosition2);
		shader1.setUniformVec3("lightPos3", lightPosition3);
		shader1.setUniformVec3("lightPos4", lightPosition4);
		shader1.setUniformFloat("boxSize", boxSize);
		shader1.setUniformFloat("boxShininess", boxShininess);
		shader1.setUniformVec4("uWave0", waveSettings0);
		shader1.setUniformVec4("uWave1", waveSettings1);
		shader1.setUniformVec4("uWave2", waveSettings2);
		shader1.setUniformVec4("uWave3", waveSettings3);
		shader1.setUniformInt("activeLights", activeLights);
		shader1.setUniformFloat("fbmScale", fbmScale);
		shader1.setUniformVec3("waveColor1", waveColor1);
		shader1.setUniformVec3("waveColor2", waveColor2);
		shader1.setUniformFloat("lightStrength", lightStrength);
		shader1.setUniformInt("OCTAVES", fbmOctaves);

		shader2.setUniformVec3("lightPos1", lightPosition1);
		shader2.setUniformVec3("lightPos2", lightPosition2);
		shader2.setUniformVec3("lightPos3", lightPosition3);
		shader2.setUniformVec3("lightPos4", lightPosition4);
		shader2.setUniformFloat("waveShininess", waveShininess);
		shader2.setUniformVec4("uWave0", waveSettings0);
		shader2.setUniformVec4("uWave1", waveSettings1);
		shader2.setUniformVec4("uWave2", waveSettings2);
		shader2.setUniformVec4("uWave3", waveSettings3);
		shader2.setUniformInt("activeLights", activeLights);
		shader2.setUniformFloat("fbmScale", fbmScale);
		shader2.setUniformVec3("waveColor1", waveColor1);
		shader2.setUniformVec3("waveColor2", waveColor2);
		shader2.setUniformFloat("lightStrength", lightStrength);
		shader2.setUniformInt("OCTAVES", fbmOctaves);
		ImGui::EndChild();

		// Ends the property window
		ImGui::End();

		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}