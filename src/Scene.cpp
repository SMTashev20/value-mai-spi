#include "./Scene.hpp"
#include "./Entity.hpp"
#include "./Components.hpp"
#include "./RenderSystem.hpp"
#include <raylib.h>

namespace Feather
{
	Entity Scene::CreateEntity()
	{
		return Entity(m_Registry.create(), this);
	}

	void Scene::DestroyEntity(const Entity& e)
	{
		assert(m_Registry.valid(e.GetHandle()) && "Entity handle is invalid");
		m_Registry.destroy(e.GetHandle());
	}

	void Scene::OnRuntimeStart()
	{
		m_IsRunning = true;
	}
	void Scene::OnRuntimeUpdate(float deltaTimeSeconds)
	{
		if (!m_IsRunning) return;

		// TODO: implement renderers

		BeginDrawing();
		m_Registry.view<const SpriteRendererComponent, const TransformComponent>().each(
			[](const SpriteRendererComponent& renderer, const TransformComponent& transform)
			{
				RenderSystem::RenderSprite(renderer, transform);
			}
		);
		EndDrawing();
	}

	void Scene::OnRuntimeStop()
	{
		m_IsRunning = false;
	}
}