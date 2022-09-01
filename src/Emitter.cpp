#include "Emitter.h"
#include "affectors/ScaleAffector.h"
#include "affectors/FadeAffector.h"
#include "affectors/AddVelocityAffector.h"
#include <iostream>

#define RAND_RANGE(X, Y) ((X) + (rand() % ((Y) + abs(X))))

Emitter::Emitter(const std::string &file)
{
    this->loadXML(file);

    vertices = (Vertex *)malloc(sizeof(Vertex) * 6 * NUMPARTICLES);

    transform = glm::mat4(1);

    if (mode == Burst)
    {
        this->_calcBurstTime();
    }

    // init all particles and place them in free list
    pParticles = new Particle[NUMPARTICLES];
    for (int i = 0; i < NUMPARTICLES; i++)
    {
        _addToFree(&pParticles[i]);
        pParticles[i].setRotation(particleRot);
    }

    this->init();
}

Emitter::~Emitter()
{
    free(vertices);
    delete decl;
    delete[] pParticles;
    wolf::MaterialManager::DestroyMaterial(material);
    wolf::BufferManager::DestroyBuffer(vertBuffer);
}

void Emitter::init()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    vertBuffer = wolf::BufferManager::CreateVertexBuffer(vertices, sizeof(Vertex) * NUMPARTICLES * 6);
    decl = new wolf::VertexDeclaration();
    decl->Begin();
    decl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    decl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
    decl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
    decl->SetVertexBuffer(vertBuffer);
    decl->End();

    material = wolf::MaterialManager::CreateMaterial(matName);
    material->SetProgram("data/particle.vsh", "data/particle.fsh");
    material->SetDepthTest(true);
    material->SetDepthFunc(wolf::DF_LessEqual);
    material->SetDepthWrite(false);

    if (blending == Additive)
    {
        material->SetBlend(true);
        material->SetBlendMode(wolf::BM_SrcAlpha, wolf::BM_DstAlpha);
    }
    else if (blending == Fade)
    {
        material->SetBlend(true);
        material->SetBlendMode(wolf::BM_SrcAlpha, wolf::BM_OneMinusSrcAlpha);
    }

    material->SetTexture("tex", wolf::TextureManager::CreateTexture(texture));
}

void Emitter::_addToFree(Particle *particle)
{
    if (freeListHead == NULL) // first insertion check
    {
        freeListHead = particle;
    }
    else
    {
        particle->next = freeListHead;
        particle->prev = NULL;
        freeListHead->prev = particle;
        freeListHead = particle; // update head
    }
}

void Emitter::setPos(const glm::vec3 &inPos)
{
    emitterPos = inPos;
}

void Emitter::setParticlePos(const glm::vec3 &inPos)
{
    particlePos = inPos;
}

void Emitter::setRot(const glm::vec3 &inRot)
{
    emitterRot = inRot;
}

/**
 * spawns a particle by removing a particle from the free list and adding it to the empty list
 * also responsible for assigning particles initial values
 * */
