#pragma once

#include "Hazel/Core/Layer.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel
{
	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		enum class ColorTheme {Dark, Hazel};

		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetech() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetColorTheme(ColorTheme theme);

	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;

		ColorTheme m_ColorTheme = ColorTheme::Dark;

		void SetDarkColorTheme();
		void SetHazelColorTheme();
	};
}

