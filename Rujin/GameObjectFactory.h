#pragma once
#include "GameObject.h"

namespace rujin
{
	/**
	 * \brief Abstract class for creation of GameObjects
	 */
	class GameObjectFactory
	{
	public:
		static std::shared_ptr<GameObject> CreateEmpty(const std::string& name);
		static std::shared_ptr<GameObject> CreateDefault(const std::string& name);
		static std::shared_ptr<GameObject> CreateCanvas(const std::string& name);
	};

}
