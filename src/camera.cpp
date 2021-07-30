#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

// processes input received from a mouse input system
void Camera::ProcessMouseMovement(float xpos, float ypos, GLboolean constrainPitch)
{
    float xoffset = (xpos - x);
    float yoffset = (ypos - y);
    y = ypos;
    x = xpos;
    if (firstMouseInput) {
        firstMouseInput = false;
        return;
    }
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch -= yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event
void Camera::ProcessMouseScroll(float yoffset)
{
    if (glm::length(Position) >= 75.0f && yoffset <= 0.0f) return;
    if (glm::length(Position) <= 25.0f && yoffset >= 0.0f) return;
    Position *= (-yoffset * 0.05f + 1.0f);
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::rotate(float dt, bool clockwise) {
    glm::mat4 model = glm::mat4(1.0f);
    if (clockwise) {
        model = glm::rotate(model, glm::radians(90*dt), glm::vec3(0.0f, 1.0f, 0.0f));
        Position = glm::vec3(model * glm::vec4(Position, 1.0f));
        Yaw -= 90 * dt;
    }

    if (!clockwise) { //counterclockwise
        model = glm::rotate(model, -glm::radians(90 * dt), glm::vec3(0.0f, 1.0f, 0.0f));
        Position = glm::vec3(model * glm::vec4(Position, 1.0f));
        Yaw += 90 * dt;
    }
    updateCameraVectors();
}