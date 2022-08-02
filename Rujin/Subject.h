#ifndef SUBJECT_H
#define SUBJECT_H

#include "IObserver.h"
#include <vector>

namespace rujin::event
{
	class Subject
	{
	public:
		explicit Subject(std::vector<IObserver*> pObservers)
		{
			for (auto* pObserver : pObservers)
				AddObserver(pObserver);
		}
		virtual ~Subject() = 0;

		Subject(const Subject&) = delete;
		Subject(Subject&&) noexcept = delete;
		Subject& operator=(const Subject&) = delete;
		Subject& operator=(Subject&&) noexcept = delete;


		void AddObserver(IObserver* pObserver)
		{
			const auto it = std::ranges::find(m_Observers, pObserver);

			// don't register if already registered
			if (it != m_Observers.end()) return;

			m_Observers.push_back(pObserver);
		}
		void RemoveObserver(IObserver* pObserver)
		{
			const auto it = std::ranges::find(m_Observers, pObserver);

			// if we don't have this observer, we cannot remove it. return.
			if (it == m_Observers.end()) return;

			
			if (m_PendRemoval) //removals should be pended.
			{
				//set to nullptr to avoid further usage.
				(*it)->m_PendingRemove = true;
				m_ObserversToRemove.push_back(it);
			}
			else 
				m_Observers.erase(it); //removals don't have to be pended, can instantly remove.
		}

	protected:
		void Notify(const uint32_t identifier, const Data* pData)
		{
			//ensures any removals are going to happen AFTER we are done iterating
			m_PendRemoval = true;

			for (auto* pObserver : m_Observers)
				if (!pObserver->m_PendingRemove)
					pObserver->OnNotify(identifier, pData);

			//erase the observers pending removal
			for (const auto it : m_ObserversToRemove)
				m_Observers.erase(it);
			m_ObserversToRemove.clear();
			
			m_PendRemoval = false;
		}

	private:
		std::vector<IObserver*> m_Observers{};

		//used to delay
		bool m_PendRemoval{ false };
		std::vector<std::ranges::borrowed_iterator_t<std::vector<IObserver*>&>> m_ObserversToRemove{};
	};

	inline Subject::~Subject() = default;
}


#endif // Include Guard: SUBJECT_H
