#include "./CTexture.hpp"
#include "rlgl.h"

namespace Feather
{

	CTexture::CTexture(const std::string& path)
		: m_Texture({})
	{
		m_Path = path;
		Load(m_Path);
	}

	glm::ivec2 CTexture::GetDimensions() const
	{
		return glm::ivec2(m_Texture.width, m_Texture.height);
	}

	uint32_t CTexture::GetRendererID() const
	{
		return m_Texture.id;
	}

	const std::string& CTexture::GetPath() const
	{
		return m_Path;
	}

	const Texture& CTexture::GetRaylib() const
	{
		return m_Texture;
	}

	bool CTexture::IsLoaded() const
	{
		return m_Texture.id != 0;
	}

	void CTexture::Load(const std::string& path)
	{
		Unload();
		m_Path = path;
		m_Texture = LoadTexture(m_Path.c_str());
	}

	void CTexture::Unload()
	{
		if (m_Texture.id == 0) return;

		UnloadTexture(m_Texture);
		m_Texture = Texture{};
		m_Path = "";
	}

	CTexture::CTexture()
		: m_Texture({}), m_Path({})
	{
	}

	CTexture::~CTexture()
	{
		Unload();
	}

}