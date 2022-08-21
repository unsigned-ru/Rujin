#ifndef RECOGNIZER_AI_CONTROLLER_H
#define RECOGNIZER_AI_CONTROLLER_H
#include "Component.h"
#include "EnemyAIController.h"

class RecognizerComponent;

class RecognizerAIController final : public EnemyAIController, public event::IObserver
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

	void OnNotify(const uint32_t identifier, const event::Data* pEventData) override;

	State m_State{ State::MoveToClosestPlayer };
	std::vector<rujin::GameObject*> m_Players{};

	RecognizerComponent* m_pRecognizer = nullptr;

	const float m_MaxAttackDistance{ 60.f };

	inline static constexpr uint32_t s_Score{ 250u };
};

#endif // Include Guard: RECOGNIZER_AI_CONTROLLER_H
