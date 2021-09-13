#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 50.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float ROTATE_SPEED = 50.0f;

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
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
    float x = 0.0f, y = 0.0f;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    // rotation
    int rotating = 0; //0: not rotating, 1: rotating clockwise, -1: rotating counterclockwise
    float degreesRotated = 0.0f;

    bool firstMouseInput = true;
    bool freeMode        = false;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    glm::mat4 GetProjectionMatrix()
    {
        return glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 500.0f);  //persepective
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // processes input received from a mouse input system
    void ProcessMouseMovement(float xpos, float ypos, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event
    void ProcessMouseScroll(float ypos);

    //rotate camera by 90 degrees if user presses q or e
    void rotate(float dt);

    //move camera vertically
    void moveVertical(float dy);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};