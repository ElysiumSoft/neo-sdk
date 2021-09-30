#pragma once

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/impl/imgui_impl_dx9.h"

//#include <vector>
#include <map>
#include <list>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

using std::string;
using std::map;
//using std::vector;

#include <ctype.h>
#include <cstdint>
#include <cstddef>

#include "../csgo/globals.h"
#include "../csgo/sdk.h"
#include "../csgo/misc/options.h"
#include "../csgo/misc/input.h"
#include "../buildtype.h"

typedef struct ESColor
{
public:
	std::string strName;
	const char* szName;
	float red, green, blue;
	float alpha = 1.0f;

	ESColor EScolor(std::string name, float red, float green, float blue, float alpha = 1.0f)
	{
		strName = name;
		szName = name.c_str();
		red = red;
		green = green;
		blue = blue;
		alpha = alpha;
		return *this;
	};

};


#define IMGCOLOR(name,R,G,B) \
	ImU32 name = GetColorU32({R,G,B,1.0f}); \
	ESColor::ESColorMap.try_emplace<std::string,ImU32>(#name, GetColorU32(R,G,B,1.0f))


#define IMCOLOR(RR,GG,BB,AA) ImGui::GetColorU32({RR,GG,BB,AA})

/// <summary>
/// This namespace contains all of my custom color definitions,
/// when adding a color with <code>IMGCOLOR(<name>, <red>, <green>, <blue>)</code>
/// the new color is automagically added to this map.
/// 
/// The colors can also just be accessed directly through the namespace,
/// but I added the map for easy iteration for debugging / display
/// </summary>


 
namespace ESColor
{

	void test()
	{
		boost::serialization::
	}

	/* =~=~=~=~=[ FLATUICOLORS ~=~> TURKISH PALETTE ]=~=~=~=~= */
	IMGCOLOR(red, 255.0f, 56.0f, 56.0f);
	IMGCOLOR(light_red, 255.0f, 77.0f, 77.0f);
	IMGCOLOR(green, 58.0f, 227.0f, 116.0f);
	IMGCOLOR(light_green, 50.0f, 255.0f, 126.0f);
	IMGCOLOR(blue, 23.0f, 192.0f, 235.0f);
	IMGCOLOR(light_blue, 24.0f, 220.0f, 255.0f);
	IMGCOLOR(orange, 255.0f, 159.0f, 26.0f);
	IMGCOLOR(light_orange, 255.0f, 175.0f, 64.0f);
	IMGCOLOR(pink, 255.0f, 184.0f, 184.0f);
	IMGCOLOR(light_pink, 255.0f, 204.0f, 204.0f)
	IMGCOLOR(yellow, 255.0f, 242.0f, 0.0f);
	IMGCOLOR(light_yellow, 255.0f, 250.0f, 101.0f);
	IMGCOLOR(purple, 197.0f, 108.0f, 240.0f);
	IMGCOLOR(light_purple, 205.0f, 132.0f, 241.0f);
	IMGCOLOR(cyan, 103.0f, 230.0f, 220.0f);
	IMGCOLOR(light_cyan, 126.0f, 255.0f, 245.0f);
	IMGCOLOR(dark_grey, 75.0f, 75.0f, 75.0f);
	IMGCOLOR(near_black, 61.0f, 61.0f, 61.0f);
	IMGCOLOR(violet, 113.0f, 88.0f, 226.0f);

	/* =~=~=~=~=[ DIGITALSYNOPSIS ~=~> RAINBOW DASK ]=~=~=~=~= */
	IMGCOLOR(rbw_red, .0f, .0f, .0f);
	IMGCOLOR(rbw_orange, 0.f, 0.f, 0.f);
	IMGCOLOR(rbw_yellow, 0.0f, 0.0f, 0.0f);
	IMGCOLOR(rbw_green, 0.0f, 0.0f, 0.0f);
	IMGCOLOR(rbw_blue, 0.0f, 0.0f, 0.0f);

	/* =~=~=~=~=[ DIGITALSYNOPSIS ~=~> PASTEL RAINBOW ]=~=~=~=~= */
	IMGCOLOR(pastel_turquoise, 168.0f, 230.0f, 207.0f);
	IMGCOLOR(pastel_green, 220.0f, 237.0f, 193.0f);
	IMGCOLOR(pastel_orange, 255.0f, 211.0f, 182.0f);
	IMGCOLOR(pastel_pink, 255.0f, 170.0f, 165.0f);
	IMGCOLOR(pastel_red, 255.0f, 139.0f, 148.0f);
}

namespace ImGui
{
	ImU32 togglebutton_on = ImGui::GetColorU32({ 50.0f, 255.0f, 126.0f, 1.0f });
	ImU32 togglebutton_off = ImGui::GetColorU32({ 255.f, 77.f, 77.f, 1.0f });
	ImU32 togglebutton_bg = ImGui::GetColorU32({ 75.f, 75.f, 75.f, 1.0f });
	ImU32 togglebutton_border = ImGui::GetColorU32({ 75.f, 75.f, 75.f, 1.0f });

	inline ImGuiWindow* GetCurrentWindowRead() { ImGuiContext& g = *GImGui; return g.CurrentWindow; }
	inline ImGuiWindow* GetCurrentWindow() { ImGuiContext& g = *GImGui; g.CurrentWindow->WriteAccessed = true; return g.CurrentWindow; }
	IMGUI_API ImGuiWindow* FindWindowByID(ImGuiID id);
	IMGUI_API ImGuiWindow* FindWindowByName(const char* name);

	/// <summary>
	/// Togglable button extension for ImGui, Created by ElysiumSoft
	/// Default design is similar to the sliding toggle buttons on IOS
	/// and Android
	/// </summary>
	/// <param name="label">The text to display with the item</param>
	/// <param name="v">Pointer to bool variable the ToggleButton represents</param>
	/// <returns>bool value. indicates whether the item is in a pressed state</returns>
	bool ImGui::ToggleButton(const char* label, bool* v)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = CalcTextSize(label, NULL, true);

		const float square_sz = GetFrameHeight();
		const ImVec2 pos = window->DC.CursorPos;
		const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
		ItemSize(total_bb, style.FramePadding.y);
		if (!ItemAdd(total_bb, id))
		{
			IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlagsChecked : 0));
			return false;
		}

		bool hovered, held;
		bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
		if (pressed)
		{
			*v = !(*v);
			MarkItemEdited(id);
		}

		const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
		RenderNavHighlight(total_bb, id);
		RenderFrame(check_bb.Min, check_bb.Max, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
		ImU32 check_col = GetColorU32(ImGuiCol_CheckMark);
		bool mixed_value = (g.LastItemData.InFlags & ImGuiItemFlags_MixedValue) != 0;
		if (mixed_value)
		{
			ImVec2 pad(ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)), ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)));
			window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, style.FrameRounding);
		}
		else if (*v)
		{
			const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
			RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
		}

		ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
		if (g.LogEnabled)
			LogRenderedText(&label_pos, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");
		if (label_size.x > 0.0f)
			RenderText(label_pos, label);

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
		return pressed;
	}

	/*=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~[  ]=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~*/

	bool ImGui::ESRadioButton(const char* label, bool active)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = CalcTextSize(label, NULL, true);

		const float square_sz = GetFrameHeight();
		const ImVec2 pos = window->DC.CursorPos;
		const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
		const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
		ItemSize(total_bb, style.FramePadding.y);
		if (!ItemAdd(total_bb, id)) return false;

		ImVec2 center = check_bb.GetCenter();
		center.x = IM_ROUND(center.x);
		center.y = IM_ROUND(center.y);
		const float radius = (square_sz - 1.0f) * 0.5f;

		bool hovered, held;
		bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
		if (pressed) MarkItemEdited(id);

		RenderNavHighlight(total_bb, id);
		window->DrawList->AddCircleFilled(center, radius, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 16);
		if (active)
		{
			const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
			window->DrawList->AddCircleFilled(center, radius - pad, GetColorU32(togglebutton_on), 16);
		}

		if (style.FrameBorderSize > 0.0f)
		{
			window->DrawList->AddCircle(center + ImVec2(1, 1), radius, GetColorU32(togglebutton_bg), 16, style.FrameBorderSize);
			window->DrawList->AddCircle(center, radius, GetColorU32(togglebutton_border), 16, style.FrameBorderSize);
		}

		ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
		if (g.LogEnabled) LogRenderedText(&label_pos, active ? "(x)" : "( )");
		if (label_size.x > 0.0f) RenderText(label_pos, label);

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
		return pressed;
	}

	template <class T=const char*>
	void StatisticItem(const char* label, T val)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) return;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = CalcTextSize(label, NULL, true);
		const ImVec2 value_size = CalcTextSize("" + val, NULL, true);

		const float square_sz = GetFrameHeight();
		const ImVec2 pos = window->DC.CursorPos;
		const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
		const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x + value_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
		ItemSize(total_bb, style.FramePadding.y);
		if (!ItemAdd(total_bb, id)) return false;

		
	}
}