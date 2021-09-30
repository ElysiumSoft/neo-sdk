#include "debugmenu.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "../csgo/sdk.h"
#include "../csgo/globals.h"
#include "../csgo/misc/input.h"
#include "../csgo/misc/options.h"
#include "../buildtype.h"

namespace debugmenu
{

	void init()
	{
		_visible = false;
	}

	void render()
	{
		if (!_visible) return;
#ifndef debug || testing
		return;
#endif

		auto flags = ImGuiWindowFlags_NoSavedSettings | NULL | NULL | ImGuiWindowFlags_NoResize | NULL | NULL | NULL | NULL;
		auto flags_alpha = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_AlphaPreview;
		auto flags_no_alpha = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip;

		int w, h;
		csgo::engine->screen_size(w, h);

		ImGui::SetNextWindowSize({500.f, 960.f});
		ImGui::SetNextWindowBgAlpha(0.75f);

		/* =~=~=~=~=~=~=[ CREATION OF THE DEBUG MENU ]=~=~=~=~=~=~= */
		ImGui::Begin("ElysiumHaxx =~= DEBUG MENU", nullptr, flags);
		{
			ImGui::SetCursorPosX(0.f);
			
		}
	}

	void toggle()
	{
		_visible = !_visible;
	}

	bool is_visible()
	{
		return _visible;
	}

	void dumper()
	{

	}

}