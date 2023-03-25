#include "Application.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "CTexture.hpp"
#include "CShader.hpp"
#include "RenderSystem.hpp"
#include "GameScene.hpp"
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
		auto waterTex = std::make_shared<Feather::CTexture>("res/chemical/water.png");
		auto nitrogenTex = std::make_shared<Feather::CTexture>("res/chemical/nitrogen.png");
		auto oxygenTex = std::make_shared<Feather::CTexture>("res/chemical/oxygen.png");
		auto ozoneTex = std::make_shared<Feather::CTexture>("res/chemical/ozone.png");

		Game::CreateDragNDropSprite(sceneRef, glm::vec2(0.f, 0.f), waterTex, nullptr);
		Game::CreateDragNDropSprite(sceneRef, glm::vec2(30.f, 30.f), nitrogenTex, nullptr);
		Game::CreateDragNDropSprite(sceneRef, glm::vec2(100.f, 50.f), ozoneTex, nullptr);
		auto entity = Game::CreateDragNDropSprite(sceneRef, glm::vec2(200.f, 200.f), oxygenTex, nullptr);
		entity.AddComponent<Feather::ScriptComponent>();
		entity.GetComponent<Feather::ScriptComponent>().m_OnUpdate = [](Feather::Scene&, Feather::Entity& entity, float) {
			entity.GetComponent<Feather::TransformComponent>().m_Translation.x++;
			entity.GetComponent<Feather::TransformComponent>().m_Translation.y++;
		};
		Game::CreateDragNDropManager(sceneRef);
	}

	{
		auto devTex = std::make_shared<Feather::CTexture>("res/developer_texture.png");
		auto shader = std::make_shared<Feather::CShader>(std::optional<std::string>{}, "res/fs.glsl");

		auto entity = sceneRef.CreateEntity();

		auto transformComponent = Feather::TransformComponent{
			{ 0.f, 0.f, 0.f },
			{ 0.f, 0.f, 0.f },
			{ 1.f, 1.f, 1.f }
		};

		auto spriteRendererComponent = Feather::SpriteRendererComponent{
			devTex
		};

		auto winRes = Feather::Application::GetInstance().GetWindow().GetSize();
		spriteRendererComponent.m_CustomDestRect = glm::vec2(winRes.x, winRes.y);

		entity.AddComponent<Feather::TagComponent>();
		entity.AddComponent<Feather::TransformComponent>(transformComponent);
		entity.AddComponent<Feather::SpriteRendererComponent>(spriteRendererComponent);
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
	// TODO: implement Game::GameScene

	auto app = Feather::Application::Create(Feather::CWindow::Properties());
	DragNDropScene(app->GetMainScene());
	app->Run();
	Feather::Application::Destroy();
}