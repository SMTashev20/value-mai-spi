#include "Application.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "CTexture.hpp"
#include "CShader.hpp"
#include <memory>

void DragNDropScene(Feather::Scene& sceneRef);

void NextScene(Feather::Scene& sceneRef)
{
	{
		auto entity = sceneRef.CreateEntity();

		auto scriptComp = Feather::ScriptComponent{};

		scriptComp.m_OnUpdate = [](Feather::Scene& scene, Feather::Entity&, float) {
			if (IsKeyReleased(KEY_RIGHT))
			{
				scene.DestroyAll();
				DragNDropScene(scene);
			}
		};

		entity.AddOrReplaceComponent<Feather::ScriptComponent>(scriptComp);
	}
}

void DragNDropScene(Feather::Scene& sceneRef)
{
	{
		auto entity = sceneRef.CreateEntity();
		auto texture = std::make_shared<Feather::CTexture>("res/developer_texture.png");
		auto shader = std::make_shared<Feather::CShader>(std::optional<std::string>{}, "res/fs.glsl");

		auto renderer = Feather::SpriteRendererComponent{
			texture,
			shader
		};

		renderer.m_Anchor.m_Type = Feather::SpriteRendererComponent::Anchor::Type::Center;
		entity.AddOrReplaceComponent<Feather::SpriteRendererComponent>(renderer);

		entity.AddOrReplaceComponent<Feather::TransformComponent>(Feather::TransformComponent{
			glm::vec3{},
			glm::vec3{0.f, 0.f, 0.f},
			glm::vec3{1.f, 1.f, 1.f}
			});

		auto scriptComp = Feather::ScriptComponent{};
		scriptComp.m_OnUpdate = [](Feather::Scene& scene, Feather::Entity& entity, float delta) {
			auto& transform = entity.GetComponent<Feather::TransformComponent>();

			transform.m_Translation.x += delta * 5.f;
			transform.m_Translation.y += delta * 2.f;
			transform.m_Rotation.x += delta * 10.f;
		};
		entity.AddOrReplaceComponent<Feather::ScriptComponent>(scriptComp);
	}

	{
		auto entity = sceneRef.CreateEntity();

		auto scriptComp = Feather::ScriptComponent{};

		scriptComp.m_OnUpdate = [](Feather::Scene& scene, Feather::Entity&, float) {
			if (IsKeyReleased(KEY_RIGHT))
			{
				scene.DestroyAll();
				NextScene(scene);
			}
		};

		entity.AddOrReplaceComponent<Feather::ScriptComponent>(scriptComp);
	}
}

int main()
{
	auto app = Feather::Application::Create(Feather::CWindow::Properties());
	DragNDropScene(app->GetMainScene());
	app->Run();
	Feather::Application::Destroy();
}