void Emitter::spawnParticle()
{
    Particle *particle = getFreeParticle();
    _addToActive(particle);

    // default shape
    if (emitterType == Point)
    {
        particle->setPos(particlePos);
    }
    // render inside a sphere area
    else if (emitterType == Sphere)
    {
        float x, y, z;
        x = rand();
        y = rand();
        z = rand();

        float mag = sqrt(x * x + y * y + z * z);
        x /= mag;
        y /= mag;
        z /= mag;

        // Math.cbrt is cube root
        float c = cbrt(radius);

        int positiveOrNegative = rand() % 2;

        positiveOrNegative == 1 ? x = (emitterPos.x + x * c) : x = (emitterPos.x - x * c);

        positiveOrNegative = rand() % 2;

        positiveOrNegative == 1 ? y = (emitterPos.y + y * c) : y = (emitterPos.y - y * c);

        positiveOrNegative = rand() % 2;

        positiveOrNegative == 1 ? z = (emitterPos.z + z * c) : z = (emitterPos.z - z * c);

        particle->setPos(glm::vec3(x, y, z));
    }
    //  render on the edge of the circle
    else if (emitterType == CircleEdge)
    {
        float x, z;

        int angle = rand();
        x = radius * cos(angle);
        z = radius * sin(angle);

        particle->setPos(glm::vec3(emitterPos.x + x, emitterPos.y, emitterPos.z + z));
    }
    // render inside a box area
    else
    {
        float xoffset, yoffset, zoffset;

        // generate a random number from 0.0 to 1
        xoffset = ((float)(rand())) / RAND_MAX;
        yoffset = ((float)(rand())) / RAND_MAX;
        zoffset = ((float)(rand())) / RAND_MAX;

        xoffset = xoffset * boxX;
        yoffset = yoffset * boxY;
        zoffset = zoffset * boxZ;

        int positiveOrNegative = rand() % 2;

        positiveOrNegative == 1 ? xoffset = (emitterPos.x + xoffset) : xoffset = (emitterPos.x - xoffset);

        positiveOrNegative = rand() % 2;

        positiveOrNegative == 1 ? yoffset = (emitterPos.y + yoffset) : yoffset = (emitterPos.y - yoffset);

        positiveOrNegative = rand() % 2;

        positiveOrNegative == 1 ? zoffset = (emitterPos.z + zoffset) : zoffset = (emitterPos.z - zoffset);

        particle->setPos(glm::vec3(xoffset, yoffset, zoffset));
    }

    // color
    if (randomColor)
    {
        int r, g, b;

        if (minColor.r == maxColor.r && minColor.r == 0)
        {
            r = 0;
        }
        else
        {
            r = RAND_RANGE((int)minColor.r, (int)maxColor.r);
        }

        if (minColor.g == maxColor.g && minColor.g == 0)
        {
            g = 0;
        }
        else
        {
            g = RAND_RANGE((int)minColor.g, (int)maxColor.g);
        }

        if (minColor.b == maxColor.b && minColor.b == 0)
        {
            b = 0;
        }
        else
        {
            b = RAND_RANGE((int)minColor.b, (int)maxColor.b);
        }

        particle->setColor(glm::vec3(r, g, b));
    }
    else
    {
        particle->setColor(color);
    }

    // velocity
    if (randomVelocity)
    {
        int x, y, z;

        if (minVelocity.x == maxVelocity.x && minVelocity.x == 0)
        {
            x = 0;
        }
        else
        {
            x = RAND_RANGE((int)minVelocity.x, (int)maxVelocity.x);
        }

        if (minVelocity.y == maxVelocity.y && minVelocity.y == 0)
        {
            y = 0;
        }
        else
        {
            y = RAND_RANGE((int)minVelocity.y, (int)maxVelocity.y);
        }

        if (minVelocity.z == maxVelocity.z && minVelocity.z == 0)
        {
            z = 0;
        }
        else
        {
            z = RAND_RANGE((int)minVelocity.z, (int)maxVelocity.z);
        }

        particle->setVelocity(glm::vec3(x, y, z));
    }
    else
    {
        particle->setVelocity(glm::vec3(0, 0, 0));
    }

    // scale
    if (randomScale)
    {
        int scale;

        if (minScale == maxScale && maxScale == 0)
        {
            scale = 0;
        }
        else
        {
            scale = RAND_RANGE(minScale, maxScale);
        }

        particle->setScale(glm::vec3(scale, scale, scale));
    }
    else
    {
        particle->setScale(glm::vec3(scale, scale, scale));
    }

    // particle lifetime
    if (randomLife)
    {
        if (minLife == maxLife && minLife == 0)
        {
            lifetime = 0;
        }
        else
        {
            lifetime = RAND_RANGE(minLife, maxLife);
        }

        particle->setLife(lifetime);
    }
    else
    {
        particle->setLife(lifetime);
    }

    // particle fade
    if (randomFade)
    {
        if (minFade == maxFade && minFade == 0)
        {
            fade = 0;
        }
        else
        {
            fade = RAND_RANGE(minFade, maxFade);
        }

        particle->setFade(fade);
    }
    else
    {
        particle->setFade(fade);
    }
}

void Emitter::_addToActive(Particle *particle)
{
    if (activeListHead == NULL) // check for first insertion
    {
        activeListTail = particle;
        activeListHead = particle;
    }
    else
    {
        particle->next = activeListHead;
        particle->prev = NULL;
        activeListHead->prev = particle;
        activeListHead = particle; // update head
    }
    numberOfActiveParticles++;
}

/**
 * used when a particle has expired
 * removes particle from active list and places it in free list
 * */
void Emitter::particleKilled(Particle *particle)
{
    removeFromActive(particle);
    _addToFree(particle);
    particle->resetTimeAlive();
}

Particle *Emitter::getFreeParticle()
{
    // if free list is emtpy
    if (freeListHead == NULL)
    {
        particleKilled(activeListTail); // kill oldest particle and put it in free list
    }

    Particle *p = freeListHead;

    if (freeListHead->next != NULL)
    {
        freeListHead->next->prev = NULL;
        freeListHead = freeListHead->next;
    }
    else
    {
        freeListHead->next = NULL;
        freeListHead->prev = NULL;
        freeListHead = NULL;
    }

    p->prev = NULL;
    p->next = NULL;

    return p;
}

