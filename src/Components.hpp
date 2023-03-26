#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <optional>
#include <functional>
#include <unordered_map>
#include <any>
#include "./CTexture.hpp"
#include "./CShader.hpp"
#include "./CFrameBuffer.hpp"
#include "./Entity.hpp"

namespace Feather
{
	
	// TODO: this is not a component it should
	//       be moved somewhere else
	struct GlmRectangle
	{
		glm::vec2 m_Position;
		glm::vec2 m_Dimensions;
	};

	// used to exclude entities from rendering stage
	struct HideComponent {};

	struct TagComponent
	{
		std::string m_Tag = "Default";
	};

	struct TransformComponent
	{
		glm::vec3 m_Translation;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
	};

	struct ViewportRendererComponent
	{
		std::shared_ptr<CFrameBuffer> m_Buffer;
		std::shared_ptr<CShader> m_Shader;

		glm::vec4 m_Tint = glm::vec4(1.f, 1.f, 1.f, 1.f);

		bool m_FlipH = false;
		bool m_FlipV = false;
	};

	struct SpriteRendererComponent
	{
		std::shared_ptr<CTexture> m_Texture;
		std::shared_ptr<CShader> m_Shader; // (optional)

		// Tint color
		glm::vec4 m_Tint = glm::vec4(1.f, 1.f, 1.f, 1.f);

		// Texture horizontal flip member
		bool m_FlipH = false;

		// Texture vertical flip member
		bool m_FlipV = false;

		// Texture will be translated, rotated and scaled
		// accordingly to this Anchor member
		struct Anchor
		{
			enum class Type
			{
				TopLeft,    TopCenter,    TopRight,
				CenterLeft, Center,       CenterRight,
				BottomLeft, BottomCenter, BottomRight,

				Custom
			} m_Type;

			glm::vec2 m_CustomPosition;
		} m_Anchor = { Anchor::Type::TopLeft, glm::vec2(0, 0) };

		// Scaling using pixels, instead of fractions
		std::optional<glm::vec2> m_CustomDestRect = {};

		// Used to render a specific region of a texture
		// NOTE: a texture alias workaround lol
		std::optional<GlmRectangle> m_CustomSourceRect = {};
	};

	struct ModelRendererComponent
	{
		// not yet :P
	};

	// forward decl
	class Scene;

	// TODO
	struct ScriptComponent
	{
		std::unordered_map<std::string, std::any> m_VariableMap;

		// Scene& mainScene
		std::function<void(Scene&, Entity&)> m_OnCreate = [](Scene&, Entity&) {};
		// Scene& mainScene, float deltaTimeSeconds
		std::function<void(Scene&, Entity&, float)> m_OnUpdate = [](Scene&, Entity&, float) {};
		// Scene& mainScene
		std::function<void(Scene&, Entity&)> m_OnDestroy = [](Scene&, Entity&) {};
	};

}