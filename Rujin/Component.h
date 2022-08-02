#ifndef COMPONENT_H
#define COMPONENT_H
#include "IGameLoopObject.h"

namespace rujin
{
	struct CollisionResult;
	class GameObject;

	class Component : public IGameLoopObject
	{
	public:
		explicit Component();
		~Component() override = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		// Get the GameObject this component is attached to.
		GameObject* GameObject() const;

		// does not own GameObject.
		rujin::GameObject* m_pGameObject;

	private:
		friend class GameObject;
		virtual void OnOverlap(const CollisionResult& /*collision*/) {};
	};

}

#endif