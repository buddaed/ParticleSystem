#pragma once
#include "../Particle.h"

class Affector
{
    public:
    Affector(){}

    virtual ~Affector(){}

    virtual void Apply(float dt, Particle*) = 0;
};