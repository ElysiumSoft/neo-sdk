#include "menu.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "../csgo/sdk.h"
#include "../csgo/misc/input.h"
#include "../csgo/misc/options.h"
#include "../buildtype.h"

namespace menu {

	void init() {
		ImGui::CreateContext();
		ImGui_ImplDX9_Init(input_system::get_main_window(), csgo::d3_device);
		_visible = false;
	}

	void render() {
		if (!_visible) return;

		auto flags = ImGuiWindowFlags_NoSavedSettings | NULL | NULL | ImGuiWindowFlags_NoResize | NULL | NULL | NULL | NULL;
		auto flags_alpha = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_AlphaPreview;
		auto flags_no_alpha = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip;

		int w, h;
		csgo::engine->screen_size(w, h);

		ImGui::SetNextWindowSize({ 350.f, 500.f });
		ImGui::SetNextWindowBgAlpha(0.6f);

		if (options::menu::fontchooser)
			ImGui::ShowFontSelector("Font Chooser");

		if (options::menu::styleeditor)
			ImGui::ShowStyleEditor();

		if (options::menu::debugwin)
			ImGui::ShowDemoWindow();

		if (options::menu::stylesel)
			ImGui::ShowStyleSelector("Style Selector");

		ImGui::Begin("ElysiumHaxx =~= Menu", nullptr, flags | ImGuiWindowFlags_NoTitleBar);
		{

			ImGui::SetCursorPosX(0.f);
			ImGui::BeginTabBar("fdl", ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip);
			ImGui::SetCursorPosX(350.f);
			ImGui::EndTabBar();

			ImGui::SetCursorPos({ 25.f, 8.f });
			ImGui::BeginTabBar("Haxx", ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip);
			if (ImGui::BeginTabItem("Legit")) {
				// backtrack
				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("Backtrack", &options::legit::backtrack);

				ImGui::SetCursorPosX(5.f);

				if (options::legit::backtrack)
					ImGui::Checkbox("Extended Backtrack", &options::legit::fake_latency);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("ESP")) {
				ImGui::SetCursorPosX(5.f);
				//box
				ImGui::Checkbox("Box ESP", &options::esp::box_esp); ImGui::SameLine();
				ImGui::ColorEdit5("##Box_Color", &options::esp::box_color, flags_alpha);
				ImGui::SliderFloat("Box Weight", &options::esp::box_weight, .1f, 10.f, "%.2f");
				
				//lines
				ImGui::Checkbox("Snap Lines", &options::esp::lines); ImGui::SameLine();
				ImGui::ColorEdit5("##Lines_Color", &options::esp::line_color, flags_alpha);
				ImGui::SliderFloat("Lines Weight", &options::esp::lines_weight, .1f, 10.f, "%.2f");

				ImGui::SetCursorPosX(5.f);

				//bones
				ImGui::Checkbox("Show Bones", &options::esp::bones_esp); ImGui::SameLine();
				ImGui::ColorEdit5("##Bones_Color", &options::esp::bones_color, flags_alpha);
				ImGui::SliderFloat("Bones Weight", &options::esp::bones_weight, .1f, 10.f, "%.2f");

				ImGui::SetCursorPosX(5.f);
				//health
				ImGui::Checkbox("Show Health", &options::esp::health_esp); ImGui::SameLine();
				ImGui::ColorEdit5("##Health_Color", &options::esp::health_color, flags_alpha);

				ImGui::SetCursorPosX(5.f);
				//weapon
				ImGui::Checkbox("Show Weapon", &options::esp::weapon_esp); ImGui::SameLine();
				ImGui::ColorEdit5("##Weapon_Color", &options::esp::weapon_color, flags_alpha);

				ImGui::SetCursorPosX(5.f);
				//ammo
				ImGui::Checkbox("Show Ammo", &options::esp::ammo_esp); ImGui::SameLine();
				ImGui::ColorEdit5("##Ammo_Color", &options::esp::ammo_color, flags_alpha);
				
				ImGui::SetCursorPosX(5.f);
				//scoped
				ImGui::Checkbox("Show Scoped", &options::esp::scoped_esp); ImGui::SameLine();
				ImGui::ColorEdit5("##Scoped_Color", &options::esp::scoped_color, flags_alpha);

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Aimbot"))
			{
				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("Aimbot", &options::aim::aim_enable);
				ImGui::Checkbox("Anti-Recoil", &options::rcs::anti_recoil);

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Chams"))
			{
				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("Chams", &options::chams::on);
				ImGui::SameLine();
				ImGui::ColorEdit5("##chams_visible", &options::chams::visible, flags_alpha);

				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("Occluded Chams", &options::chams::xqz);
				ImGui::SameLine();
				ImGui::ColorEdit5("##chams_occluded", &options::chams::invisible, flags_alpha);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Settings"))
			{
				ImGui::SetCursorPosX(5.f);

				if (ImGui::Button("Choose Font"))
				{
					options::menu::fontchooser = !options::menu::fontchooser;
				}
				if (ImGui::Button("Style Editor"))
				{
					options::menu::styleeditor = !options::menu::styleeditor;
				}
				if (ImGui::Button("Debug Window"))
				{
					options::menu::debugwin = !options::menu::debugwin;
				}
				if (ImGui::Button("Style Selector"))
				{
					options::menu::stylesel = !options::menu::stylesel;
				}

				ImGui::EndTabItem();
			}
			ImGui::SetCursorPosX(250.f);
			if (ImGui::BeginTabItem("Misc"))
			{
				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("B.Hop", &options::move::bhop);

				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("Show Branding", &options::misc::watermark);
				ImGui::SameLine();
				ImGui::ColorEdit5("##Branding_Color", &options::misc::watermark_color, flags_alpha);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}

	void toggle()
	{
		_visible = !_visible;
	}

	bool is_visible()
	{
		return _visible;
	}

}