#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Idra {
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Idra Engine",
				uint32_t width = 1920,
				uint32_t height = 1080)
			: Title(title), Width(width), Height(height) 
		{

		}
	};

	enum class IDRA_API CursorMode
	{
		Normal = 0,
		Hidden = 1,
		Disabled = 2
	};

	// Interface representing a desktop system based Window
	class IDRA_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		
		// Cursor 
		virtual void SetCursorMode(CursorMode mode) = 0;
		virtual CursorMode GetCursorMode() const = 0;
		virtual void SetCursorPosition(float x, float y) = 0;

		virtual const std::string& GetVendor() const = 0;
		virtual const std::string& GetRenderer() const = 0;
		virtual const std::string& GetVersion() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual float GetTime() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
} // namespace Idra