#include "./Scene.hpp"
#include "./Entity.hpp"
#include "./Components.hpp"
#include "./RenderSystem.hpp"
#include <raylib.h>

namespace Feather
{
	Entity Scene::CreateEntity()
	{
		m_IsDirty = true;
		return Entity(m_Registry.create(), this);
	}

	void Scene::DestroyEntity(const Entity& e)
	{
		m_IsDirty = true;
		assert(m_Registry.valid(e.GetHandle()) && "Entity handle is invalid");
		m_Registry.destroy(e.GetHandle());
	}

	void Scene::DestroyAll()
	{
		m_IsDirty = true;
		m_Registry = entt::registry{};
	}

	void Scene::OnRuntimeStart()
	{
		m_IsRunning = true;
	}
	void Scene::OnRender()
	{
		BeginDrawing();
		ClearBackground(BLACK);
		m_Registry.view<const SpriteRendererComponent, const TransformComponent>().each(
			[](const SpriteRendererComponent& renderer, const TransformComponent& transform)
			{
				RenderSystem::RenderSprite(renderer, transform);
			}
		);

		m_Registry.view<const ViewportRendererComponent, const TransformComponent>().each(
			[](const ViewportRendererComponent& renderer, const TransformComponent& transform)
			{
				RenderSystem::RenderViewport(renderer, transform);
			}
		);
		EndDrawing();
	}

	void Scene::OnTick(float deltaTimeSeconds)
	{
		if (!m_IsRunning) return;

		// script runtime update
		auto scriptView = m_Registry.view<ScriptComponent>().each();

		for (auto [entity, scriptComponent] : scriptView)
		{
			auto sceneEnt = Entity(entity, this);
			scriptComponent.m_OnUpdate(*this, sceneEnt, deltaTimeSeconds);
			if (m_IsDirty) break;
		}

		m_IsDirty = false;
	}

	void Scene::OnRuntimeStop()
	{
		m_IsRunning = false;
	}

	std::vector<Entity> Scene::GetEntitiesWithTagName(std::string tag)
	{
		auto view = m_Registry.view<const TagComponent>();
		std::vector<Entity> out{};

		for (auto [entity, tagComp] : view.each())
		{
			if (tagComp.m_Tag == tag)
				out.push_back(Entity(entity, this));
		}

		return out;
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity& entity, T& component)
	{
		static_assert(sizeof(T) != 0);
	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity& entity, ScriptComponent& component)
	{
		component.m_OnCreate(*this, entity);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity& entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity& entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity& entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<ModelRendererComponent>(Entity& entity, ModelRendererComponent& component)
	{
	}

}