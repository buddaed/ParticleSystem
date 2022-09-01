#include "Camera.h"

Camera::Camera(wolf::App *inApp)
{
    cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    nearPlane = 0.1f;
    farPlane = 1000.0f;
    fieldOfView = 45.0f;
    app = inApp;
    glfwSetInputMode(app->m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Camera::~Camera()
{
}

wolf::App *const Camera::getApp()
{
    return app;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::setSpeed(float const inSpeed)
{
    speedMultiplier = inSpeed;
}

void Camera::setFront(glm::vec3 const target)
{
    cameraFront = target;
}
void Camera::setPos(glm::vec3 const pos)
{
    cameraPos = pos;
}
void Camera::setFOV(float const fov)
{
    fieldOfView = fov;
}
void Camera::setNear(float const near)
{
    nearPlane = near;
}
void Camera::setFar(float const far)
{
    farPlane = far;
}
void Camera::setCameraUp(glm::vec3 const up)
{
    cameraUp = up;
}
glm::vec3 Camera::getPos() const
{
    return cameraPos;
}
float Camera::getFOV() const
{
    return fieldOfView;
}
float Camera::getNear() const
{
    return nearPlane;
}
float Camera::getFar() const
{
    return farPlane;
}
glm::vec3 const Camera::getFront()
{
    return cameraFront;
}
glm::vec3 const Camera::getCameraUp()
{
    return cameraUp;
}
glm::mat4 const Camera::getPersProjection(float const width, float const height)
{
    if (persProjMatrix == glm::mat4(1))
    {
        persProjMatrix = glm::perspective(glm::radians(this->getFOV()), (float)width / (float)height, this->getNear(), this->getFar());
    }
    return persProjMatrix;
}

void Camera::update(float const dt)
{
    float currentFrame = glfwGetTime();
    glm::vec2 mousePos = app->getMousePos();
    float speed = speedMultiplier * dt;

    if (app->isKeyDown('w'))
    {
        cameraPos += cameraFront * speed;
    }
    if (app->isKeyDown('s'))
    {
        cameraPos -= cameraFront * speed;
    }
    if (app->isKeyDown('a'))
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    }
    if (app->isKeyDown('d'))
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    }

    if (firstMouse)
    {
        lastX = mousePos.x;
        lastY = mousePos.y;
        firstMouse = false;
    }

    float xoffset = mousePos.x - lastX;
    float yoffset = lastY - mousePos.y; // reversed since y-coordinates go from bottom to top
    lastX = mousePos.x;
    lastY = mousePos.y;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }

    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    glm::vec3 front;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(front);
}