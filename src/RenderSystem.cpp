#include "./RenderSystem.hpp"
#include <raylib.h>

namespace Feather
{
	Color GlmToRaylibColor(const glm::vec4& color)
	{
		return Color{
			(unsigned char)int(color.r * 255.f),
			(unsigned char)int(color.g * 255.f),
			(unsigned char)int(color.b * 255.f),
			(unsigned char)int(color.a * 255.f)
		};
	}

	Vector2 SpriteAnchorToRaylibVector(const SpriteRendererComponent::Anchor& anchor, const Texture& tex)
	{
		Vector2 origin = {};
		switch (anchor.m_Type)
		{
		case SpriteRendererComponent::Anchor::Type::TopLeft:
			break;
		case SpriteRendererComponent::Anchor::Type::TopCenter:
			origin.x = tex.width / 2.f;
			break;
		case SpriteRendererComponent::Anchor::Type::TopRight:
			origin.x = (float)tex.width;
			break;
		case SpriteRendererComponent::Anchor::Type::CenterLeft:
			origin.y = tex.height / 2.f;
			break;
		case SpriteRendererComponent::Anchor::Type::Center:
			origin.x = tex.width / 2.f;
			origin.y = tex.height / 2.f;
			break;
		case SpriteRendererComponent::Anchor::Type::CenterRight:
			origin.x = (float)tex.width;
			origin.y = tex.height / 2.f;
			break;
		case SpriteRendererComponent::Anchor::Type::BottomLeft:
			origin.y = (float)tex.height;
			break;
		case SpriteRendererComponent::Anchor::Type::BottomCenter:
			origin.x = tex.width / 2.f;
			origin.y = (float)tex.height;
			break;
		case SpriteRendererComponent::Anchor::Type::BottomRight:
			origin.x = (float)tex.width;
			origin.y = (float)tex.height;
			break;
		case SpriteRendererComponent::Anchor::Type::Custom:
			origin = Vector2{ anchor.m_CustomPosition.x, anchor.m_CustomPosition.y };
		}

		return origin;
	}

	namespace RenderSystem
	{
		void RenderSprite(const SpriteRendererComponent& renderer, const TransformComponent& transform)
		{
			// TODO: account for m_Shader
			const Texture& tex = renderer.m_Texture->GetRaylib();

			// --- source rectangle ---
			Rectangle source = {};
			if (renderer.m_CustomSourceRect.has_value())
			{
				source = {
					renderer.m_CustomSourceRect->m_Position.x,
					renderer.m_CustomSourceRect->m_Position.y,
					renderer.m_CustomSourceRect->m_Dimensions.x,
					renderer.m_CustomSourceRect->m_Dimensions.y
				};
			}
			else
				source = { 0.f, 0.f, (float)tex.width, (float)tex.height };

			if (renderer.m_FlipH) source.width *= -1.f;
			if (renderer.m_FlipV) source.height *= -1.f;


			// --- destination rectangle ---
			Rectangle dest = { transform.m_Translation.x, transform.m_Translation.y };
			if (renderer.m_CustomDestRect.has_value())
			{
				dest.width = renderer.m_CustomDestRect->x;
				dest.height = renderer.m_CustomDestRect->y;
			}
			else
			{
				dest.width = tex.width * transform.m_Scale.x;
				dest.height = tex.height * transform.m_Scale.y;
			}

			
			// -- origin vector --
			Vector2 origin = SpriteAnchorToRaylibVector(renderer.m_Anchor, tex);
			

			DrawTexturePro(
				tex, source, dest, origin,
				0.f, // TODO: implement rotation
				GlmToRaylibColor(renderer.m_Tint));
		}
	}
}