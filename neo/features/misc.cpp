#include "misc.h"

#include <format>

int w, h = 0;

void miscfeatures::watermark() {
	csgo::engine->screen_size(w, h);

	if (!options::misc::watermark) return;

	render::draw_text_ex("ElysiumHaxx 2021", { (w * 0.5f) - 25.f, (h - 25.f)}, options::misc::watermark_color, ImGui::GetFont(), 18.f);
}

void miscfeatures::statuses() {

	vec3_t* vaEngine = new vec3_t(0.f, 0.f, 0.f);
	csgo::engine->GetViewAngles(vaEngine);

	std::stringstream ssEngine;
	ssEngine << vaEngine->x << ", " << vaEngine->y << ", " << vaEngine->z;
	std::string strEngine = ssEngine.str();
	ssEngine.flush();

	vec3_t vaClientState = csgo::client_s->viewangles;
	std::stringstream ssClientState;
	ssClientState << vaClientState.x << ", " << vaClientState.y << ", " << vaClientState.z;
	std::string strClientState = ssClientState.str();
	ssClientState.flush();

	std::stringstream ssCmd;
	ssCmd << vaCmd.x << ", " << vaCmd.y << ", " << vaCmd.z;
	std::string strCmd = ssCmd.str();
	ssCmd.flush();

	std::stringstream ssTgt;
	ssTgt << vaTgt.x << ", " << vaTgt.y << ", " << vaTgt.z;
	csgo::engine->screen_size(w, h);

	render::draw_text("Aim Target: " + options::status::aimtarget, { (w * 0.5f) - 100.f,(h - 175.f) }, color(255.f, 255.f, 0.f, 255.f));
	render::draw_text("Engine ViewAngles: " + strEngine, { (w * 0.5f) - 100.f, (h - 100.f) }, color(0, 0, 255, 255));
	render::draw_text("CliState ViewAngles: " + strClientState, { (w * 0.5f) - 100.f, (h - 125.f) }, color(0, 0, 255, 255));
	render::draw_text("Cmd ViewAngles: " + strCmd, { (w * 0.5f) - 100.f, (h - 150.f) }, color(0, 0, 255, 255));
}