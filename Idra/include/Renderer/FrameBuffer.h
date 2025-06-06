#pragma once

#include "Core/Core.h"

namespace Idra {
	struct IDRA_API FrameBufferSpecification
	{
		uint32_t Width = 1280;
		uint32_t Height = 720;

		bool SwapChainTarget = false; // If true, this framebuffer is used as a swapchain target (e.g. for the main window)
	};

	class IDRA_API FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Invalidate() = 0;
		virtual void Resize(const FrameBufferSpecification& spec) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

	};
}