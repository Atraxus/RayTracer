#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// TODO: the error stuff should really get its own class

// assert macro which stops debugger on false (breakpoint)
#define ASSERT(x) if(!(x)) __debugbreak()
// macro for easier error handling in opengl (put almost around every opengl function)
#define GLCall(x) GlClearError();\
    x;\
    ASSERT(GlLogCall(#x, __FILE__, __LINE__))

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);


class Renderer {

public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const ;
    void Clear() const;
private:

};