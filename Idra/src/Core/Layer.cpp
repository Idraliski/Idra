#include "Core/Layer.h"

namespace Idra {
	Layer::Layer(const std::string& name)
		: m_DebugName(name)
	{
		IDRA_CORE_INFO("Layer Created: {0}", m_DebugName); // #DEBUG
	}

	Layer::~Layer()
	{
		IDRA_CORE_INFO("Layer Destroyed: {0}", m_DebugName); // #DEBUG
	}


} // namespace Idra