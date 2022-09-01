#pragma once
#include "../wolf/wolf.h"
#include "camera.h"
#include "Effect.h"
#include "Emitter.h"
#include "../samplefw/Sample.h"

class SampleFireEffectRunner: public Sample
{
public:
    SampleFireEffectRunner(wolf::App* pApp) : Sample(pApp,"Plane") {}
    ~SampleFireEffectRunner();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:
    Camera* camera;
    Emitter* emitter;
    Effect* effect;
};
