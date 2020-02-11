#include "ScreenQuad.h"
#include <glm/glm.hpp>

struct Vertex {
    glm::vec4 pos;
    glm::vec2 texCoor;
};

ScreenQuad::~ScreenQuad()
{
    glDeleteBuffers(1, &m_vertexUVBufferID);
}

ScreenQuad::ScreenQuad(Shader& shader) :
    m_VFShader(shader)
{
    assert(glGetError() == GL_NO_ERROR);

    glGenVertexArrays(1, &m_VertexArrayID);
    glBindVertexArray(m_VertexArrayID);
    glEnableVertexAttribArray(0);

    struct Vertex Vertices[] = {
        {glm::vec4(-1,-1,0,1), glm::vec2(0,0)}, {glm::vec4(1,-1,0,1), glm::vec2(1,0)},
        {glm::vec4(-1,1,0,1), glm::vec2(0,1)}, {glm::vec4(1,1,0,1), glm::vec2(1,1)} };

    glGenBuffers(1, &m_vertexUVBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexUVBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

void ScreenQuad::draw(Texture& textureToRender)
{
    m_VFShader.Bind();
    glActiveTexture(GL_TEXTURE0 + textureToRender.GetBindPoint());

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexUVBufferID);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(glm::vec4));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}