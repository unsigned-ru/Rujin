#ifndef IOBSERVER_H
#define IOBSERVER_H

namespace rujin::event
{
	struct Data;

	class IObserver
	{
	public:
		IObserver() = default;
		virtual ~IObserver() = default;

		IObserver(IObserver&) = delete;
		IObserver(IObserver&&) = delete;
		IObserver& operator=(IObserver&) = delete;
		IObserver& operator=(IObserver&&) = delete;

	protected:
		friend class Subject;
		virtual void OnNotify(const uint32_t identifier, const event::Data* pEventData) = 0;

	private:
		bool m_PendingRemove{ false };
	};
}


#endif // Include Guard: IOBSERVER_H




