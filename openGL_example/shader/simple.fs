#version 330 core

out vec4 fragColor;
uniform vec4 color;
in vec4 vertexColor;

void main()
{
	fragColor = color + vertexColor;
}