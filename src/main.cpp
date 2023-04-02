#include "Application.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "CTexture.hpp"
#include "CShader.hpp"
#include "CFont.hpp"
#include "RenderSystem.hpp"
#include "GameScene.hpp"

#include <nlohmann/json.hpp>

#include <memory>
#include <fstream>
#include <sstream>

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
}

struct ElementData
{
	int m_Position;
	std::string m_Name;
	std::string m_Symbol;
	std::vector<int> m_Shells;

	glm::ivec2 m_RelativePos;
};

void RenderPeriodicElement(const ElementData& data, const Feather::CFont& symFont, const Feather::CFont& nameFont,
	const int& squareSize, const float& symFontSize, const float& nameFontSize)
{
	DrawRectangleRounded({ 0.f, 0.f, squareSize - 10.f, squareSize - 10.f }, 0.2f, 20, PURPLE);
	DrawRectangle(10, 10, squareSize - 10, squareSize - 10, WHITE);
	Vector2 symbolTextDim = MeasureTextEx(symFont.GetRaylib(), data.m_Symbol.c_str(), symFontSize, 0.f);
	Vector2 nameTextDim = MeasureTextEx(nameFont.GetRaylib(), data.m_Name.c_str(), nameFontSize, 0.f);

	Vector2 symbolTextPos = { squareSize / 2.f - symbolTextDim.x / 2.f, squareSize / 2.f - symbolTextDim.y / 2.f };
	Vector2 nameTextPos = { squareSize / 2.f - nameTextDim.x / 2.f, squareSize / 2.f - nameTextDim.y / 2.f + symFontSize / 2.f + 4.f };
	DrawTextEx(symFont.GetRaylib(), data.m_Symbol.c_str(), symbolTextPos, symFontSize, 0.f, BLACK);
	DrawTextEx(nameFont.GetRaylib(), data.m_Name.c_str(), nameTextPos, nameFontSize, 0.f, BLACK);
}

void CreatePeriodicElement(Feather::Scene& scene, Feather::CFont& symFontRef, Feather::CFont& nameFontRef, const ElementData& data)
{
	const int squareSizePx = 83;
	const int spacingPx = 5;
	const float symFontSize = 35.f;
	const float nameFontSize = 15.f;

	if (symFontRef.GetRaylib().baseSize != int(symFontSize))
	{
		symFontRef.ReloadFontWithSize(int(symFontSize));
		SetTextureFilter(symFontRef.GetRaylib().texture, TEXTURE_FILTER_ANISOTROPIC_4X);
	}

	if (nameFontRef.GetRaylib().baseSize != int(nameFontSize))
	{
		nameFontRef.ReloadFontWithSize(int(nameFontSize));
		SetTextureFilter(nameFontRef.GetRaylib().texture, TEXTURE_FILTER_ANISOTROPIC_4X);
	}

	auto entity = scene.CreateEntity();

	auto elementEntity = scene.CreateEntity();
	auto viewport = Feather::ViewportRendererComponent{};
	auto buffer = std::make_shared<Feather::CFrameBuffer>(glm::ivec2(squareSizePx, squareSizePx));

	viewport.m_Buffer = buffer;

	buffer->Bind();
	RenderPeriodicElement(data, symFontRef, nameFontRef, squareSizePx, symFontSize, nameFontSize);
	buffer->Unbind();

	auto transform = Feather::TransformComponent{
		glm::vec3(
			squareSizePx * (data.m_RelativePos.x - 1) + spacingPx * (data.m_RelativePos.x - 1),
			squareSizePx * (data.m_RelativePos.y - 1) + spacingPx * (data.m_RelativePos.y - 1),
			0.f
		),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(1.f, 1.f, 1.f),
	};

	entity.AddComponent<Feather::ViewportRendererComponent>(viewport);
	entity.AddComponent<Feather::TransformComponent>(transform);
}

void SetupPeriodicTable(Feather::Scene& scene)
{
	auto managerEntity = scene.CreateEntity();

	auto script = Feather::ScriptComponent{};

	managerEntity.AddComponent<Feather::TagComponent>("PTableManager");
	managerEntity.AddComponent<Feather::ScriptComponent>(script);

	const std::string elementFilePath = "res/elements.json";
	const std::string symFontPath = "res/fonts/Inter/Inter-Bold.ttf";
	const std::string nameFontPath = "res/fonts/Inter/Inter-Light.ttf";

	auto symFontPtr = std::make_shared<Feather::CFont>(symFontPath);
	auto nameFontPtr = std::make_shared<Feather::CFont>(nameFontPath);

	if (!symFontPtr->IsLoaded())
		throw std::runtime_error("Font \"" + symFontPath + "\" doesn't exist.");

	if (!nameFontPtr->IsLoaded())
		throw std::runtime_error("Font \"" + nameFontPath + "\" doesn't exist.");
	
	nlohmann::json elementJson;
	{
		std::ifstream elementFile(elementFilePath);
		if (!elementFile.is_open())
			throw std::runtime_error("Element file \"" + elementFilePath + "\" doesn't exist.");

		std::stringstream sstr;
		sstr << elementFile.rdbuf();
		elementJson = nlohmann::json::parse(sstr.str());
	}

	auto elementArr = elementJson["elements"].get<std::vector<nlohmann::json>>();

	int positionCounter = 1;
	for (nlohmann::json& el : elementArr)
	{
		auto elData = ElementData {
			positionCounter,
			el["name"].get<std::string>(),
			el["symbol"].get<std::string>(),
			el["shells"].get<std::vector<int>>(),
			glm::ivec2(
				el["xpos"].get<int>(),
				el["ypos"].get<int>()
			)
		};

		CreatePeriodicElement(scene, *symFontPtr, *nameFontPtr, elData);
		positionCounter++;
	}
}

int main()
{
	// TODO: implement Game::GameScene

	auto app = Feather::Application::Create(Feather::CWindow::Properties());
	//DragNDropScene(app->GetMainScene());
	SetupPeriodicTable(app->GetMainScene());
	app->Run();
	Feather::Application::Destroy();
}