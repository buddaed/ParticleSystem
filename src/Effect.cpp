#include "Effect.h"

Effect::Effect(const std::string &file, const glm::vec3 &inPos)
{
    pos = inPos;
    loadXML(file);
}

Effect::~Effect()
{
    for(Emitter* emitter: emitters)
    {
        delete emitter;
    }
    emitters.clear();
}

void Effect::update(float dt, const glm::mat4 &viewMatrix)
{
    for (Emitter *emitter : emitters)
    {
        emitter->update(dt, viewMatrix);
    }
}

void Effect::loadXML(const std::string &file)
{
    const char *path;
    glm::vec3 offset;

    TiXmlDocument doc(file.c_str());

    if (doc.LoadFile() == false)
    {
        printf("error loading xml effect file \n");
    }

    // Look for the root "Effect" node and print error if missing
    TiXmlNode *node = doc.FirstChild("Effect");
    if (node == NULL)
    {
        printf("error loading xml effect file \n");
    }

    TiXmlNode *childNode = node->FirstChild();

    while (childNode)
    {
        if (childNode)
        {
            TiXmlElement *element = childNode->ToElement();
            path = element->Attribute("file");
            element->QueryFloatAttribute("xoffset", &offset.x);
            element->QueryFloatAttribute("yoffset", &offset.y);
            element->QueryFloatAttribute("zoffset", &offset.z);
        }

        emitters.push_back(new Emitter(path));
        emitters.back()->setPos(pos + offset);

        childNode = childNode->NextSibling();
    }
}

void Effect::setPos(const glm::vec3 &inPos)
{
    pos = inPos;
}

void Effect::render(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix)
{
    for (Emitter *emitter : emitters)
    {
        emitter->render(viewMatrix, projMatrix);
    }
}
