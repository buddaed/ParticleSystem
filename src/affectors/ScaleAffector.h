#pragma once
#include "Affector.h"

class ScaleAffector : public Affector
{
public:
    ScaleAffector(float, float);
    ~ScaleAffector();

    void Apply(float dt, Particle *) override;

private:
    float endSize;
    float startSize;
};