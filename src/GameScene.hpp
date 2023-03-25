#pragma once

#include "Entity.hpp"
#include "Scene.hpp"
#include "Components.hpp"
#include "RenderSystem.hpp"

#include <raylib.h>
#include <nlohmann/json.hpp>

namespace Game
{

	Feather::Entity CreateDragNDropManager(Feather::Scene& sceneRef)
	{
		auto entity = sceneRef.CreateEntity();

		auto tag = Feather::TagComponent{ "DragNDropManager" };

		auto script = Feather::ScriptComponent{};
		script.m_VariableMap.insert(std::pair("m_CurrentlyHeldEntity", Feather::Entity(entt::null, &sceneRef)));

		// methods to hook to
		script.m_VariableMap.insert(std::pair("m_OnReleased", std::function<void(Feather::Entity&)>{[](Feather::Entity&) {}}));
		script.m_VariableMap.insert(std::pair("m_OnBeginHold", std::function<void(Feather::Entity&)>{[](Feather::Entity&) {}}));
		script.m_VariableMap.insert(std::pair("m_OnMove", std::function<void(Feather::Entity&)>{[](Feather::Entity&) {}}));


		script.m_OnUpdate = [](Feather::Scene& scene, Feather::Entity& managerEntity, float) {
			std::vector<Feather::Entity> entities = scene.GetEntitiesWithTagName("DragNDropEntity");
			Vector2 mousePos = GetMousePosition();

			auto& script = managerEntity.GetComponent<Feather::ScriptComponent>();
			Feather::Entity& currHeld = std::any_cast<Feather::Entity&>(script.m_VariableMap.at("m_CurrentlyHeldEntity"));

			// if there is an entity that is held
			// execute logic for it
			if (currHeld.GetHandle() != entt::null)
			{
				if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
				{
					std::any_cast<std::function<void(Feather::Entity&)>>(script.m_VariableMap.at("m_OnReleased"))(currHeld);
					currHeld = Feather::Entity(entt::null, &scene);
				}
				else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				{
					std::any_cast<std::function<void(Feather::Entity&)>>(script.m_VariableMap.at("m_OnMove"))(currHeld);
					auto& transform = currHeld.GetComponent<Feather::TransformComponent>();
					transform.m_Translation.x = mousePos.x;
					transform.m_Translation.y = mousePos.y;
				}

				return;
			}

			// else find one entity to hold
			for (auto& entity : entities)
			{
				auto& transform = entity.GetComponent<Feather::TransformComponent>();

				Rectangle boundingRect = {
					transform.m_Translation.x,
					transform.m_Translation.y,
				};

				// determine size for boundingRect
				if (entity.HasComponent<Feather::SpriteRendererComponent>())
				{
					auto& renderer = entity.GetComponent<Feather::SpriteRendererComponent>();

					auto size = Feather::RenderSystem::GetSpriteSize(renderer, transform);
					boundingRect.width = size.x;
					boundingRect.height = size.y;
				}
				else if (entity.HasComponent<Feather::ViewportRendererComponent>())
				{
					auto& renderer = entity.GetComponent<Feather::ViewportRendererComponent>();

					auto size = Feather::RenderSystem::GetViewportSize(renderer, transform);

					boundingRect.width = size.x;
					boundingRect.height = size.y;
				}

				// to account for the centered origin of all sprites
				boundingRect.x -= boundingRect.width / 2.f;
				boundingRect.y -= boundingRect.height / 2.f;

				// check if the currently checked sprite is actually clicked on
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, boundingRect))
				{
					std::any_cast<std::function<void(Feather::Entity&)>>(script.m_VariableMap.at("m_OnBeginHold"))(currHeld);
					managerEntity.GetComponent<Feather::ScriptComponent>().m_VariableMap.at("m_CurrentlyHeldEntity")
						= Feather::Entity(entity.GetHandle(), &scene);
				}
			}
		};

		entity.AddOrReplaceComponent<Feather::TagComponent>(tag);
		entity.AddOrReplaceComponent<Feather::ScriptComponent>(script);

		return entity;
	}

	Feather::Entity CreateDragNDropSprite(
		Feather::Scene& sceneRef,
		const glm::vec2& initialPos,
		std::shared_ptr<Feather::CTexture> texturePtr,
		std::shared_ptr<Feather::CShader> shaderPtr)
	{
		auto entity = sceneRef.CreateEntity();

		auto tag = Feather::TagComponent{ "DragNDropEntity" };

		auto renderer = Feather::SpriteRendererComponent{};
		renderer.m_Texture = texturePtr;
		renderer.m_Shader = shaderPtr;
		renderer.m_Anchor.m_Type = Feather::SpriteRendererComponent::Anchor::Type::Center;

		auto transform = Feather::TransformComponent{};
		transform.m_Translation = glm::vec3(initialPos.x, initialPos.y, 0.f);
		transform.m_Rotation = glm::vec3();
		transform.m_Scale = glm::vec3(1.f, 1.f, 1.f);

		entity.AddOrReplaceComponent<Feather::TagComponent>(tag);
		entity.AddOrReplaceComponent<Feather::SpriteRendererComponent>(renderer);
		entity.AddOrReplaceComponent<Feather::TransformComponent>(transform);

		return entity;
	}

	void GameScene(Feather::Scene& sceneRef)
	{

	}

}