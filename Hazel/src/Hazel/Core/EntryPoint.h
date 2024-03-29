#pragma once
#include "Hazel/Core/Application.h"

// extern tells the compiler that this function exists somewhere. The compiler does not care where; 
// the linker will resolve all of this references to the one definition that it finds in one of the 
// compiled files -> in our case on the clientSide in Hazelnut.cpp
extern Hazel::Application* Hazel::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Hazel::Log::Init();

	HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup-json");
	auto app = Hazel::CreateApplication({ argc, argv });
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime-json");
	app->Run();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown-json");
	delete app;
	HZ_PROFILE_END_SESSION();
}