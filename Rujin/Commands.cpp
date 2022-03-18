#include "RujinPCH.h"
#include "Commands.h"

#include "Components/PeterPepperComponent.h"

rujin::command::Die::Die(PeterPepperComponent* pComp) : Base(pComp) {};
void rujin::command::Die::Execute()
{
	GetComponent<PeterPepperComponent>()->Die();
}


rujin::command::GainPoints::GainPoints(PeterPepperComponent* pComp) : Base(pComp) {};
void rujin::command::GainPoints::Execute()
{
	GetComponent<PeterPepperComponent>()->AddPoints(rand() % 21);
}


