#include "Idra.h"
#include "Core/EntryPoint.h"

#include "EditorLayer.h"

namespace Idra {
	class IdraEditor : public Application
	{
	public:
		IdraEditor()
			: Application(WindowProps{ "Idra Editor", 1920, 1080 })
		{
			IDRA_WARN("Idra Editor Created"); // #DEBUG

			PushLayer(new EditorLayer());
		}

		~IdraEditor()
		{
			IDRA_WARN("Idra Editor Destroyed"); // #DEBUG
		}
	};

	Application* CreateApplication()
	{
		return new IdraEditor();
	}

}