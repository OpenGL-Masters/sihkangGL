#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shader)
{
	auto program = ProgramUPtr(new Program());
	if (!program->Link(shader))
		return nullptr;
	return std::move(program);
}

bool Program::Link(const std::vector<ShaderPtr>& shaders)
{
	// 프로그램의 정수번호 할당.
	m_program = glCreateProgram();

	// 프로그램에 사용될 쉐이더 ID를 붙여주기.
	for (auto &shader: shaders)
		glAttachShader(m_program, shader->Get());
	glLinkProgram(m_program);

	int success = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
		SPDLOG_ERROR("failed to link program: {}", infoLog);
		return false;
	}
	return true;

}

Program::~Program()
{
	if (m_program)
	{
		glDeleteProgram(m_program);
	}
}

void Program::Use() const
{
	glUseProgram(m_program);
}