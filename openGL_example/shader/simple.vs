#version 330 core // 3.3 core profile 명시
layout (location = 0) in vec3 aPos;

// shader code 도 메인이 필요.
void main()
{
	gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}