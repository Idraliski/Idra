#pragma once

#include "Core/Window.h"
#include "Renderer/RenderingContext.h"

struct GLFWwindow;

namespace Idra {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		const std::string& GetVendor() const override { return m_RenderingContext->GetVendor(); }
		const std::string& GetRenderer() const override { return m_RenderingContext->GetRenderer(); }
		const std::string& GetVersion() const override { return m_RenderingContext->GetVersion(); }

		inline virtual void* GetNativeWindow() const override { return m_Window; }

		virtual float GetTime() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		std::unique_ptr<RenderingContext> m_RenderingContext;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
} // namespace Idra