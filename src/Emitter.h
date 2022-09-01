#pragma once
#include "tinyxml.h"
#include "affectors/Affector.h"
#include "Particle.h"
#include "../wolf/wolf.h"
#include <list>

//the way the particles are spawned
enum EmissionMode
{
    Continuous,
    Burst
};

//the shape the emitter will spawn particles in
enum Type
{
    Point,
    Sphere,
    CircleEdge, //spawns particles on the edge of the circle and leaves middle empty
    Box
};

//different blendmodes to achieve a broader range of effects    
enum BlendMode
{
    Additive,
    Fade,
    None
};

class Emitter
{
public:
    Emitter(const std::string &);
    ~Emitter();

    void init();
    void update(float dt, const glm::mat4 &);
    void render(const glm::mat4 &, const glm::mat4 &);
    void loadXML(const std::string &);

    void setPos(const glm::vec3 &);
    void setRot(const glm::vec3 &);
    void setParticlePos(const glm::vec3 &);

    void spawnParticle();
    void particleKilled(Particle *);
    void removeFromActive(Particle *);
    Particle *getFreeParticle();

private:
    void _addToFree(Particle *);
    void _addToActive(Particle *);
    void _calcBurstTime();
    void _updateParticles(float, const glm::mat4 &);

    EmissionMode mode;
    Type emitterType;
    Particle *pParticles;

    Particle *freeListHead = NULL;
    Particle *activeListHead = NULL;
    Particle *activeListTail = NULL;
    std::list<Affector *> affectors;
    glm::mat4 transform;

    //particle initial properties
    glm::vec3 particlePos = glm::vec3(0, 0, 0);
    glm::vec3 particleRot = glm::vec3(0, 0, 0);
    glm::vec3 color = glm::vec3(1, 0, 0);
    glm::vec3 velocity = glm::vec3(0, 0, 0);

    //emitter properties
    glm::vec3 emitterPos = glm::vec3(0,0,0);
    glm::vec3 emitterRot = glm::vec3(0,0,0);

    Vertex *vertices;
    
    std::string matName, texture;

    int NUMPARTICLES;
    int numberOfActiveParticles = 0;
    float duration; // -1 is infinite

    bool randomColor, randomVelocity, randomScale, randomLife, randomFade; // randomization
    
    glm::vec3 minVelocity, maxVelocity;                                    // velocity
    int scale, minScale, maxScale;                                         // scale
    glm::vec3 minColor, maxColor;                                          // color
    float lifetime;
    int minLife, maxLife;
    int fade, minFade, maxFade;

    // blending
    BlendMode blending;

    // continouos mode varaibles
    int birthRate;
    int minBirthRate, maxBirthRate;
    float toSpawnAccumulator = 0;
    bool randomBirthRate;

    // burst mode varaibles
    float timeToBurst, burstTime;
    int burstSpawns;
    int minBurstTime, maxBurstTime, minBurstSpawn, maxBurstSpawn;
    bool randomBurstTime, randomBurstSpawn, repeatBurst;

    // box emitter varaibles
    float boxX, boxY, boxZ;

    // sphere emitter varaibles
    float radius;

    //wolf variables
    wolf::VertexBuffer *vertBuffer = 0;
    wolf::VertexDeclaration *decl = 0;
    wolf::Material *material = 0;
};