#include "misc.h"

void miscfeatures::watermark() {
	if (!options::misc::watermark) return;

	render::draw_text_ex("ElysiumHaxx 2021", { ImGui::GetWindowSize().x / 2 - 25, ImGui::GetWindowHeight() - 25}, options::misc::watermark_color, ImGui::GetFont(), 18.f);
}