#pragma once
#include "./Components.hpp"

namespace Feather
{
	namespace RenderSystem
	{
		void RenderSprite(const SpriteRendererComponent& renderer, const TransformComponent& transform);
		void RenderViewport(const Feather::Entity&, const Feather::Scene&, const ViewportRendererComponent& renderer, const TransformComponent& transform);

		glm::vec2 GetSpriteSize(const SpriteRendererComponent& renderer, const TransformComponent& transform);
		glm::vec2 GetViewportSize(const ViewportRendererComponent& renderer, const TransformComponent& transform);
	}
}