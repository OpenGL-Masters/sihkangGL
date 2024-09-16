#version 330 core

in vec4 vertexColor;
in vec2 texCoord;

out vec4 fragColor;

// texture 2D가 glsl에서는 sampler2D라는 이름으로 설정.
uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	fragColor = texture(tex1, texCoord) * 0.8 + texture(tex2, texCoord) * 0.2;
}