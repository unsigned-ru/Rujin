#ifndef RECOGNIZER_COMPONENT_H
#define RECOGNIZER_COMPONENT_H
#include <chrono>

#include "Component.h"


class HealthComponent;

namespace rujin
{
	class TextureRenderComponent;
	class BoxColliderComponent;
}

class TronMovementComponent;

class RecognizerComponent : public Component
{
public:
	explicit RecognizerComponent
	(
		TronMovementComponent* pMovement,
		TextureRenderComponent* pBodyRenderer,
		BoxColliderComponent* pCollider,
		HealthComponent* pHealth,
		uint32_t attackDamage = 1u,
		uint32_t attackCooldown = 1'000u
	);

	TronMovementComponent* GetMovement() const;
	TextureRenderComponent* GetBodyRenderer() const;
	BoxColliderComponent* GetColliderComponent() const;
	HealthComponent* GetHealthComponent() const;

	bool CanAttack() const;
	void Attack(const class GameObject* pObjectToAttack);

private:
	TronMovementComponent* m_pMovementComp = nullptr;
	TextureRenderComponent* m_pBodyRenderer = nullptr;
	BoxColliderComponent* m_pBoxCollider = nullptr;
	HealthComponent* m_pHealth = nullptr;

	const uint32_t m_AttackDamage;
	const uint32_t m_AttackingCooldown;
	using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
	TimePoint m_LastAttackTime = std::chrono::high_resolution_clock::now();
};


#endif // Include Guard: RECOGNIZER_COMPONENT_H
