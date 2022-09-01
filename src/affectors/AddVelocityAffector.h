#pragma once
#include "Affector.h"

class AddVelocityAffector : public Affector
{
    public:
    AddVelocityAffector(float, float, float);
    ~AddVelocityAffector();

    void Apply(float dt, Particle *) override;

    private:
    glm::vec3 velocityOffset;
};