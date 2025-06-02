#include "IdraPCH.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Idra {

	void OpenGLRendererAPI::Init()
	{
		IDRA_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetDepthFunction(DepthFunction depthFunc)
	{
		switch (depthFunc)
		{
		case DepthFunction::Less:
			glDepthFunc(GL_LESS);
			break;
		case DepthFunction::LEqual:
			glDepthFunc(GL_LEQUAL);
			break;
		}
	}

	void OpenGLRendererAPI::SetCullFace(CullFace cullFace)
	{
		switch (cullFace)
		{
		case CullFace::Front:
			glCullFace(GL_FRONT);
			break;
		case CullFace::Back:
			glCullFace(GL_BACK);
			break;
		case CullFace::FrontAndBack:
			glCullFace(GL_FRONT_AND_BACK);
			break;
		}
	}
}