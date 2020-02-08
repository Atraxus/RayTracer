#shader vertex
#version 330 core

layout(location = 0) in vec4 inVertPos;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec4 vertexColor;

out vec2 passTexCoord;
out vec4 fragmentColor;

uniform mat4 u_MVP;
//uniform mat4 u_View;

void main()
{
    // Positions 
    gl_Position = u_MVP * inVertPos ;
    // Texture coordinates
    passTexCoord = inTexCoord;
    // Color
    fragmentColor = vertexColor;
};


#shader fragment
#version 330 core

in vec2 passTexCoord;
in vec4 fragmentColor;

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, passTexCoord);
    color = fragmentColor;
};