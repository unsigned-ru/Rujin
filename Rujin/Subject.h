#ifndef SUBJECT_H
#define SUBJECT_H

#include <stack>

#include "IObserver.h"
#include <vector>

#include "DefferedRemovalVector.h"

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

		virtual ~Subject() = default;

		Subject(const Subject&) = delete;
		Subject(Subject&&) noexcept = delete;
		Subject& operator=(const Subject&) = delete;
		Subject& operator=(Subject&&) noexcept = delete;

		virtual void ProcessAdditionsAndRemovals()
		{
			m_Observers.ProcessAdditions();
			m_Observers.ProcessRemovals();
		}

		void AddObserver(IObserver* pObserver)
		{
			//check if said observer is in the to-remove list
			if (const auto it = std::ranges::find(m_Observers.GetElementsToRemove(), pObserver); it != m_Observers.GetElementsToRemove().end())
			{
				//if the observer we want to add is in the to-be removed list, then just remove it from the to-be-removed list
				m_Observers.GetElementsToRemove().erase(it);
				return;
			}

			//check if we already have the observer
			if (auto it = std::ranges::find(m_Observers.GetVector(), pObserver); it != m_Observers.GetVector().end())
				return; //if we do, don't add it

			//add observer.
			m_Observers.GetVector().push_back(pObserver);
		}

		void RemoveObserver(IObserver* pObserver)
		{
			//check if said observer is in the to-be-added list
			if (const auto it = std::ranges::find(m_Observers.GetElementsToAdd(), pObserver); it != m_Observers.GetElementsToAdd().end())
			{
				//if the observer we want to remove is in the to-be added list, then just remove it from the to-be-added list
				m_Observers.GetElementsToAdd().erase(it);
				return;
			}

			//remove observer.
			auto it = std::ranges::find(m_Observers.GetVector(), pObserver);
 			if (it != m_Observers.GetVector().end()) m_Observers.GetVector().erase(it);
		}

		void RemoveObserverInstant(IObserver* pObserver)
		{
			//check if said observer is in the to-be-added list
			if (const auto it = std::ranges::find(m_Observers.GetElementsToAdd(), pObserver); it != m_Observers.GetElementsToAdd().end())
			{
				//if the observer we want to remove is in the to-be added list, then just remove it from the to-be-added list
				m_Observers.GetElementsToAdd().erase(it);
				return;
			}

			//remove observer.
			m_Observers.GetVector().erase(std::ranges::find(m_Observers.GetVector(), pObserver));
		}

	protected:
		void Notify(const uint32_t identifier, const Data* pData)
		{
			for (auto* pObserver : m_Observers.GetVector())
				pObserver->OnNotify(identifier, pData);
		}

	private:
		DeferredVector<IObserver*, IObserver*> m_Observers{};
	};
}


#endif // Include Guard: SUBJECT_H