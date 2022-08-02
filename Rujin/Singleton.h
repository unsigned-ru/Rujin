#ifndef SINGLETON_H
#define SINGLETON_H

namespace rujin
{
	class Rujin;
	class IGame;

	template <typename T>
	class Singleton
	{
	public:
		template<typename... Args>
		static T* Create(Args&&... args)
		{
			if (!m_pInstance)
			{
				m_pInstance = new T(std::forward<Args>(args)...);
				m_pInstance->Initialize();
			}

			return m_pInstance;
		}

		static T* Get()
		{
			return m_pInstance;
		}

		static void Destroy()
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}

		Singleton(const Singleton& other) noexcept = delete;
		Singleton(Singleton&& other) noexcept = delete;
		Singleton& operator=(const Singleton& other) = delete;
		Singleton& operator=(Singleton&& other) noexcept = delete;

	protected:
		Singleton() = default;
		virtual ~Singleton() = default;

	private:
		inline static T* m_pInstance = nullptr;
		virtual void Initialize() = 0;
	};
}


#endif // Include Guard: SINGLETON_H
