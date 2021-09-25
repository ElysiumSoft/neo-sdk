#include "aimbotnew.h"
#include "../csgo/misc/options.h"

struct aimdata
{
	float fov;
	int ent_index;
	entity_t* ent;
	player_t* pent;
	bool visible;
};

bool operator<(aimdata const& a, aimdata const& b)
{
	return a.fov < b.fov;
}

vec3_t LerpAngle(vec3_t& from, vec3_t& to, float step)
{
	vec3_t delta = (to - from);
	
	delta = delta.clamp();
	vec3_t newv = from + (delta * step);
	newv = newv.clamp();
	return newv;
}

vec3_t CerpAngle(vec3_t& from, vec3_t& to, float stepx, float stepy)
{
	float cubicStepX = (1 - cos(stepx * math::pi)) / 2;
	float cubicStepY = (1 - cos(stepy * math::pi)) / 2;
	vec3_t delta = (to - from);
	delta = delta.clamp();
	vec3_t newv(0, 0, 0);
	newv.x = (from.x + cubicStepX * delta.x);
	newv.y = (from.y + cubicStepY * delta.y);
	newv = newv.clamp();

	return newv;
}

namespace aimbot
{
	int aimbox = HITBOX_HEAD;

	int bestEnt = -1;
	int prevBest = -1;

	static std::vector<aimdata> targets;

	bool justSwitched;

	void Runn(c_cmd* cmd)
	{
		if (!options::aim::aim_enable)
			return;

		if (!(GetAsyncKeyState(VK_RBUTTON) & 0x800))
			return;

		if (bestEnt == -1)
		{
			bestEnt = GetBestTarget(cmd);
			if (bestEnt != prevBest)
			{
				prevBest = bestEnt;
				justSwitched = true;
				return;
			}
		}
		
		auto tar = csgo::entity_list->get_client_entity(bestEnt)->GetBaseEntity();
		if (bestEnt != -1)
		{
			AimAtPlayer(tar, cmd);
		}

		prevBest = bestEnt;
		justSwitched = false;
		bestEnt = -1;
	}

	void AimAtPlayer(entity_t* tar, c_cmd* cmd)
	{
		player_t* tPlayer = (player_t*)tar;
		vec3_t enpos = tPlayer->get_hitbox_pos(HITBOX_HEAD), eyepos = g::local->eye_pos();
		
		vec3_t aim = math::DirectionToAngle(enpos - eyepos);
		aim = aim.clamp();
		csgo::engine->SetViewAngles(&aim);
	}

	int GetBestTarget(c_cmd* cmd)
	{
		targets.clear();

		for (int i = 0; i < csgo::engine->GetMaxClients(); ++i)
		{
			entity_t* ent = (entity_t*)csgo::entity_list->get_client_entity(i);
			player_t* pent = (player_t*)ent;
			
			if (ent == g::local)
				continue;

			if (pent->team() == g::local->team())
				continue;

			vec3_t enpos = pent->get_hitbox_pos(HITBOX_HEAD), eyepos = g::local->eye_pos();

			vec3_t aim = math::DirectionToAngle(enpos - eyepos);
			float FOV = math::GetFOV(cmd->view_angles + (g::local->aim_punch() * 2.f), aim);
			
			aimdata temp = {
				FOV, i, ent, pent, true
			};

			targets.emplace_back(temp);
		}

		if (targets.empty())
		{
			utils::color_print("[^r!!!!^!] ^rTARGETS LIST IS EMPTY!");
			return -1;
		}
		std::sort(targets.begin(), targets.end());

		utils::color_print("[^g??^!] Best Target:\n\tIndex: ^y%i^!\n\tfov: ^y%f^!\n", targets[0].ent_index, targets[0].fov);
		return targets[0].ent_index;
	}
}