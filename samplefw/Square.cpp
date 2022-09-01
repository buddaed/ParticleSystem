#include "Square.h"

Square::Square(float inScale)
{
    scale = glm::vec3(inScale, inScale, inScale);
    pos = glm::vec3(0, 0, 0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    m_pVB = wolf::BufferManager::CreateVertexBuffer(vertices, sizeof(Vertex) * 6);
    m_pDecl = new wolf::VertexDeclaration();
    m_pDecl->Begin();
    m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    m_pDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
    m_pDecl->SetVertexBuffer(m_pVB);
    m_pDecl->End();

    m_pMat = wolf::MaterialManager::CreateMaterial("Circle");
    m_pMat->SetProgram("samplefw/data/square.vsh", "samplefw/data/square.fsh");
    m_pMat->SetDepthTest(true);
    m_pMat->SetDepthFunc(wolf::DF_LessEqual);
    m_pMat->SetDepthWrite(false);

    m_pMat->SetTexture("tex", wolf::TextureManager::CreateTexture("data/magicCircle.png"));
}

glm::vec3 Square::getPos() const
{
    return pos;
}

void Square::render(const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix)
{
    glm::mat4 worldMatrix = glm::translate(pos) * glm::rotate(glm::mat4(1.0f), DEG_TO_RAD(0), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(scale);
    
    m_pDecl->Bind();

    m_pMat->SetUniform("projection", projMatrix);
    m_pMat->SetUniform("view", viewMatrix);
    m_pMat->SetUniform("world", worldMatrix);
    m_pMat->Apply();

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

