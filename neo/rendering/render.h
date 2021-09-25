#pragma once
#include "../rendering/imgui/imgui.h"
#include "../features/lag_compensation.h"
#include "fonts.h"

namespace render {


	RECT get_bounding_box(entity_t* ent);
	void draw_box(float x1, float y1, float x2, float y2, color clr, float size);
	void draw_text(std::string text, ImVec2 position, color color);

	//--------------------------------------------------------------------------------------------------------------------------------

	ImVec2* PointsArrayToImVec2(const vec2_t* points, const int num_points);

	//--------------------------------------------------------------------------------------------------------------------------------

	void draw_text_ex(std::string text, const ImVec2& pos, color col, const ImFont* font, float font_size);
	void draw_line(float x1, float y1, float x2, float y2, color clr, float size);
	void draw_circle(float xcenter, float ycenter, float radius, color clr, float size);
	void draw_path(const vec2_t* points, const int num_points, color clr, bool closed = false, float size = 1.f);
}