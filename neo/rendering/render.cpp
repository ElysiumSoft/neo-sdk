#include "render.h"
#include "../rendering/imgui/imgui.h"
#include "../csgo/misc/options.h"
#include "../features/lag_compensation.h"

#include <cmath>
#include <list>
#include <map>

RECT render::get_bounding_box(entity_t* ent) {
	RECT rect{};
	auto collideable = ent->get_collideable();
	if (!collideable)
		return rect;
	auto min = collideable->obb_mins();
	auto max = collideable->obb_maxs();
	const matrix3x4_t& trans = ent->coordinate_frame();
	vec3_t points[] = {
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, max.y, max.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(max.x, min.y, max.z)
	};
	vec3_t points_transformed[8];
	for (int i = 0; i < 8; i++) {
		math::vector_transform(points[i], trans, points_transformed[i]);
	}
	vec3_t screen_points[8] = {};
	for (int i = 0; i < 8; i++) {
		if (!math::w2s(points_transformed[i], screen_points[i]))
			return rect;
	}
	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;
	for (int i = 1; i < 8; i++) {
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}
	return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}

//--------------------------------------------------------------------------------------------------------------------------------

/// <summary>
/// Converts an array of vec2_t objects into an array of ImVec2 objects,
/// so that they may be used in ImGui drawing methods
/// </summary>
/// <param name="points">The array of points to convert</param>
/// <param name="num_points">The exact number of points in the array</param>
/// <returns>An array of ImVec2 objects</returns>
ImVec2* render::PointsArrayToImVec2(const vec2_t* points, const int num_points)
{
	//std::vector<ImVec2> ptlist;
	ImVec2* working{};

	for (int i = 0; i < num_points; i++)
	{
		//ptlist.push_back(ImVec2(points[i].x, points[i].y));
		working[i] = ImVec2(points[i].x, points[i].y);
	}

	return working;
}

//--------------------------------------------------------------------------------------------------------------------------------

void render::draw_box(float x1, float y1, float x2, float y2, color clr, float size) {
	g::draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y1), utils::to_im32(clr, clr.a()), size);
	g::draw_list->AddLine(ImVec2(x1, y2), ImVec2(x2, y2), utils::to_im32(clr, clr.a()), size);
	g::draw_list->AddLine(ImVec2(x1, y1), ImVec2(x1, y2), utils::to_im32(clr, clr.a()), size);
	g::draw_list->AddLine(ImVec2(x2, y1), ImVec2(x2, y2 + 1), utils::to_im32(clr, clr.a()), size);
}

void render::draw_circle(float xcenter, float ycenter, float radius, color clr, float size)
{
	ImVec2 centerpos = ImVec2(xcenter, ycenter);
	g::draw_list->AddCircleFilled(centerpos, radius, utils::to_im32(clr, clr.a()), size);
}

//--------------------------------------------------------------------------------------------------------------------------------

void render::draw_text(std::string text, ImVec2 position, color color) {
	g::draw_list->AddText(position, utils::to_im32(color, color.a()), text.c_str(), NULL);
}

void render::draw_text_ex(std::string text, const ImVec2& pos, color col, const ImFont* font, float font_size)
{
	g::draw_list->AddText(font, font_size, pos, utils::to_im32(col, col.a()), text.c_str(), NULL, 0.0f, NULL);
}

//void render::draw_text_ex(const ImFont* font, float font_size, const ImVec2& pos, color col, const char* text)
//{
//
//	g::draw_list->AddText(font, font_size, pos, utils::to_im32(col, col.a()), text, NULL, 0.0f, NULL);
//}

// g::draw_list->AddText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, 
//						const char* text_end = (const char*)0, float wrap_width = (0.0f), const ImVec4* cpu_fine_clip_rect = (const ImVec4*)0);

void render::draw_line(float x1, float y1, float x2, float y2, color clr, float size)
{
	g::draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), utils::to_im32(clr, clr.a()), size);
}

void render::draw_path(const vec2_t* points, const int num_points, color clr, bool closed, float size)
{
	ImVec2* pointsIm = PointsArrayToImVec2(points, num_points);

	g::draw_list->AddPolyline(pointsIm, num_points, closed, utils::to_im32(clr, clr.a()), size);
}