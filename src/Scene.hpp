#pragma once

#include <entt/entt.hpp>

namespace Feather
{
	class Entity;

	class Scene
	{
	private:
		friend class Entity;

	public:
		Entity CreateEntity();
		void DestroyEntity(const Entity&);

		bool IsRunning() { return m_IsRunning; }

		void OnRuntimeStart();

		void OnRender();
		void OnTick(float deltaTimeSeconds);

		void OnRuntimeStop();

		template<typename... Other, typename... Exclude>
		auto GetEntitiesWith(entt::exclude_t<Exclude...> = {})
		{
			auto view = m_Registry.view<Other...>(entt::exclude<Exclude...>);
			std::vector<Entity> out{};

			for (auto& entity : view)
				out.push_back(Entity(entity, this));

			return out;
		}
	
	private:
		// not really used, but could come in handy
		// when internals need to be notified when a certain
		// component has been added to the scene
		template<typename T>
		void OnComponentAdded(Entity& entity, T& component);

	private:
		bool m_IsRunning = true;
		entt::registry m_Registry;
	};

}