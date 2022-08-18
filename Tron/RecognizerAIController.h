#ifndef RECOGNIZER_AI_CONTROLLER_H
#define RECOGNIZER_AI_CONTROLLER_H
#include "Component.h"
#include "EnemyAIController.h"

class RecognizerComponent;

class RecognizerAIController final : public EnemyAIController
{
public:
	explicit RecognizerAIController(RecognizerComponent* pRecognizerComponent);

	void Start() override;
	void FixedUpdate() override;

	enum class State
	{
		MoveToClosestPlayer,
		Attack
	};

private:
	void MoveToClosestPlayer();

	void ExecuteCurrentState();
	void HandleStateTransitions();

	State m_State{ State::MoveToClosestPlayer };
	std::vector<rujin::GameObject*> m_Players{};

	RecognizerComponent* m_pRecognizer = nullptr;

	const float m_MaxAttackDistance{ 105.f };
};

#endif // Include Guard: RECOGNIZER_AI_CONTROLLER_H
