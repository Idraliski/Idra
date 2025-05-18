#include "IdraPCH.h"

#include "Renderer/Renderer.h"
#include "Renderer/Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Idra {

	////////////////////////////////////////////////////////////////////////
	// BufferElement ///////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////

	BufferElement::BufferElement(ShaderDataType type, const std::string& name, bool normalized)
		: Type(type), Name(name), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
	{

	}

	uint32_t BufferElement::GetComponentCount() const
	{
		switch (Type)
		{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Mat3:    return 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Bool:    return 1;
		}

		IDRA_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}

	////////////////////////////////////////////////////////////////////////
	// BufferLayout ////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	
	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
		: m_Elements(elements)
	{
		CalculateOffsetsAndStride();
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		size_t offset = 0;
		m_Stride = 0;

		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

	BufferLayout BufferLayout::GetVertexFormat(VertexFormat format)
	{
		switch (format)
		{
			case VertexFormat::Pos: 
				return { 
					{ ShaderDataType::Float3, "a_Position" } 
				};
			case VertexFormat::PosNor: 
				return { 
					{ ShaderDataType::Float3, "a_Position" }, 
					{ ShaderDataType::Float3, "a_Normal" } 
				};
			case VertexFormat::PosNorTex: 
				return { 
					{ ShaderDataType::Float3, "a_Position" }, 
					{ ShaderDataType::Float3, "a_Normal" }, 
					{ ShaderDataType::Float2, "a_TexCoord" } 
				};
			case VertexFormat::PosTex: 
				return { 
					{ ShaderDataType::Float3, "a_Position" }, 
					{ ShaderDataType::Float2, "a_TexCoord" } 
				};
		}

		IDRA_CORE_ASSERT(false, "Unknown vertex format!");
		return {};
	}
	////////////////////////////////////////////////////////////////////////
	// VertexBuffer ////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////

	Ref<VertexBuffer> VertexBuffer::Create(const std::vector<float>& vertices)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: 
				IDRA_CORE_ASSERT(false, "RendererAPI::None is not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(vertices);
			case RendererAPI::API::DirectX:
				IDRA_CORE_ASSERT(false, "RendererAPI::DirectX is not supported!");
				return nullptr;
			case RendererAPI::API::Vulkan:
				IDRA_CORE_ASSERT(false, "RendererAPI::Vulkan is not supported!");
				return nullptr;
		}

		IDRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	////////////////////////////////////////////////////////////////////////
	// IndexBuffer /////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////

	Ref<IndexBuffer> IndexBuffer::Create(const std::vector<uint32_t>& indices)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				IDRA_CORE_ASSERT(false, "RendererAPI::None is not supported!"); 
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLIndexBuffer>(indices);
			case RendererAPI::API::DirectX:
				IDRA_CORE_ASSERT(false, "RendererAPI::DirectX is not supported!");
				return nullptr;
			case RendererAPI::API::Vulkan:
				IDRA_CORE_ASSERT(false, "RendererAPI::Vulkan is not supported!");
				return nullptr;
		}

		IDRA_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}