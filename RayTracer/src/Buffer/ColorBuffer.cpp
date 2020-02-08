#include "ColorBuffer.h"
#include "../Renderer/Renderer.h"

ColorBuffer::ColorBuffer(const void* data, unsigned int size)
    : m_Size(size)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

}

ColorBuffer::~ColorBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void ColorBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void ColorBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
