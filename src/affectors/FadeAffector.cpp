#include "FadeAffector.h"

FadeAffector::FadeAffector(unsigned char inStartFade, unsigned char inEndFade)
{
    startFade = inStartFade;
    endFade = inEndFade;
}

FadeAffector::~FadeAffector()
{
}

void FadeAffector::Apply(float dt, Particle *p)
{
    float currentFade = p->getFade();
    float timeAlive = p->getTimeAlive();
    float fadeDiff = endFade - startFade;

    currentFade = startFade + (fadeDiff * (timeAlive / p->getLife()));

    if (currentFade < 0)
    {
        currentFade = 0;
    }

    p->setFade(currentFade);
}