#ifndef EVENT_DATA_H
#define EVENT_DATA_H

namespace rujin
{
	class TransformComponent;
}

namespace rujin::event
{
	enum class Identifier : uint32_t
	{
		OnTransformChanged,

		//always last
		LAST_ENGINE_EVENT
	};

	struct Data {};

	struct OnTransformChanged_t final : Data
	{
		explicit OnTransformChanged_t(TransformComponent* pTransform)
			: pTransformComponent(pTransform)
		{}

		TransformComponent* pTransformComponent;

		bool position = false;
		bool rotation = false;
		bool scale = false;
	};

}

#endif