#pragma once

#include "Texture/Texture.h"
#include "Shader/Shader.h"

class ScreenQuad
{
public:
    ScreenQuad(Shader& shader);
    ~ScreenQuad();

    void draw(Texture& textureToRender);

private:
    GLuint m_vertexUVBufferID;
    GLuint m_VertexArrayID;
    Shader m_VFShader;
};