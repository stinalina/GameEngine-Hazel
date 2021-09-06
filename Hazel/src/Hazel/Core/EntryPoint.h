#pragma once
#include "Hazel.h"

#ifdef HZ_PLATFORM_WINDOWS

// extern tells the compiler that this function exits somewhere. The compiler does not care where; 
// the linker will resolve all of this references to the one definition that it finds in one of the 
// compiled files -> in our case on the clientSide in SandboxApp.cpp
extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif