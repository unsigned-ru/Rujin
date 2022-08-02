#ifndef COLLISION_TEST_COMPONENT_H
#define COLLISION_TEST_COMPONENT_H
#include "Component.h"


class CollisionTestComponent : public rujin::Component
{
public:
	explicit CollisionTestComponent() = default;
	~CollisionTestComponent() override = default;

	void Draw() const override;

	void OnOverlap(const CollisionResult&) override;
};


#endif // Include Guard: COLLISION_TEST_COMPONENT_H
