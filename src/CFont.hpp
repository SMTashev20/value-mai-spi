#pragma once

#include <string>
#include <raylib.h>

namespace Feather
{
	class CFont
	{
	public:
		CFont(std::string path);
		virtual ~CFont();

		void Load(std::string path, int sizePx = 20);
		void Unload();

		void ReloadFontWithSize(int sizePx);

		const Font& GetRaylib() const { return m_Font; };
		bool IsLoaded() const { return m_Font.texture.id != 0; }

	private:
		Font m_Font = {};
		std::string m_Path = "";
	};
}
