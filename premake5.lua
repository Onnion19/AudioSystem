workspace "NNGine_Audio"
	architecture "x64"
	targetdir "build"

	configurations {
		"Debug",
		"Release",
		"Distribution"
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
project "AudioSystem"

	location "AudioSystem"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


	files
	{
		"AudioSystem/*.cpp",
		"AudioSystem/Source/**.cpp",
		"AudioSystem/Source/**.c",
		"AudioSystem/Headers/**.h",
		"AudioSystem/*.h",
		"AudioSystem/Headers/**.hpp",
		"AudioSystem/ExternalInclude/**.h"
	}

	includedirs 
	{
		"AudioSystem/Headers",
		"AudioSystem"
	}


	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"NNGINE_PLATFROM_WINDOWS", 
		}

	filter "configurations:Debug"
		defines
		{
			"NNGINE_DEBUG"
		}
		symbols "On"

	filter "configurations:Release"
		defines
		{
			"NNGINE_RELEASE"
		}
		optimize "On"


	filter "configurations:Distribution"
		defines
		{
			"NNGINE_DISTRIBUTION"
		}
		optimize "Off"
	

	filter {"system:windows","configurations:Release"}
		buildoptions "/MT"
