#include "context.h"

ContextUPtr Context::Create()
{
	auto context = ContextUPtr(new Context());
	if (!context->Init())
		return nullptr;
	return std::move(context);
}

bool Context::Init()
{
	// float vertices[] = {
	// 	0.5f, 0.5f, 0.0f,
	// 	0.5f, -0.5f, 0.0f,
	// 	-0.5f, -0.5f, 0.0f,
	// 	-0.5f, 0.5f, 0.0f,
	// };

	float vertices[] = {
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top right, red
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right, green
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottomleft, blue
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // top left, yellow
	};

	uint32_t indices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	// VertexLayout 클래스 리팩토링으로 아래 주석 대체
	m_vertexLayout = VertexLayout::Create();
// 	// VAO -> VBO 순으로 작성하기.
// 	// glGenVertexArrays(1, &m_vertexArrayObject);
// 	// glBindVertexArray(m_vertexArrayObject);


	// refactoring 으로 m_vertexBuffer를 클래스로 생성.
	m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, 
				GL_STATIC_DRAW, vertices, sizeof(float) * 24);
	// glGenBuffers(1, &m_vertexBuffer);
	// glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);

	m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6 , 0); // for location
	m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6 , sizeof(float) * 3); // for color

	// refactoring 으로 m_indexBuffer를 클래스로 생성.
	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);
	// glGenBuffers(1, &m_indexBuffer);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 6, indices, GL_STATIC_DRAW);


	// 두 함수 호출 순서에 주의
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	// glEnableVertexAttribArray(0);
	// glEnableVertexAttribArray(1);

	const char* vertexShaderPath = "/Users/sihwan/Programming/sihkang_GL/openGL_example/shader/per_vertex_color.vs";
	const char* fragmentShaderPath = "/Users/sihwan/Programming/sihkang_GL/openGL_example/shader/per_vertex_color.fs";

	ShaderPtr vertexShader = Shader::CreateFromFile(vertexShaderPath, GL_VERTEX_SHADER);
	ShaderPtr fragmentShader = Shader::CreateFromFile(fragmentShaderPath, GL_FRAGMENT_SHADER);
	
	if (!vertexShader || !fragmentShader)
		return false;
	
	SPDLOG_INFO("vertex shader id: {}", vertexShader->Get());
	SPDLOG_INFO("fragment shader id: {}", fragmentShader->Get());

	m_program = Program::Create({fragmentShader, vertexShader});
	if (!m_program)
		return false;
	SPDLOG_INFO("Program id: {}", m_program->Get());

	// uniform 값을 넣어보자.
	// auto loc = glGetUniformLocation(m_program->Get(), "color"); // color라는 유니폼 변수의 위치를 찾아옴
	// m_program->Use(); // 현재 context로 프로그램 등록
	// glUniform4f(loc, 1.0f, 1.0f, 0.0f, 1.0f);

	// 컬러 프레임버퍼 화면을 클리어할 색상 지정
	glClearColor(0.0f, 0.1f, 0.2f, 0.0f);
	return true;
}

void Context::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// static float time = 0.0f;
	// float t = sinf(time) * 0.5f + 0.5f;
	// auto loc = glGetUniformLocation(m_program->Get(), "color");
	m_program->Use();
	// glUniform4f(loc, t*t, 2.0f * t * (1.0f -t ), (1.0f - t)*(1.0f - t), 1.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// time += 0.016f;
}