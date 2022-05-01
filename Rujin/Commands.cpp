#include "RujinPCH.h"
#include "Commands.h"

#include "BurgerComponent.h"
#include "PeterPepperComponent.h"
#include "EnemyComponent.h"
#include "ServiceLocator.h"

rujin::command::Die::Die(PeterPepperComponent* pComp) : IBase(pComp) {};
void rujin::command::Die::Execute()
{
	GetComponent<PeterPepperComponent>()->Die();
}

rujin::command::DropBurger::DropBurger(PeterPepperComponent* pComp, BurgerComponent* pBurger)
	: IBase(pComp)
	, m_pBurger(pBurger)
{}
void rujin::command::DropBurger::Execute()
{
	m_pBurger->Drop(GetComponent<PeterPepperComponent>());
}

rujin::command::KillEnemy::KillEnemy(PeterPepperComponent* pComp, EnemyComponent* pEnemy)
	: IBase(pComp)
	, m_pEnemy(pEnemy)
{
}
void rujin::command::KillEnemy::Execute()
{
	m_pEnemy->Die(GetComponent<PeterPepperComponent>());
}

rujin::command::PlaySound::PlaySound(const std::string& soundPath)
	: IBase(nullptr)
	, m_SoundPath(soundPath)

{}

void rujin::command::PlaySound::Execute()
{
	ServiceLocator::GetService<AudioService>().PlaySound(m_SoundPath, 100);
}




