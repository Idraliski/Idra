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
		void OnImGuiRender(Timestep ts) override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};

} // namespace Idra