workspace "G-Engine"
	architecture "x64"
	targetdir "build"
	
	configurations 
	{ 
		"Debug", 
		"Release",
		"Dist"
	}

	startproject "G-Enginenut"
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Glad"] = "G-Engine/vendor/Glad/include"
IncludeDir["GLFW"] = "G-Engine/vendor/GLFW/include"
IncludeDir["ImGui"] = "G-Engine/vendor/ImGui"
IncludeDir["glm"] = "G-Engine/vendor/glm"

include "G-Engine/vendor/GLFW"
include "G-Engine/vendor/Glad"
include "G-Engine/vendor/ImGui"

project "G-Engine"
	location "G-Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gepch.h"
	pchsource "G-Engine/src/gepch.cpp"

	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp" 
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{prj.name}/vendor/assimp/include",
		"%{prj.name}/vendor/stb/include"
	}
	
	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"
		
		defines 
		{ 
			"GE_PLATFORM_WINDOWS",
			"GE_BUILD_DLL"
		}

	filter "configurations:Debug"
		defines "GE_DEBUG"
		symbols "On"
				
	filter "configurations:Release"
		defines "GE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "GE_DIST"
		optimize "On"

project "G-Enginenut"
	location "G-Enginenut"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	links 
	{ 
		"G-Engine"
	}
	
	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp" 
	}
	
	includedirs 
	{
		"%{prj.name}/src",
		"G-Engine/src",
		"G-Engine/vendor",
		"%{IncludeDir.glm}"
	}

	postbuildcommands 
	{
		'{COPY} "../G-Enginenut/assets" "%{cfg.targetdir}/assets"'
	}
	
	filter "system:windows"
		systemversion "latest"
				
		defines 
		{ 
			"GE_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "GE_DEBUG"
		symbols "on"

		links
		{
			"G-Engine/vendor/assimp/bin/Debug/assimp-vc141-mtd.lib"
		}

		postbuildcommands 
		{
			'{COPY} "../G-Engine/vendor/assimp/bin/Debug/assimp-vc141-mtd.dll" "%{cfg.targetdir}"'
		}
				
	filter "configurations:Release"
		defines "GE_RELEASE"
		optimize "on"

		links
		{
			"G-Engine/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		}

		postbuildcommands 
		{
			'{COPY} "../G-Engine/vendor/assimp/bin/Release/assimp-vc141-mt.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Dist"
		defines "GE_DIST"
		optimize "on"

		links
		{
			"G-Engine/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		}

		postbuildcommands 
		{
			'{COPY} "../G-Engine/vendor/assimp/bin/Release/assimp-vc141-mtd.dll" "%{cfg.targetdir}"'
		}
		
--[[project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	links 
	{ 
		"G-Engine"
	}
	
	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp" 
	}
	
	includedirs 
	{
		"%{prj.name}/src",
		"G-Engine/src",
		"G-Engine/vendor",
		"%{IncludeDir.glm}"
	}
	
	filter "system:windows"
		systemversion "latest"
				
		defines 
		{ 
			"GE_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "GE_DEBUG"
		symbols "on"

		links
		{
			"G-Engine/vendor/assimp/bin/Debug/assimp-vc141-mtd.lib"
		}
				
	filter "configurations:Release"
		defines "GE_RELEASE"
		optimize "on"

		links
		{
			"G-Engine/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		}

	filter "configurations:Dist"
		defines "GE_DIST"
		optimize "on"

		links
		{
			"G-Engine/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		}
--]]