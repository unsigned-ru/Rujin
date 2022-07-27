#pragma once
#include "Service.h"

#include <memory>
#include <unordered_map>
#include "SDL_AudioProvider.h"

namespace rujin
{
	//TODO: add null init function
	class ServiceLocator final
	{
	public:
		template
		<
			typename ServiceType,
			typename ProviderType,
			typename = std::enable_if<std::is_base_of_v<Service, ServiceType> && std::is_abstract_v<ServiceType>>,
			typename = std::enable_if<std::is_base_of_v<ProviderType, ServiceType>>
		>
		static ServiceType& RegisterService()
		{
			auto rv = s_Services.insert_or_assign(typeid(ServiceType).hash_code(), std::make_unique<ProviderType>());
			return *static_cast<ServiceType*>(rv.first->second.get());
		}

		template
		<
			typename ServiceType,
			typename = std::enable_if<std::is_base_of_v<Service, ServiceType> && std::is_abstract_v<ServiceType>>
		>
		static [[nodiscard]] ServiceType& GetService()
		{
			const auto it = s_Services.find(typeid(ServiceType).hash_code());

			if (it != s_Services.end())
				return *static_cast<ServiceType*>(it->second.get());

			LOG_ERROR_("Tried to get unregistered service: {}", typeid(ServiceType).name());
			throw;
		}

		static void ClearServices()
		{
			s_Services.clear();
		}

	private:
		inline static std::unordered_map<size_t, std::unique_ptr<Service>> s_Services{};
	};
}
