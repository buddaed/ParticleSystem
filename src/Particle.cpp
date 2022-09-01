#include "Particle.h"

Particle::Particle()
{
    life = 5.0f;
    timeAlive = 0;
    fade = 1;
    velocity = glm::vec3(0, 0, 0);
    pos = glm::vec3(0, 0, 0);
    scale = glm::vec3(10, 10, 10);
    next = NULL;
    prev = NULL;
}

Particle::~Particle()
{
}

Vertex *Particle::getVertices()
{
    return finalVertices;
}

/**
 * updates all vertices related to the particle
 * @param viewMatrix the view matrix of the camera to construct billboard from
 * */
void Particle::_calcVertices(const glm::mat4 &viewMatrix)
{
    glm::mat4 rot = glm::mat4(
        glm::vec4(viewMatrix[0].x, viewMatrix[0].y, viewMatrix[0].z, 0),
        glm::vec4(viewMatrix[1].x, viewMatrix[1].y, viewMatrix[1].z, 0),
        glm::vec4(viewMatrix[2].x, viewMatrix[2].y, viewMatrix[2].z, 0),
        glm::vec4(0, 0, 0, 1));

    rot = glm::transpose(rot);

    rot = glm::rotate(glm::mat4(1.0f), DEG_TO_RAD(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) * rot;
    rot = glm::rotate(glm::mat4(1.0f), DEG_TO_RAD(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * rot;
    rot = glm::rotate(glm::mat4(1.0f), DEG_TO_RAD(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) * rot;

    glm::vec4 vector = glm::translate(pos) * glm::scale(scale) * rot * glm::vec4(vertices[0].x, vertices[0].y, vertices[0].z, 1);
    finalVertices[0].x = vector.x;
    finalVertices[0].y = vector.y;
    finalVertices[0].z = vector.z;
    finalVertices[0].r = color.x;
    finalVertices[0].g = color.y;
    finalVertices[0].b = color.z;
    finalVertices[0].a = fade;

    vector = glm::translate(pos) * glm::scale(scale) * rot * glm::vec4(vertices[1].x, vertices[1].y, vertices[1].z, 1);
    finalVertices[1].x = vector.x;
    finalVertices[1].y = vector.y;
    finalVertices[1].z = vector.z;
    finalVertices[1].r = color.x;
    finalVertices[1].g = color.y;
    finalVertices[1].b = color.z;
    finalVertices[1].a = fade;

    vector = glm::translate(pos) * glm::scale(scale) * rot * glm::vec4(vertices[2].x, vertices[2].y, vertices[2].z, 1);
    finalVertices[2].x = vector.x;
    finalVertices[2].y = vector.y;
    finalVertices[2].z = vector.z;
    finalVertices[2].r = color.x;
    finalVertices[2].g = color.y;
    finalVertices[2].b = color.z;
    finalVertices[2].a = fade;

    vector = glm::translate(pos) * glm::scale(scale) * rot * glm::vec4(vertices[3].x, vertices[3].y, vertices[3].z, 1);
    finalVertices[3].x = vector.x;
    finalVertices[3].y = vector.y;
    finalVertices[3].z = vector.z;
    finalVertices[3].r = color.x;
    finalVertices[3].g = color.y;
    finalVertices[3].b = color.z;
    finalVertices[3].a = fade;

    vector = glm::translate(pos) * glm::scale(scale) * rot * glm::vec4(vertices[4].x, vertices[4].y, vertices[4].z, 1);
    finalVertices[4].x = vector.x;
    finalVertices[4].y = vector.y;
    finalVertices[4].z = vector.z;
    finalVertices[4].r = color.x;
    finalVertices[4].g = color.y;
    finalVertices[4].b = color.z;
    finalVertices[4].a = fade;

    vector = glm::translate(pos) * glm::scale(scale) * rot * glm::vec4(vertices[5].x, vertices[5].y, vertices[5].z, 1);
    finalVertices[5].x = vector.x;
    finalVertices[5].y = vector.y;
    finalVertices[5].z = vector.z;
    finalVertices[5].r = color.x;
    finalVertices[5].g = color.y;
    finalVertices[5].b = color.z;
    finalVertices[5].a = fade;
}

void Particle::setPos(const glm::vec3 &inPos)
{
    pos = inPos;
}

void Particle::setRotation(const glm::vec3 &inRotion)
{
    rotation = inRotion;
}

void Particle::setVelocity(const glm::vec3 &inVelocity)
{
    velocity = inVelocity;
}

void Particle::setColor(const glm::vec3 &inColor)
{
    color = inColor;
}

void Particle::setScale(const glm::vec3 &inScale)
{
    scale = inScale;
}

void Particle::setLife(float inLife)
{
    life = inLife;
}

void Particle::setFade(float inFade)
{
    fade = inFade;
}

void Particle::resetTimeAlive()
{
    timeAlive = 0;
}

glm::vec3 Particle::getScale() const
{
    return scale;
}

float Particle::getLife() const
{
    return life;
}

unsigned char Particle::getFade() const
{
    return fade;
}

float Particle::getTimeAlive() const
{
    return timeAlive;
}

glm::vec3 Particle::getVelocity() const
{
    return velocity;
}

glm::vec3 Particle::getPos() const
{
    return pos;
}

/**
 * updates particle position
 * updates vertices
 * @param dt delta time between each update cycle
 * @param viewMatrix the view matrix used to construct billboard
 * */
void Particle::update(float dt, const glm::mat4 &viewMatrix)
{
    timeAlive += dt;
    pos += velocity * dt;
    this->_calcVertices(viewMatrix);
}