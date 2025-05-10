#pragma once

#include "Core/Core.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Buffer.h"

namespace Idra {
	class IDRA_API Mesh
	{
	public:
		Mesh(const std::vector<float>& vBO, const BufferLayout& bLO, const std::vector<uint32_t>& iBO);
		Mesh(float* vBO, uint32_t vBOsize, const BufferLayout& bLO,  uint32_t* iBO, uint32_t iBOsize);
		~Mesh() = default;

		void Bind() const;
		void Unbind() const;

		inline const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }

	private:
		std::shared_ptr<VertexArray> m_VertexArray;
	};
}