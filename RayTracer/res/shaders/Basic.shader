#shader vertex
#version 330 core

layout(location = 0) in vec4 inVertPos;
layout(location = 1) in vec2 inTexCoord;

out vec2 passTexCoord;

uniform mat4 u_MVP;
//uniform mat4 u_View;

void main()
{
    gl_Position = u_MVP * inVertPos ;
    passTexCoord = inTexCoord;
};


#shader fragment
#version 330 core

in vec2 passTexCoord;

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, passTexCoord);
    color = texColor;
};