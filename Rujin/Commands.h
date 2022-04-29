#pragma once
#include "Component.h"

namespace rujin
{
	class BurgerComponent;
	class EnemyComponent;
	class Component;
	class PeterPepperComponent;

	namespace command
	{
		class IBase
		{
		public:
			explicit IBase(Component* pComp) : m_pComponent(pComp) {}
			virtual ~IBase() = default;
			virtual void Execute() = 0;

		protected:
			template<typename ComponentType>
			ComponentType* GetComponent() const { return static_cast<ComponentType*>(m_pComponent); }

		private:
			Component* m_pComponent;
		};

		class Die final : public IBase
		{
		public:
			explicit Die(PeterPepperComponent* pComp);
			void Execute() override;
		};

		class DropBurger final : public IBase
		{
		public:
			explicit DropBurger(PeterPepperComponent* pComp, BurgerComponent* pBurger);
			void Execute() override;

		private:
			BurgerComponent* m_pBurger;
		};

		class KillEnemy final : public IBase
		{
		public:
			explicit KillEnemy(PeterPepperComponent* pComp, EnemyComponent* pEnemy);
			void Execute() override;

		private:
			EnemyComponent* m_pEnemy;
		};
	}
}
