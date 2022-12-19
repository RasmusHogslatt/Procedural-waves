#pragma once

#include <imgui.h>
#include <Camera.h>

Camera::Camera(float width, float height, glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Right{ glm::vec3(1) }, Up{ glm::vec3(0.0f, 1.0f, 0.0f) }, width{width}, height{height}
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	Near = 0.01f;
	Far = 50.0;
	Fov = 55.0f;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(Fov), width/height, Near, Far);
}

void Camera::updateGUI()
{
	
	ImGui::Text("CAMERA");
	if (ImGui::SliderFloat3("Camera position", &Position[0], -10.0f, 10.0f)) {
		updateCameraVectors();
	}
	if (ImGui::SliderFloat("Yaw", &Yaw, -180.0f, 180.0f)) {
		updateCameraVectors();
	}
	if (ImGui::SliderFloat("Pitch", &Pitch, -180.0f, 180.0f)) {
		updateCameraVectors();
	}
	if (ImGui::SliderFloat("FoV", &Fov, 15.0f, 120.0f)) {
		updateCameraVectors();
	}
	if (ImGui::SliderFloat("Near-Clip", &Near, 0.0f, 1.0f)) {
		updateCameraVectors();
	}
	if (ImGui::SliderFloat("Far-Clip", &Far, 1.0f, 10.0f)) {
		updateCameraVectors();
	}

}

void Camera::updateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}
