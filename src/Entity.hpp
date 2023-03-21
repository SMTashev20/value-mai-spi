#pragma once
#include <Scene.hpp>

#include <cassert>

namespace Feather
{

	// an entity class for interfacing
	// with entt's internals
	class Entity
	{
	public:
		Entity(entt::entity handle, Scene* scene);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			assert(!HasComponent<T>() && "Entity already has said component");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, args...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, args...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		void RemoveComponent()
		{
			assert(!HasComponent<T>() && "Entity doesn't have said component");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			assert(HasComponent<T>() && "Entity doesn't have said component");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			// quite a costly approach
			// TODO: implement a different way
			if (m_Scene->m_Registry.try_get<T>(m_EntityHandle))
				return true;
			else return false;
		}

		entt::entity GetHandle() const { return m_EntityHandle; }

	private:
		entt::entity m_EntityHandle;
		Scene* m_Scene;
	};

}