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
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

		0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
	};

	uint32_t indices[] = {
		0,  2,  1,  2,  0,  3,
		4,  5,  6,  6,  7,  4,
		8,  9, 10, 10, 11,  8,
		12, 14, 13, 14, 12, 15,
		16, 17, 18, 18, 19, 16,
		20, 22, 21, 22, 20, 23,
	};

	m_vertexLayout = VertexLayout::Create();

	m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, 
				GL_STATIC_DRAW, vertices, sizeof(float) * 120);
	
	m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0); // for location
	// m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3); // for color
	m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3); // for texture coord 2D.

	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 36);
	
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
	m_program->SetUniform("tex1", 0);
	m_program->SetUniform("tex2", 1);
	
	return true;
}

void Context::Render()
{
	std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
    auto projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 20.0f);
    auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

    for (size_t i = 0; i < cubePositions.size(); i++){
        auto& pos = cubePositions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model,
            glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i),
            glm::vec3(1.0f, 0.5f, 0.0f));
        auto transform = projection * view * model;
        m_program->SetUniform("transform", transform);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
	m_program->Use();
}