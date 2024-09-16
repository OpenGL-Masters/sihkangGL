#include "context.h"
#include "image.h"

ContextUPtr Context::Create()
{
	auto context = ContextUPtr(new Context());
	if (!context->Init())
		return nullptr;
	return std::move(context);
}

bool Context::Init()
{
	float vertices[] = {
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	};

	uint32_t indices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	m_vertexLayout = VertexLayout::Create();

	m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, 
				GL_STATIC_DRAW, vertices, sizeof(float) * 32);
	
	m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0); // for location
	m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3); // for color
	m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6); // for texture coord 2D.

	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);
	
	const char* vertexShaderPath = "/Users/sihwan/Programming/sihkang_GL/openGL_example/shader/texture.vs";
	const char* fragmentShaderPath = "/Users/sihwan/Programming/sihkang_GL/openGL_example/shader/texture.fs";

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

	glClearColor(0.0f, 0.1f, 0.2f, 0.0f);
	
	const char * imagePath = "/Users/sihwan/Programming/sihkang_GL/openGL_example/image/container.jpg";
	const char * imagePath2 = "/Users/sihwan/Programming/sihkang_GL/openGL_example/image/image.png";
	auto image = Image::Load(imagePath);
	if (!image)
		return false;
	SPDLOG_INFO("image: {} x {}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());
	// auto image = Image::Create(512,512);
	// image->SetCheckImage(64, 128);
	
	m_texture1 = Texture::CreateFromImage(image.get());
	auto image2 = Image::Load(imagePath2);
	m_texture2 = Texture::CreateFromImage(image2.get());
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture1->Get());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

	m_program->Use();
	glUniform1i(glGetUniformLocation(m_program->Get(), "tex1"), 0);
	glUniform1i(glGetUniformLocation(m_program->Get(), "tex2"), 1);

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