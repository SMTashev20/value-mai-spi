#pragma once

#include <glm/glm.hpp>
#include <raylib.h>

namespace Feather
{

	class CFrameBuffer
	{
	public:
		CFrameBuffer();
		CFrameBuffer(glm::ivec2 size);

		virtual ~CFrameBuffer();

		void Load(glm::ivec2 size);
		void Unload();

		void Bind();
		void Unbind();

		glm::ivec2 GetDimensions() const
		{
			return glm::ivec2(m_Buffer.texture.width, m_Buffer.texture.height);
		}

		void Resize(glm::ivec2 newSize);
		void Clear(glm::vec4 color);

		bool IsLoaded() const { return m_Buffer.id != 0; }

		const RenderTexture& GetRaylib() const { return m_Buffer; }

	private:
		RenderTexture m_Buffer;
	};

}