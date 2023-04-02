#include "./CFont.hpp"

namespace Feather
{
	CFont::CFont(std::string path)
	{
		Load(path);
	}

	CFont::~CFont()
	{
		Unload();
	}

	void CFont::Load(std::string path, int sizePx)
	{
		Unload();
		m_Path = path;
		m_Font = LoadFontEx(m_Path.c_str(), sizePx, nullptr, 0);
	}

	void CFont::Unload()
	{
		if (m_Font.texture.id == 0) return;
		UnloadFont(m_Font);
		m_Font = {};
	}

	void CFont::ReloadFontWithSize(int sizePx)
	{
		if (!FileExists(m_Path.c_str())) return;
		else Load(m_Path, sizePx);
	}
}