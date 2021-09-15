#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Hazel
{
	class HazelnutApp : public Application
	{
	public:
		HazelnutApp()
			: Application("Hazelnut Editor")
		{
			PushLayer(new EditorLayer());
		}

		~HazelnutApp() {}
	};

	Application* CreateApplication()
	{
		return new HazelnutApp();
	}
}