void Emitter::removeFromActive(Particle *particle)
{
    if (activeListTail == particle)
    {
        if (activeListTail != activeListHead) // last particle in the list
        {
            activeListTail = activeListTail->prev;
        }
        else // only particle in the list
        {
            activeListHead = NULL;
            ;
            activeListTail = NULL;
        }
    }
    else if (activeListHead == particle)
    {
        if (particle->next)
        {
            activeListHead = particle->next;
        }
        else
        {
            activeListHead = NULL;
        }
    }
    else
    {
        particle->next->prev = particle->prev;
        particle->prev->next = particle->next;
    }

    particle->next = NULL;
    particle->prev = NULL;

    numberOfActiveParticles--;
}

/**
 * updates all particles
 * kills particles that have expired
 * applies all affectors
 * @param viewMatrix the view matrix of the camera to construct billboard from
 * */
void Emitter::_updateParticles(float dt, const glm::mat4 &viewMatrix)
{
    Particle *particle = activeListHead;

    int j = 0;
    for (int i = 0; i < numberOfActiveParticles; i++)
    {
        if (particle->getTimeAlive() >= particle->getLife())
        {
            Particle *p = particle; // temp variable

            // update current particle before killing it
            if (particle->next)
            {
                particle = particle->next;
            }

            particleKilled(p);
        }

        for (Affector *affector : affectors)
        {
            affector->Apply(dt, particle);
        }

        particle->update(dt, viewMatrix);

        // loop 6 times for each particle
        int k = j + 6;
        while (j < k)
        {
            vertices[j] = particle->getVertices()[j % 6];
            j++;
        }

        particle = particle->next;
    }
}

/**
 * stops spawning particles if emitter is expired
 * spawns particles and writes new vertices data to vertex buffer every frame
 * @param viewMatrix the view matrix of the camera to construct billboard from
 * */
void Emitter::update(float dt, const glm::mat4 &viewMatrix)
{
    int numOfSpawns = 0;

    if (mode == Continuous)
    {
        if (randomBirthRate)
        {
            birthRate = RAND_RANGE(minBirthRate, maxBirthRate);
        }

        toSpawnAccumulator += birthRate * dt;
        numOfSpawns = int(toSpawnAccumulator);
        toSpawnAccumulator -= numOfSpawns;
    }
    else if (mode == Burst)
    {
        timeToBurst -= dt;

        if (timeToBurst <= 0)
        {
            if (randomBurstSpawn)
            {
                numOfSpawns = RAND_RANGE(minBurstSpawn, maxBurstSpawn);
            }
            else
            {
                numOfSpawns = burstSpawns;
            }

            if (repeatBurst)
            {
                this->_calcBurstTime();
            }
            else
            {
                timeToBurst = 9999; // infinity
            }
        }
    }

    if (duration >= 0 || duration == -1)
    {
        for (int i = 0; i < numOfSpawns; i++)
        {
            this->spawnParticle();
        }
    }

    // update remaining life of the emitter
    if (duration != -1)
    {
        duration -= dt;
    }

    // update vertices, check lifetime, apply affectors
    this->_updateParticles(dt, viewMatrix);

    // update vertex buffer
    vertBuffer->Write(vertices, sizeof(Vertex) * 6 * numberOfActiveParticles);
}

void Emitter::_calcBurstTime()
{
    if (randomBurstTime)
    {
        timeToBurst = RAND_RANGE(minBurstTime, maxBurstTime);
    }
    else
    {
        timeToBurst = burstTime;
    }
}

