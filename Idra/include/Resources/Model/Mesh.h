#pragma once

#include "Core/Core.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Buffer.h"
#include "Renderer/Texture.h"

namespace Idra {
	class IDRA_API Mesh
	{
	public:
		Mesh(const std::vector<float>& vBO, const BufferLayout& bLO, const std::vector<uint32_t>& iBO);
		~Mesh() = default;

		void Bind() const;
		void Unbind() const;

		inline const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<Texture> m_Texture;
	};
}