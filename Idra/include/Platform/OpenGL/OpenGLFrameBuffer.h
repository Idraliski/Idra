#pragma once

#include "Renderer/FrameBuffer.h"

namespace Idra {
	class IDRA_API OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		~OpenGLFrameBuffer();

		void Bind() const override;
		void Unbind() const override;

		void Invalidate() override;
		void Resize(const FrameBufferSpecification& spec) override;

		uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
		FrameBufferSpecification m_Specification;
	};
}