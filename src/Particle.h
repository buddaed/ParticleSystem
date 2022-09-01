#pragma once
#include "../wolf/wolf.h"

struct Vertex
{
    float x, y, z;
    unsigned char r = 255, g = 255, b = 255, a = 255;
    float u, v;


    Vertex()
    {
    }

    Vertex(float inX, float inY, float inZ, unsigned char inR, unsigned char inG, unsigned char inB, unsigned char inA, float inU, float inV)
    {
        x = inX;
        y = inY;
        z = inZ;
        r = inR;
        g = inG;
        b = inB;
        a = inA;
        u = inU;
        v = inV;
    }
};

class Particle
{

public:
    Particle();
    ~Particle();

    void render(const glm::mat4 &, const glm::mat4 &);
    void init();
    void update(float, const glm::mat4 &);

    //setters
    void setPos(const glm::vec3 &);
    void setRotation(const glm::vec3 &);
    void setScale(const glm::vec3 &);
    void setVelocity(const glm::vec3 &);
    void setColor(const glm::vec3 &);
    void setLife(float);
    void setFade(float);
    void resetTimeAlive();
    
    //getters
    float getLife() const;
    float getTimeAlive() const;
    unsigned char getFade() const;
    glm::vec3 getVelocity() const;
    glm::vec3 getScale() const;
    glm::vec3 getPos() const;
    Vertex *getVertices();

    //linked list references
    Particle *prev;
    Particle *next;

private:
    void _calcVertices(const glm::mat4 &);

    // object space vertices before transformations
    Vertex vertices[6] = {
        {-.01, -.01, 0, 1, 0, 0, 1, 0, 0}, // bottom left
        {-.01, .01, 0, 1, 0, 0, 1, 0, 1},  // top left
        {.01, -.01, 0, 1, 0, 0, 1, 1, 0},  // bottom right
        {.01, -.01, 0, 1, 0, 0, 1, 1, 0},  // bottom right
        {.01, .01, 0, 1, 0, 0, 1, 1, 1},   // top right
        {-.01, .01, 0, 1, 0, 0, 1, 0, 1}   // top left
    };

    // object space vertices after transformations
    Vertex finalVertices[6] = {
        {-.01, -.01, 0, 1, 0, 0, 1, 0, 0}, // bottom left
        {-.01, .01, 0, 1, 0, 0, 1, 0, 1},  // top left
        {.01, -.01, 0, 1, 0, 0, 1, 1, 0},  // bottom right
        {.01, -.01, 0, 1, 0, 0, 1, 1, 0},  // bottom right
        {.01, .01, 0, 1, 0, 0, 1, 1, 1},   // top right
        {-.01, .01, 0, 1, 0, 0, 1, 0, 1}   // top left
    };

    glm::vec3 pos;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 color;
    glm::vec3 velocity;

    float life;
    float timeAlive;
    float fade;
};