#include "AddVelocityAffector.h"

AddVelocityAffector::AddVelocityAffector(float x, float y, float z)
{
    velocityOffset = glm::vec3(x,y,z);
}

AddVelocityAffector::~AddVelocityAffector()
{
}

void AddVelocityAffector::Apply(float dt, Particle *p)
{
    glm::vec3 velocity = p->getVelocity();
    velocity += velocityOffset*dt;
    p->setVelocity(velocity);
}