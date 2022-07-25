#pragma once
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
	void OnGui(SDL_Window*) override;

private:
	bool m_ShouldRotate{false};
	float m_RotationSpeed{ glm::radians(45.f)};

	bool m_ShouldMove{ false };
	float m_Frequency{ 1.f };
	float m_Distance{ 100.f };

	float m_ParentPos[2];
};

