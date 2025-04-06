#pragma once

#include "IdraPCH.h"
#include "Core/Layer.h"

namespace Idra
{
	class IDRA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() override;

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		void SetDarkThemeColors();

	private:
		float m_Time = 0.0f;
	};

} // namespace Idra