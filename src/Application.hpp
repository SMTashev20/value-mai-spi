#pragma once

#include "CWindow.hpp"
#include "Scene.hpp"
#include <memory>

namespace Feather
{
	class Application
	{
	public:
		Application(CWindow::Properties);
		virtual ~Application() = default;
		
		static Application& GetInstance();

		CWindow& GetWindow() { return *m_Window; }
		Scene& GetMainScene() { return *m_MainScene; }

		static Application* Create(CWindow::Properties props);
		static void Destroy();
		void Run();

	private:
		std::unique_ptr<CWindow> m_Window;
		std::unique_ptr<Scene> m_MainScene;

		static Application* s_Instance;
	};
}