#include "IdraPCH.h"

#include "Resources/Model/Mesh.h"

namespace Idra {
	Mesh::Mesh(const std::vector<float>& vBO, const BufferLayout& bLO, const std::vector<uint32_t>& iBO)
	{
		m_VertexArray = VertexArray::Create();

		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vBO);
		vertexBuffer->SetLayout(bLO);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		Ref<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::Create(iBO);
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