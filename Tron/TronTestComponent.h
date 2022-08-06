#ifndef TRON_TEST_COMPONENT_H
#define TRON_TEST_COMPONENT_H


#include <Component.h>

class TronTestComponent final : public rujin::Component
{
public:
	explicit TronTestComponent() = default;
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
	bool m_RaycastHit = false;
};

#endif // Include Guard: TRON_TEST_COMPONENT_H
