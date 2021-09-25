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
		//ImGui::GetIO().MouseDrawCursor = _visible; ->disgusting and bugs out
		if (!_visible) return;

		auto flags = ImGuiWindowFlags_NoSavedSettings | NULL | NULL | ImGuiWindowFlags_NoResize | NULL | NULL | NULL | NULL;
		auto flags_alpha = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_AlphaPreview;
		auto flags_no_alpha = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip;

		ImGui::SetNextWindowSize({ 400.f, 300.f });

		ImGui::Begin("neo-sdk", nullptr, flags | ImGuiWindowFlags_NoTitleBar);
		{
			ImGui::SetCursorPosX(0.f);
			ImGui::BeginTabBar("for da line", ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip);
			ImGui::SetCursorPosX(400.f);
			ImGui::EndTabBar();

			ImGui::SetCursorPos({ 115.f, 8.f });
			ImGui::BeginTabBar("features", ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip);
			if (ImGui::BeginTabItem("legit")) {
				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("backtrack", &options::legit::backtrack);

				ImGui::SetCursorPosX(5.f);

				if (options::legit::backtrack)
					ImGui::Checkbox("extended backtrack", &options::legit::fake_latency);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("esp")) {
				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("box esp", &options::esp::box_esp); ImGui::SameLine();
				ImGui::ColorEdit5("##box_color", &options::esp::box_color, flags_alpha);
				ImGui::SliderFloat("box weight", &options::esp::box_weight, 0.1f, 10.0f, "%.1f");

				ImGui::Checkbox("esp lines", &options::esp::lines); ImGui::SameLine();
				ImGui::ColorEdit5("##lines_color", &options::esp::line_color, flags_alpha);
				ImGui::SliderFloat("lines weight", &options::esp::lines_weight, 0.1f, 10.0f, "%.1f");

				ImGui::SetCursorPosX(5.f);
				ImGui::Checkbox("draw skeletons", &options::esp::bones_esp);
				ImGui::ColorEdit5("##bones_color", &options::esp::bones_color, flags_alpha);
				ImGui::SliderFloat("bones weight", &options::esp::bones_weight, 0.1f, 10.0f, "%.1f");

				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("health esp", &options::esp::health_esp); ImGui::SameLine();
				ImGui::ColorEdit5("##health_color", &options::esp::health_color, flags_alpha);

				ImGui::SetCursorPosX(5.f);
#if testing
				ImGui::Checkbox("weapon esp", &options::esp::weapon_esp); ImGui::SameLine();  //crashes -> line 447 imgui_draw.cpp | cba to fix
				ImGui::ColorEdit5("##weapon_color", &options::esp::weapon_color, flags_alpha);
#endif
				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("ammo esp", &options::esp::ammo_esp); ImGui::SameLine();
				ImGui::ColorEdit5("##ammo_color", &options::esp::ammo_color, flags_alpha);

				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("scoped esp", &options::esp::scoped_esp); ImGui::SameLine();
				ImGui::ColorEdit5("##scoped_color", &options::esp::scoped_color, flags_alpha);

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("aimbot")) {
				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("aimbot", &options::aim::aim_enable);
				ImGui::Checkbox("anti recoil", &options::rcs::anti_recoil);

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("chams")) {
				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("chams", &options::chams::on); ImGui::SameLine();
				ImGui::ColorEdit5("##chams_visible", &options::chams::visible, flags_alpha);

				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("chams xqz", &options::chams::xqz); ImGui::SameLine();
				ImGui::ColorEdit5("##chams_invisible", &options::chams::invisible, flags_alpha);
				ImGui::EndTabItem();
			}
			ImGui::SetCursorPosX(250.f);
			if (ImGui::BeginTabItem("misc")) {
				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("bhop", &options::move::bhop);


				ImGui::SetCursorPosX(5.f);

				ImGui::Checkbox("watermark", &options::misc::watermark); ImGui::SameLine();
				ImGui::ColorEdit5("##watermark_color", &options::misc::watermark_color, flags_alpha);
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