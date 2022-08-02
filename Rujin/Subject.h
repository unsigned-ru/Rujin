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

			// don't register if already registered
			if (it == m_Observers.end()) return;

			m_Observers.erase(it);
		}

	protected:
		void Notify(const uint32_t identifier, const Data* pData) const
		{
			for (auto* pObserver : m_Observers)
				pObserver->OnNotify(identifier, pData);
		}

	private:

		std::vector<IObserver*> m_Observers{};
	};

	inline Subject::~Subject() = default;
}


#endif // Include Guard: SUBJECT_H
