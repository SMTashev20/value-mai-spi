#include "Application.hpp"
#include "raylib.h"

Feather::Application* Feather::Application::s_Instance = nullptr;

Feather::Application::Application(CWindow::Properties winProps)
{
	m_Window = std::make_unique<CWindow>(winProps);
	m_MainScene = std::make_unique<Scene>();
}

Feather::Application* Feather::Application::Create(CWindow::Properties props)
{
	if (!s_Instance)
		s_Instance = new Application(props);
	return s_Instance;
}

void Feather::Application::Destroy()
{
	if (s_Instance)
		delete s_Instance;;
}

Feather::Application& Feather::Application::GetInstance()
{
	return *s_Instance;
}

void Feather::Application::Run()
{
	m_MainScene->OnRuntimeStart();

	while (!m_Window->WantsToClose())
	{
		m_MainScene->OnTick(GetFrameTime());
		m_MainScene->OnRender();
	}

	m_MainScene->OnRuntimeStop();
}
