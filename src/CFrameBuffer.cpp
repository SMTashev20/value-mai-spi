#include <raylib.h>
#include "./CFrameBuffer.hpp"

namespace Feather
{

	CFrameBuffer::CFrameBuffer(glm::ivec2 dimensions)
		: m_Buffer({})
	{
		Load(dimensions);
	}

	CFrameBuffer::~CFrameBuffer()
	{
		Unload();
	}

	void CFrameBuffer::Bind()
	{
		if (!IsLoaded()) return;
		BeginTextureMode(m_Buffer);
	}

	void CFrameBuffer::Unbind()
	{
		if (!IsLoaded()) return;
		EndTextureMode();
	}

	void CFrameBuffer::Resize(glm::ivec2 newDimensions)
	{
		Unload();
		Load(newDimensions);
	}

	void CFrameBuffer::Clear(glm::vec4 color)
	{
		if (!IsLoaded()) return;
		Bind();
		ClearBackground(
			Color{
				unsigned char(int(color.r * 255.f)),
				unsigned char(int(color.g * 255.f)),
				unsigned char(int(color.b * 255.f)),
				unsigned char(int(color.a * 255.f)),
			});
		Unbind();
	}

	CFrameBuffer::CFrameBuffer()
		: m_Buffer({})
	{
	}

	void CFrameBuffer::Load(glm::ivec2 size)
	{
		Unload();
		m_Buffer = LoadRenderTexture(size.x, size.y);
	}

	void CFrameBuffer::Unload()
	{
		if (!IsLoaded()) return;
		UnloadRenderTexture(m_Buffer);
		m_Buffer = RenderTexture{};
	}
}