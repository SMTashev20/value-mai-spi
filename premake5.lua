workspace "LAC"
   configurations { "Debug", "Release" }
   architecture "x64"

project "Game"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "On"

   filter "configurations:Debug"
      targetdir "bin/Debug"
      objdir "obj/Debug"
   filter "configurations:Release"
      targetdir "bin/Release"
      objdir "obj/Release"
   filter {}
   
   files {
      "src/*.hpp",
      "src/*.cpp",
      "res/icon.rc",
   }

   includedirs {
      "vendor/entt/src",
      "vendor/raylib/include",
      "vendor/glm",
      "src"
   }

   filter "system:windows"
      links { "raylibdll" }
      libdirs {
         "vendor/raylib/lib-windows"
      }
   filter "system:linux or system:bsd"
      links { "raylib", "GL", "m", "pthread", "dl", "rt", "X11" }
      libdirs {
         "vendor/raylib/lib-unix"
      }
   filter "system:macosx"
      -- TODO: figure out linker input
      libdirs {
         "vendor/raylib/lib-darwin"
      }
   filter {}
   
   filter "configurations:Debug"
      postbuildcommands {
         ("{COPY} vendor/raylib/lib-windows/raylib.dll ./bin/Debug/")
      }
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      postbuildcommands {
         ("{COPY} vendor/raylib/lib-windows/raylib.dll ./bin/Release/")
      }
      defines { "NDEBUG" }
      optimize "On"
   filter {}