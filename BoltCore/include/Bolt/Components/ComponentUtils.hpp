#pragma once
#include "Bolt/Scene/EntityHandle.hpp"

#include <string>
#include <stdexcept>

namespace Bolt {
	class ComponentUtils {
	public:
		template<typename TComponent, typename... Args>
			requires (!std::is_empty_v<TComponent>)
		static TComponent& AddComponent(entt::registry& registry, EntityHandle entity, Args&&... args) {
			if (registry.all_of<TComponent>(entity)) {
				throw std::runtime_error("Component of type \"" + std::string(typeid(TComponent).name()) + "\" already exists on entity.");
			}

			if constexpr (std::is_constructible_v<TComponent, EntityHandle, Args...>) {
				return registry.emplace<TComponent>(
					entity,
					entity,
					std::forward<Args>(args)...
				);
			}
			else if constexpr (sizeof...(Args) == 0 && std::is_default_constructible_v<TComponent>) {
				return registry.emplace<TComponent>(entity);
			}
			else {
				return registry.emplace<TComponent>(entity, std::forward<Args>(args)...);
			}
		}

		template<typename TTag>
			requires std::is_empty_v<TTag>
		static void AddComponent(entt::registry& registry, EntityHandle entity) {
			if (registry.all_of<TTag>(entity)) {
				throw std::runtime_error("Component of type \"" + std::string(typeid(TTag).name()) + "\" already exists on entity.");
			}
			registry.emplace<TTag>(entity);
		}


		template<typename TComponent>
		static bool HasComponent(const entt::registry& registry, EntityHandle entity) {
			return registry.all_of<TComponent>(entity);
		}


		template<typename... TComponent>
		static bool HasAnyComponent(const entt::registry& registry, EntityHandle entity) {
			return registry.any_of<TComponent...>(entity);
		}

		template<typename... TComponent, typename... TEntity>
		static bool AnyHasComponent(const entt::registry& registry, TEntity... entities) {
			return (... || registry.any_of<TComponent...>(entities));
		}


		template<typename TComponent>
		static TComponent& GetComponent(entt::registry& registry, EntityHandle entity) {
			if (!registry.all_of<TComponent>(entity)) {
				throw std::runtime_error("Component of type \"" + std::string(typeid(TComponent).name()) + "\" not found on entity.");
			}
			return registry.get<TComponent>(entity);
		}


		template<typename TComponent>
		static const TComponent& GetComponent(const entt::registry& registry, EntityHandle entity) {
			if (!registry.all_of<TComponent>(entity)) {
				throw std::runtime_error("Component of type \"" + std::string(typeid(TComponent).name()) + "\" not found on entity.");
			}
			return registry.get<TComponent>(entity);
		}


		template<typename TComponent>
		static TComponent* TryGetComponent(entt::registry& registry, EntityHandle entity) {
			if (!registry.all_of<TComponent>(entity)) {
				return nullptr;
			}
			return &registry.get<TComponent>(entity);
		}


		template<typename TComponent>
		static void RemoveComponent(entt::registry& registry, EntityHandle entity) {
			if (!registry.all_of<TComponent>(entity)) {
				throw std::runtime_error("Component of type \"" + std::string(typeid(TComponent).name()) + "\" not found on entity.");
			}
			registry.remove<TComponent>(entity);
		}
	};
}