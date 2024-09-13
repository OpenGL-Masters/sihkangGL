#version 330 core // 3.3 core profile 명시
layout (location = 0) in vec3 aPos;

out vec4 vertexColor;
// shader code 도 메인이 필요.
void main()
{
	gl_Position = vec4(aPos, 1.0);
	vertexColor = vec4(aPos + 0.5, 1.0);
}