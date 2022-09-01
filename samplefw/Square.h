#pragma once

#include "../wolf/wolf.h"

class Square
{
public:
    Square(float scale);
    ~Square();

    void render(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix);

    glm::vec3 getPos() const;

private:

    struct Vertex
    {
        float x, y, z, u, v;

        Vertex()
        {
        }

        Vertex(float inX, float inY, float inZ, float inU, float inV)
        {
            x = inX;
            y = inY;
            z = inZ;
            u = inU;
            v = inV;
        }
    };
    // object space vertices before transformations
    Vertex vertices[6] = {
        {-1.0f, -1.0f, 0, 0, 0}, // bottom left
        {-1.0f, 1.0f, 0, 0, 1},  // top left
        {1.0f, -1.0f, 0, 1, 0},  // bottom right
        {1.0f, -1.0f, 0, 1, 0},  // bottom right
        {1.0f, 1.0f, 0, 1, 1},   // top right
        {-1.0f, 1.0f, 0, 0, 1}   // top left
    };

    wolf::VertexBuffer *m_pVB;
    wolf::VertexDeclaration *m_pDecl;

    glm::vec3 pos;
    glm::vec3 scale;

    wolf::Material *m_pMat;
};