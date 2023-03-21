#include "./CWindow.hpp"
#include "raylib.h"

namespace Feather
{

    CWindow::CWindow(Properties props)
    {
        m_Props = props;

        if (m_Props.m_MSAA4XEnabled)
            SetConfigFlags(FLAG_MSAA_4X_HINT);

        if (m_Props.m_StartFullScreen)
            SetConfigFlags(FLAG_FULLSCREEN_MODE);

        if (m_Props.m_StartMinimized)
            SetConfigFlags(FLAG_WINDOW_MINIMIZED);

        if (m_Props.m_StartMaximized)
            SetConfigFlags(FLAG_WINDOW_MAXIMIZED);

        if (m_Props.m_VSyncEnabled)
            SetConfigFlags(FLAG_VSYNC_HINT);

        // internally raylib stores the pointer of the title string
        // instead of making a copy of it, but passing the pointer of
        // std::string shouldn't be a problem, since this window class
        // SHOULD live while the window is open
        InitWindow(m_Props.m_Width, m_Props.m_Height, m_Props.m_Title.c_str());
    }

    CWindow::~CWindow()
    {
        CloseWindow();
    }

    bool CWindow::WantsToClose()
    {
        return WindowShouldClose();
    }

    bool CWindow::IsReady()
    {
        return IsWindowReady();
    }

    bool CWindow::IsFullscreen()
    {
        return IsWindowFullscreen();
    }

    bool CWindow::IsHidden()
    {
        return IsWindowHidden();
    }

    bool CWindow::IsMinimized()
    {
        return IsWindowMinimized();
    }

    bool CWindow::IsMaximized()
    {
        return IsWindowMaximized();
    }

    bool CWindow::IsFocused()
    {
        return IsWindowFocused();
    }

    bool CWindow::IsResized()
    {
        return IsWindowResized();
    }

    void CWindow::SetVSync(bool state)
    {
        if (state)
            SetWindowState(FLAG_VSYNC_HINT);
        // raylib doesn't have the ability to disable vsync
    }
    void CWindow::Minimize()
    {
        MinimizeWindow();
    }
    void CWindow::Maximize()
    {
        MaximizeWindow();
    }
    void CWindow::Restore()
    {
        RestoreWindow();
    }
    void CWindow::HideCursor()
    {
        ::HideCursor();
    }
    void CWindow::ShowCursor()
    {
        ::ShowCursor();
    }

    void CWindow::SetFullscreen(bool state)
    {
        if (state && !IsFullscreen())
            ::ToggleFullscreen();
        else if (IsFullscreen())
            ::ToggleFullscreen();
    }

    // void SetWindowIcon(Image image);
    void CWindow::SetTitle(const std::string& title)
    {
        m_Props.m_Title = title;
        SetWindowTitle(m_Props.m_Title.c_str());
    }
    void CWindow::SetPosition(glm::ivec2 pos)
    {
        SetWindowPosition(pos.x, pos.y);
    }

    void CWindow::SetSize(glm::ivec2 size)
    {
        SetWindowSize(size.x, size.y);
    }

    void CWindow::SetOpacity(float opacity)
    {
        SetWindowOpacity(opacity);
    }

    void* CWindow::GetNativeHandle()
    {
        return GetWindowHandle();
    }

    glm::ivec2 CWindow::GetPosition(void)
    {
        Vector2 pos = GetWindowPosition();
        return glm::ivec2((int)pos.x, (int)pos.y);
    }

    glm::ivec2 CWindow::GetSize()
    {
        return glm::ivec2(GetScreenWidth(), GetScreenHeight());
    }

    glm::ivec2 CWindow::GetCurrentMonitorDimensions()
    {
        int monitor = GetCurrentMonitor();
        return glm::ivec2(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
    }

    int CWindow::GetCurrentMonitorRefreshRate()
    {
        return GetMonitorRefreshRate(GetCurrentMonitor());
    }

    std::string CWindow::GetCurrentMonitorName()
    {
        return std::string{ GetMonitorName(GetCurrentMonitor()) };
    }

}