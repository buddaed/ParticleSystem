#include "SampleFireEffect.h"

SampleFireEffectRunner::~SampleFireEffectRunner()
{
	printf("Destroying fire effect Sample\n");
	delete camera;
}

void SampleFireEffectRunner::init()
{
	camera = new Camera(m_pApp);
	effect = new Effect("data/effect.xml", glm::vec3(0,0,0));

	camera->setPos(glm::vec3(0,0,20));
}

void SampleFireEffectRunner::update(float dt)
{
	camera->update(dt);
	effect->update(dt, camera->getViewMatrix());
}

void SampleFireEffectRunner::render(int width, int height)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewMatrix = camera->getViewMatrix();
	const glm::mat4 projMatrix = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 1000.0f);

	effect->render(viewMatrix, projMatrix);
}
