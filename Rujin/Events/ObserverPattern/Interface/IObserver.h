#pragma once

namespace rujin::event
{
	struct Data;
	enum class Identifier;

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
		friend class ISubject;
		virtual void OnNotify(const event::Identifier identifier, const event::Data* pEventData) = 0;
	};
}


