#ifndef ENGINE_EVENTS_H
#define ENGINE_EVENTS_H

#include <cstdint>

namespace rujin
{
	class Transform;
}

namespace rujin::event
{
	enum class Identifier : uint32_t
	{
		OnTransformChanged,
		OnGameObjectDestroyed,

		//always last
		LAST_ENGINE_EVENT
	};

	struct Data {};

	struct OnTransformChanged_t final : Data
	{
		explicit OnTransformChanged_t(Transform& transform, const TransformChanged& transformChanged)
			: transform(transform)
			, transformChanged(transformChanged)
		{}

		Transform& transform;

		const TransformChanged& transformChanged;
	};

	struct OnGameObjectDestroyed_t final : Data
	{
		explicit OnGameObjectDestroyed_t(GameObject* pObj)
			: pObject(pObj)
		{}

		GameObject* pObject;
	};

}

#endif