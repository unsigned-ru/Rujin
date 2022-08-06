#ifndef TRON_TEST_COMPONENT_H
#define TRON_TEST_COMPONENT_H


#include <Component.h>
#include "InputStructs.h"

class TronTestComponent final : public rujin::Component
{
public:
	explicit TronTestComponent(PlayerIndex playerIndex);
	~TronTestComponent() override = default;

	TronTestComponent(const TronTestComponent&) = delete;
	TronTestComponent(TronTestComponent&&) noexcept = delete;
	TronTestComponent& operator=(const TronTestComponent&) = delete;
	TronTestComponent& operator=(TronTestComponent&&) noexcept = delete;

	void Start() override;
	void Update() override;
	void Draw() const override;
	void OnGui(SDL_Window*) override;

private:
	PlayerIndex m_PlayerIndex;
	bool m_RaycastHit = false;
	glm::vec2 m_RayStart{0.f, 0.f};
	glm::vec2 m_RayEnd{0.f, 0.f};
};

#endif // Include Guard: TRON_TEST_COMPONENT_H
