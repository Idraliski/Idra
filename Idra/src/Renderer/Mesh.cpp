#include "IdraPCH.h"

#include "Renderer/Mesh.h"

namespace Idra {
	Mesh::Mesh(const std::vector<float>& vBO, const BufferLayout& bLO, const std::vector<uint32_t>& iBO)
	{
		m_VertexArray.reset(VertexArray::Create());

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vBO));
		vertexBuffer->SetLayout(bLO);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(iBO));
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void Mesh::Bind() const 
	{
		m_VertexArray->Bind();
	}

	void Mesh::Unbind() const 
	{
		m_VertexArray->Unbind();
	}
}