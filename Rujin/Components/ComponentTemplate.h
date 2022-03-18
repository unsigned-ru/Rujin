#pragma once
#include "Components/Component.h"

namespace rujin
{
	class ComponentTemplate final : public Component
	{
	public:
		explicit ComponentTemplate() = default;

		void Start() override;
		void Update() override;

	private:
	};
}

