#pragma once
#include "Affector.h"

class FadeAffector : public Affector
{
    public:
    FadeAffector(unsigned char, unsigned char);
    ~FadeAffector();

    void Apply(float dt, Particle *) override;

    unsigned char startFade, endFade;
};