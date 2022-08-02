#ifndef SERVICE_H
#define SERVICE_H


namespace rujin
{
	class Service
	{
	public:
		Service() = default;
		virtual ~Service() = default;

		Service(const Service&) = delete;
		Service(Service&&) noexcept = delete;
		Service& operator=(const Service&) = delete;
		Service& operator=(Service&&) noexcept = delete;
	};
}


#endif // Include Guard: SERVICE_H
