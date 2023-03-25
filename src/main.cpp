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
		auto texture = std::make_shared<Feather::CTexture>("res/chemical/oxygen.png");
		Game::CreateDragNDropSprite(sceneRef, glm::vec2(0.f, 0.f), texture, nullptr);
		Game::CreateDragNDropSprite(sceneRef, glm::vec2(30.f, 30.f), texture, nullptr);
		Game::CreateDragNDropSprite(sceneRef, glm::vec2(100.f, 50.f), texture, nullptr);
		Game::CreateDragNDropManager(sceneRef);
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