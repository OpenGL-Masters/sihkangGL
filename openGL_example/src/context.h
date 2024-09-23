#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "program.h"
#include "shader.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"
#include "mesh.h"
#include "model.h"

CLASS_PTR(Context)
class Context
{
public:
	static ContextUPtr Create();
	void Render();
	void ProcessInput(GLFWwindow* window);
	void Reshape(int width, int height);
	void MouseMove(double x, double y);
	void MouseButton(int button, int action, double x, double y);

private:
	Context() {}
	bool Init();
	ProgramUPtr m_program;
	ProgramUPtr m_simpleProgram;

	MeshUPtr m_box;
	ModelUPtr m_model;
	TextureUPtr m_texture1;
	TextureUPtr m_texture2;

	bool m_animation { true } ;

	glm::vec4 m_clearColor { glm::vec4(0.1f, 0.2f, 0.3f, 0.0f) };

	struct Light
	{
		glm::vec3 position { glm::vec3(2.0f, 2.0f, 2.0f) };
		glm::vec3 direction { glm::vec3(-1.0f, -1.0f, -1.0f) };
		glm::vec2 cutoff { glm::vec2(20.0f, 5.0f) };
		float distance { 32.0f };
		glm::vec3 ambient { glm::vec3(0.1f, 0.1f, 0.1f) };
		glm::vec3 diffuse { glm::vec3(0.5f ,0.5f, 0.5f ) };
		glm::vec3 specular { glm::vec3(1.0f, 1.0f, 1.0f) };
	}; Light m_light;
	bool m_flashLightMode { false };

	struct Material
	{
		TextureUPtr diffuse;
		TextureUPtr specular;
		// glm::vec3 specular { glm::vec3(0.5f, 0.5f, 0.5f) };
		float shininess { 32.0f };
	}; Material m_material;

	float m_cameraPitch { 0.0f }; // 60분법 각도
	float m_cameraYaw { 0.0f } ;
	glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
	glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) }; // 카메라가 어떤 방향으로 바라보고 있다를 나타내기 위함.
	glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };

	int m_width { WINDOW_WIDTH };
	int m_height { WINDOW_HEIGHT };

	bool m_cameraControl { false };
	glm::vec2 m_prevMousePos { glm::vec2(0.0f) };
};

#endif