void Emitter::render(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix)
{
    glm::mat4 worldMatrix = glm::rotate(glm::mat4(1.0f), DEG_TO_RAD(emitterRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    worldMatrix = worldMatrix * glm::rotate(glm::mat4(1.0f), DEG_TO_RAD(emitterRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    worldMatrix = worldMatrix * glm::rotate(glm::mat4(1.0f), DEG_TO_RAD(emitterRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
    worldMatrix = worldMatrix * glm::translate(emitterPos);

    decl->Bind();

    material->SetUniform("projection", projMatrix);
    material->SetUniform("view", viewMatrix);
    material->SetUniform("world", worldMatrix);
    material->Apply();

    glDrawArrays(GL_TRIANGLES, 0, 6 * numberOfActiveParticles);
}

/**
 * reads xml script to init all required class members
 * @param file path to the xml script
 * */
void Emitter::loadXML(const std::string &file)
{
    const char *type;

    TiXmlDocument doc(file.c_str());

    if (doc.LoadFile() == false)
    {
        printf("error loading xml emitter file \n");
    }

    // Look for the root "Emitter" node and print error if missing
    TiXmlNode *node = doc.FirstChild("Emitter");
    if (node == NULL)
    {
        printf("error loading xml emitter file \n");
    }

    TiXmlNode *childNode = node->FirstChild();

    if (childNode)
    {
        const char *spawnMode;
        const char *blendMode;

        TiXmlElement *element = childNode->ToElement();

        matName = element->Attribute("matName");
        texture = element->Attribute("texture");
        blendMode = element->Attribute("blending");

        element->QueryIntAttribute("num_particles", &NUMPARTICLES);
        element->QueryFloatAttribute("duration", &duration);
        type = element->Attribute("type");
        spawnMode = element->Attribute("mode");

        element->QueryFloatAttribute("rotationX", &emitterRot.x);
        element->QueryFloatAttribute("rotationY", &emitterRot.y);
        element->QueryFloatAttribute("rotationZ", &emitterRot.z);

        if (strcmp(blendMode, "additive") == 0)
        {
            blending = Additive;
        }
        else if (strcmp(blendMode, "fade") == 0)
        {
            blending = Fade;
        }
        else
        {
            blending = None;
        }

        if (strcmp(spawnMode, "continuous") == 0)
        {
            mode = Continuous;

            element->QueryBoolAttribute("randomBirthRate", &randomBirthRate);

            if (randomBirthRate)
            {
                element->QueryIntAttribute("minBirthRate", &minBirthRate);
                element->QueryIntAttribute("maxBirthRate", &maxBirthRate);
            }
            else
            {
                element->QueryIntAttribute("birthrate", &birthRate);
            }
        }
        else
        {
            mode = Burst;

            element->QueryBoolAttribute("randomBurstSpawn", &randomBurstSpawn);

            if (randomBurstSpawn)
            {
                element->QueryIntAttribute("minBurstSpawn", &minBurstSpawn);
                element->QueryIntAttribute("maxBurstSpawn", &maxBurstSpawn);
            }
            else
            {
                element->QueryIntAttribute("burstSpawns", &burstSpawns);
            }

            element->QueryBoolAttribute("repeatBurst", &repeatBurst);

            element->QueryBoolAttribute("randomBurstTime", &randomBurstTime);

            if (randomBurstTime)
            {
                element->QueryIntAttribute("minBurstTime", &minBurstTime);
                element->QueryIntAttribute("maxBurstTime", &maxBurstTime);
            }
            else
            {
                element->QueryFloatAttribute("burstTime", &burstTime);
            }
        }

        if (strcmp(type, "point") == 0)
        {
            emitterType = Point;
        }
        else if (strcmp(type, "box") == 0)
        {
            emitterType = Box;
            element->QueryFloatAttribute("width", &boxX);
            element->QueryFloatAttribute("height", &boxY);
            element->QueryFloatAttribute("depth", &boxZ);
        }
        else if (strcmp(type, "circleEdge") == 0)
        {
            emitterType = CircleEdge;
            element->QueryFloatAttribute("radius", &radius);
        }
        else
        {
            emitterType = Sphere;
            element->QueryFloatAttribute("radius", &radius);
        }
    }

    childNode = childNode->NextSibling("spawn_property");

    while (childNode)
    {
        TiXmlElement *element = childNode->ToElement();
        const char *name;

        name = element->Attribute("name");
        // velocity
        if (strcmp(name, "velocity") == 0)
        {
            type = element->Attribute("type");

            if (strcmp(type, "random") == 0)
            {
                randomVelocity = true;

                // min
                element->QueryFloatAttribute("minX", &minVelocity.x);
                element->QueryFloatAttribute("minY", &minVelocity.y);
                element->QueryFloatAttribute("minZ", &minVelocity.z);
                // max
                element->QueryFloatAttribute("maxX", &maxVelocity.x);
                element->QueryFloatAttribute("maxY", &maxVelocity.y);
                element->QueryFloatAttribute("maxZ", &maxVelocity.z);
            }
            else
            {
                randomVelocity = false;
                element->QueryFloatAttribute("x", &velocity.x);
                element->QueryFloatAttribute("y", &velocity.y);
                element->QueryFloatAttribute("z", &velocity.z);
            }
        }
        //rotation
        else if (strcmp(name, "rotation") == 0)
        {
            element->QueryFloatAttribute("x", &particleRot.x);
            element->QueryFloatAttribute("y", &particleRot.y);
            element->QueryFloatAttribute("z", &particleRot.z);
        }
        // color
        else if (strcmp(name, "color") == 0)
        {
            type = element->Attribute("type");

            // rgb components can have different individual min and max values
            if (strcmp(type, "randomRGB") == 0)
            {
                randomColor = true;

                // min
                element->QueryFloatAttribute("minR", &minColor.r);
                element->QueryFloatAttribute("minG", &minColor.g);
                element->QueryFloatAttribute("minB", &minColor.b);
                // max
                element->QueryFloatAttribute("maxR", &maxColor.r);
                element->QueryFloatAttribute("maxG", &maxColor.g);
                element->QueryFloatAttribute("maxB", &maxColor.b);
            }
            else if (strcmp(type, "random") == 0) // rgb components have the same min and max values
            {
                randomColor = true;

                // min
                element->QueryFloatAttribute("min", &minColor.r);
                element->QueryFloatAttribute("min", &minColor.g);
                element->QueryFloatAttribute("min", &minColor.b);
                // max
                element->QueryFloatAttribute("max", &maxColor.r);
                element->QueryFloatAttribute("max", &maxColor.g);
                element->QueryFloatAttribute("max", &maxColor.b);
            }
            else
            {
                randomColor = false;
                element->QueryFloatAttribute("valueR", &color.r);
                element->QueryFloatAttribute("valueG", &color.g);
                element->QueryFloatAttribute("valueB", &color.b);
            }
        }
        // scale
        else if (strcmp(name, "scale") == 0)
        {
            type = element->Attribute("type");

            if (strcmp(type, "random") == 0)
            {
                randomScale = true;

                // min
                element->QueryIntAttribute("min", &minScale);
                // max
                element->QueryIntAttribute("max", &maxScale);
            }
            else
            {
                randomScale = false;
                element->QueryIntAttribute("value", &scale);
            }
        }
        // lifetime
        else if (strcmp(name, "life") == 0)
        {
            type = element->Attribute("type");

            if (strcmp(type, "random") == 0)
            {
                randomLife = true;

                // min
                element->QueryIntAttribute("min", &minLife);
                // max
                element->QueryIntAttribute("max", &maxLife);
            }
            else
            {
                randomLife = false;
                element->QueryFloatAttribute("value", &lifetime);
            }
        }
        // fade
        else
        {
            type = element->Attribute("type");

            if (strcmp(type, "random") == 0)
            {
                randomFade = true;
                element->QueryIntAttribute("min", &minFade);
                element->QueryIntAttribute("max", &maxFade);
            }
            else
            {
                randomFade = false;
                element->QueryIntAttribute("value", &fade);
            }
        }

        if (!childNode->NextSibling("spawn_property"))
        {
            break;
        }

        childNode = childNode->NextSibling("spawn_property");
    }

    // affectors
    while (childNode)
    {
        childNode = childNode->NextSibling("affector");

        if (!childNode)
        {
            break;
        }

        childNode = childNode->FirstChild();

        TiXmlElement *element = childNode->ToElement();

        type = element->Attribute("type");

        if (strcmp(type, "scale") == 0)
        {
            childNode = childNode->NextSibling();
            element = childNode->ToElement();

            float startValue, endValue;

            element->QueryFloatAttribute("startValue", &startValue);

            childNode = childNode->NextSibling();
            element = childNode->ToElement();

            element->QueryFloatAttribute("endValue", &endValue);

            ScaleAffector *affector = new ScaleAffector(startValue, endValue);
            affectors.push_back(affector);
        }

        else if (strcmp(type, "addVelocity") == 0)
        {
            childNode = childNode->NextSibling();
            element = childNode->ToElement();

            float x, y, z;

            element->QueryFloatAttribute("x", &x);

            childNode = childNode->NextSibling();
            element = childNode->ToElement();

            element->QueryFloatAttribute("y", &y);

            childNode = childNode->NextSibling();
            element = childNode->ToElement();

            element->QueryFloatAttribute("z", &z);

            AddVelocityAffector *affector = new AddVelocityAffector(x, y, z);
            affectors.push_back(affector);
        }

        else
        {
            childNode = childNode->NextSibling();
            element = childNode->ToElement();

            float startValue, endValue;

            element->QueryFloatAttribute("startValue", &startValue);

            childNode = childNode->NextSibling();
            element = childNode->ToElement();

            element->QueryFloatAttribute("endValue", &endValue);

            FadeAffector *affector = new FadeAffector(startValue, endValue);
            affectors.push_back(affector);
        }

        childNode = childNode->Parent();
    }
}