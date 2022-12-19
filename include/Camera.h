#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // Perspective options
    float Fov;
    float Near;
    float Far;
    float width, height;

    // constructor with vectors
    Camera(float width = 1600.0f, float height = 900.0f, glm::vec3 position = glm::vec3(1.0f, 0.65f, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -122.0f, float pitch = -22.0f);


    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();

    void updateGUI();

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
private:
};