#include "SampleMagicCircle.h"

SampleMagicCircle::~SampleMagicCircle()
{
	printf("Destroying magic circle Sample\n");
}

void SampleMagicCircle::init()
{
	camera = new Camera(m_pApp);
	effect = new Effect("data/effect2.xml", glm::vec3(0,0,0));
	square = new Square(8.5);

	camera->setPos(glm::vec3(0,0,20));
}

void SampleMagicCircle::update(float dt)
{
	camera->update(dt);
	effect->update(dt, camera->getViewMatrix());
}

void SampleMagicCircle::render(int width, int height)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewMatrix = camera->getViewMatrix();
	const glm::mat4 projMatrix = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 1000.0f);

	square->render(viewMatrix, projMatrix);
	effect->render(viewMatrix, projMatrix);
}
