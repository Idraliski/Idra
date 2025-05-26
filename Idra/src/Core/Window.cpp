#include "IdraPCH.h"

#include "Core/Window.h"

#include "Platform/Windows/WindowsWindow.h"

namespace Idra {
	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
	}
}