#pragma once

#include <raylib.h>
#include <string>
#include <glm/glm.hpp>

namespace Feather
{

	class CTexture
	{
	public:
		CTexture();
		CTexture(const std::string& path);

		~CTexture();

		glm::ivec2 GetDimensions() const;
		uint32_t GetRendererID() const;

		void Load(const std::string& path);
		void Unload();

		const std::string& GetPath() const;

		const Texture& GetRaylib() const;

		bool IsLoaded() const;

	private:
		::Texture m_Texture;
		std::string m_Path;
	};

}