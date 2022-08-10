#ifndef EVENT_DATA_H
#define EVENT_DATA_H

namespace rujin
{
	class Transform;
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
		explicit OnTransformChanged_t(Transform& transform, const TransformChanged& transformChanged)
			: transform(transform)
			, transformChanged(transformChanged)
		{}

		Transform& transform;

		const TransformChanged& transformChanged;
	};

}

#endif