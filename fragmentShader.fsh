#version 130

// This is the same old fragment shader we've been using

in vec4 color;
in vec2 texCoord;
out vec4 fColor;

uniform sampler2D texRink;

void main () {
	fColor = color*texture(texRink, texCoord);//uncomment is death
	//fColor = color;
}
