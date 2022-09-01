#pragma once
#include "tinyxml.h"
#include "../wolf/wolf.h"
#include "Emitter.h"
#include <list>

class Effect
{
    public:

    Effect(const std::string&, const glm::vec3&);
    ~Effect();

    void update(float dt, const glm::mat4&);
    void render(const glm::mat4 &, const glm::mat4 &);
    void loadXML(const std::string&);
    
    void setPos(const glm::vec3&);

    private:
    std::list<Emitter*> emitters;
    glm::mat4 transform;
    glm::vec3 pos;
};