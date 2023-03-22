#include "Application.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "CTexture.hpp"
#include <memory>

void DragNDropScene(Feather::Scene& sceneRef)
{
	{
		auto entity = sceneRef.CreateEntity();
		auto texture = std::make_shared<Feather::CTexture>("res/developer_texture.png");
		entity.AddOrReplaceComponent<Feather::SpriteRendererComponent>(Feather::SpriteRendererComponent{
			texture
		});

		entity.AddOrReplaceComponent<Feather::TransformComponent>(Feather::TransformComponent{
			glm::vec3{},
			glm::vec4{0.f, 0.f, 0.f, 1.f},
			glm::vec3{1.f, 1.f, 1.f}
		});

		auto scriptComp = Feather::ScriptComponent{};
		scriptComp.m_OnUpdate = [](Feather::Scene& scene, Feather::Entity& entity, float delta) {
			auto& transform = entity.GetComponent<Feather::TransformComponent>();

			transform.m_Translation.x += delta * 5.f;
			transform.m_Translation.y += delta * 2.f;
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