#pragma once

#include "Core/Window.h"
#include "Renderer/RenderingContext.h"

struct GLFWwindow;

namespace Idra {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow() override;

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		// Cursor
		void SetCursorMode(CursorMode mode) override;
		CursorMode GetCursorMode() const override { return m_CursorMode; }
		void SetCursorPosition(float x, float y) override;

		const std::string& GetVendor() const override { return m_RenderingContext->GetVendor(); }
		const std::string& GetRenderer() const override { return m_RenderingContext->GetRenderer(); }
		const std::string& GetVersion() const override { return m_RenderingContext->GetVersion(); }

		virtual void* GetNativeWindow() const override { return m_Window; }

		virtual float GetTime() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<RenderingContext> m_RenderingContext;

		CursorMode m_CursorMode;

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