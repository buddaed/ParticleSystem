#pragma once
#include "../wolf/wolf.h"
#include "camera.h"
#include "Effect.h"
#include "Emitter.h"
#include "../samplefw/Square.h"
#include "../samplefw/Sample.h"

class SampleMagicCircle : public Sample
{
public:
    SampleMagicCircle(wolf::App *pApp) : Sample(pApp, "Plane") {}
    ~SampleMagicCircle();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:
    Camera *camera;
    Emitter *emitter;
    Effect *effect;
    Square *square;
};
