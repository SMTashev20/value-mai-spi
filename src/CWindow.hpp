#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Feather
{

    class CWindow
    {
    public:
        struct Properties
        {
            std::string m_Title;
            int m_Width, m_Height;

            bool m_VSyncEnabled = true, m_MSAA4XEnabled = true;
            bool m_StartMinimized = false, m_StartMaximized = false, m_StartFullScreen = false;

            Properties(const std::string& title = "Feather", int width = 1600, int height = 900)
                : m_Title(title), m_Width(width), m_Height(height) {};
        };

        CWindow(Properties props);
        virtual ~CWindow();

        bool WantsToClose();

        bool IsReady();
        bool IsFullscreen();
        bool IsHidden();
        bool IsMinimized();
        bool IsMaximized();
        bool IsFocused();
        bool IsResized();

        void SetVSync(bool state);
        void Minimize();
        void Maximize();
        void Restore();
        void HideCursor();
        void ShowCursor();

        void SetFullscreen(bool state);
        void SetTitle(const std::string& title);
        void SetPosition(glm::ivec2 pos);
        void SetSize(glm::ivec2 size);
        void SetOpacity(float opacity);
        void* GetNativeHandle();
        glm::ivec2 GetSize();
        glm::ivec2 GetPosition();

        glm::ivec2 GetCurrentMonitorDimensions();
        int GetCurrentMonitorRefreshRate();
        std::string GetCurrentMonitorName();

    private:
        Properties m_Props;
    };

}