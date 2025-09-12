-- premake5.lua
workspace "Bolt"
  architecture "x86_64"
  startproject "BoltGame"
  configurations { "Debug", "Release", "RelWithDebInfo", "MinSizeRel" }
  cppdialect "C++20"
  staticruntime "Off"                -- -> /MD in allen Configs
  systemversion "latest"
  flags { "MultiProcessorCompile" }

  filter "action:vs*"
    buildoptions { "/Zc:__cplusplus", "/Zc:preprocessor" }

  filter "configurations:Debug"
    defines { "BX_CONFIG_DEBUG=1", "BGFX_CONFIG_DEBUG=1" }
    symbols "On"

  filter "configurations:Release"
    defines { "BX_CONFIG_DEBUG=0", "BGFX_CONFIG_DEBUG=0" }
    optimize "Speed"

  filter "configurations:RelWithDebInfo"
    defines { "BX_CONFIG_DEBUG=0", "BGFX_CONFIG_DEBUG=0" }
    symbols "On"
    optimize "Speed"

  filter "configurations:MinSizeRel"
    defines { "BX_CONFIG_DEBUG=0", "BGFX_CONFIG_DEBUG=0" }
    optimize "Size"

  filter {}

project "BoltCore"
  kind "StaticLib"
  language "C++"
  targetname "BoltCore"

  -- Dateien
  files {
    "BoltCore/src/**.cpp",
    "BoltCore/include/**.hpp"
  }

  -- Includes & Vendor
  includedirs {
    "BoltCore/include",
    "Vendor/External/Include"
  }
  libdirs { "Vendor/External/Library" }

  -- Alle *.lib aus Vendor automatisch linken
  links( os.matchfiles("Vendor/External/Library/*.lib") )

  -- Precompiled Header
  pchheader "Bolt/btpch.hpp"
  pchsource "BoltCore/src/btpch.cpp"  -- siehe kleine Stub-Datei unten

  -- Schicke Ordner in VS
  vpaths {
    ["Header Files/*"] = { "BoltCore/include/**.hpp" },
    ["Source Files/*"] = { "BoltCore/src/**.cpp" }
  }

  filter "action:vs*"
    buildoptions { "/Zi" }           -- wie dein CMake für BoltCore
  filter {}

project "BoltGame"
  kind "ConsoleApp"
  language "C++"
  targetname "BoltGame"

  files {
    "BoltGame/src/**.cpp",
    "BoltGame/src/**.c"
  }

  includedirs {
    "BoltCore/include",
    "Vendor/External/Include"
  }
  libdirs { "Vendor/External/Library" }
  links { "BoltCore" }
  links( os.matchfiles("Vendor/External/Library/*.lib") )

  -- Ausgabepfad & VS-Debugger-Working-Dir
  targetdir ("Binaries/%{cfg.buildcfg}")
  debugdir  ("%{cfg.targetdir}")

  -- Assets nach dem Build kopieren (wie in CMake)
  postbuildcommands {
    '{MKDIR} "%{cfg.targetdir}/Assets"',
    '{COPYDIR} "BoltGame/Assets" "%{cfg.targetdir}/Assets"'
  }

  vpaths {
    ["Source Files/*"] = { "BoltGame/src/**.cpp", "BoltGame/src/**.c" }
  }
