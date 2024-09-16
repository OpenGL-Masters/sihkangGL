#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "program.h"
#include "shader.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"

CLASS_PTR(Context)
class Context
{
public:
	static ContextUPtr Create();
	void Render();
private:
	Context() {}
	bool Init();
	ProgramUPtr m_program;

	VertexLayoutUPtr m_vertexLayout;
	BufferUPtr m_vertexBuffer;
	BufferUPtr m_indexBuffer;
	TextureUPtr m_texture1;
	TextureUPtr m_texture2;
};

#endif