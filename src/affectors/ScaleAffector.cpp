#include "ScaleAffector.h"

ScaleAffector::ScaleAffector(float inStartSize, float inEndSize)
{
    endSize = inEndSize;
    startSize = inStartSize;
}

ScaleAffector::~ScaleAffector()
{
}

void ScaleAffector::Apply(float dt, Particle *p)
{
    float timeAlive = p->getTimeAlive();
    float currentSize = 0;
    float sizeDiff = endSize - startSize;

    currentSize = startSize + sizeDiff * (timeAlive / p->getLife());

    p->setScale(glm::vec3(currentSize, currentSize, currentSize));
}