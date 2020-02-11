#shader vertex
#version 430

layout(location = 0) in vec4 currentPos;
layout(location = 1) in vec2 texPos;

out vec2 ex_TexCoor;

void main(void)
{
    gl_Position = currentPos;
    ex_TexCoor = texPos;
}


#shader fragment
#version 430

in vec2 ex_TexCoor;

out vec4 color;

uniform sampler2D texture;

void main(void) {
    color = texture2D(texture, ex_TexCoor);
}