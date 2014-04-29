#version 130

in vec4 vPosition;
in vec4 vColor;
in vec2 vTexCoord;

out vec4 color;
out vec2 texCoord;

void main()
{
color = Color;
texCoord = vTexCoord;
gl_Position = vPosition;
}
