#include "esp.h"
#include "../csgo/misc/options.h"
#include "../rendering/render.h"

bool esp::player_::begin(player_t* pl) {
	if (/*pl->is_dormant() ||*/ !pl->is_alive() || !g::local)
		return false;

	if (!(g::local->team() != pl->team()))
		return false;

	return true;
}

void esp::draw_list() {
	auto player = player_();
	for (auto i = 1; i <= csgo::entity_list->get_highest_entity_index(); ++i) {
		auto entity = entity_t::get_entity_by_index(i);
		if (!entity) continue;

		if (i <= csgo::globals->max_clients) {
			if (player.begin((player_t*)entity)) {
				if (options::esp::box_esp) player.box(entity);

				if (options::esp::lines) player.lines(entity);

				if (options::esp::bones_esp) player.bones(entity);

				if (options::esp::health_esp) player.health(entity);

				if (options::esp::show_names) player.name(entity);

				if (options::esp::weapon_esp) player.weapon(entity);

				if (options::esp::ammo_esp) player.ammo(entity);

				if (options::esp::scoped_esp) player.scoped(entity);
			}
		}
	}
}

void esp::player_::box(entity_t* ent) {
	auto box = render::get_bounding_box(ent);
	render::draw_box(box.left, box.top, box.right, box.bottom, options::esp::box_color, options::esp::box_weight);
}

void esp::player_::lines(entity_t* ent)
{
	vec3_t epos = ent->origin();
	vec3_t lpos = g::local->origin();

	vec3_t la, lb;
	math::w2s(lpos, la);
	math::w2s(epos, lb);

	render::draw_line(la.x, la.y, lb.x, lb.y, options::esp::line_color, options::esp::lines_weight);
}

void esp::player_::bones(entity_t* ent) {
	studiohdr_t* pStudioHdr = csgo::model_info->get_studio_model(ent->get_model());

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vec3_t vChild = ent->GetBonePos(j);
			vec3_t vParent = ent->GetBonePos(pBone->parent);

			vec3_t sChild, sParent;

			math::w2s(vChild, sChild);
			math::w2s(vParent, sParent);

			render::draw_line(sParent[0], sParent[1], sChild[0], sChild[1], options::esp::bones_color, options::esp::bones_weight);
			render::draw_circle(sParent[0], sParent[1], 12.f, options::esp::box_color, options::esp::bones_weight);
			render::draw_circle(sChild[0], sChild[1], 8.f, options::esp::box_color, options::esp::bones_weight);
		}
	}
}

void esp::player_::weapon(entity_t* ent) {
	auto box = render::get_bounding_box(ent);

	std::string active_weapon = ent->active_weapon().get()->name();

	render::draw_text(active_weapon, ImVec2(box.right, box.top), options::esp::weapon_color);
	esp::player_::top_spacing_y += 10;
}

void esp::player_::ammo(entity_t* ent) {
	auto box = render::get_bounding_box(ent);

	std::string ammo = std::to_string(ent->active_weapon().get()->clip());

	render::draw_text_ex(ammo, ImVec2(box.left, box.top + esp::player_::top_spacing_y), options::esp::ammo_color, ImGui::GetFont(), 22.f);
	esp::player_::top_spacing_y += 10;
}

void esp::player_::name(entity_t* ent) {
	auto box = render::get_bounding_box(ent);
	player_info_t pinfo;
	csgo::engine->GetPlayerInfo(ent->ent_index(), &pinfo);
	std::string player_name = pinfo.name;

	render::draw_text_ex(player_name, ImVec2(box.left + 5, box.bottom), options::esp::name_color, ImGui::GetFont(), 18.f);
}

void esp::player_::health(entity_t* ent) {
	auto box = render::get_bounding_box(ent);

	std::string player_health = std::to_string(ent->health()) + "HP";

	
	render::draw_text_ex(player_health, ImVec2(box.right + 5, box.bottom), options::esp::health_color, ImGui::GetFont(), 24.f);

	//render::draw_text(player_health, ImVec2(box.right + 5, box.bottom), options::esp::health_color);
	esp::player_::bottom_spacing_y += 10;
}

void esp::player_::scoped(entity_t* ent) {
	auto box = render::get_bounding_box(ent);

	bool scoped = false;

	if (ent->active_weapon().get()->zoom_level() > 0)
		scoped = true;
	else
		scoped = false;

	if (scoped)
		render::draw_text_ex("scoped", ImVec2(box.right + 5, box.bottom + esp::player_::bottom_spacing_y), options::esp::scoped_color, ImGui::GetFont(), 20.f);
}