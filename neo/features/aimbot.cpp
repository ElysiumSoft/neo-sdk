#include "aimbot.h"
#include "../csgo/misc/options.h"
#include "../csgo/globals.h"
#include "../csgo/sdk.h"
#include "misc.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

//std::vector<long> killTimes = { 0 };
//
//static bool shouldAim = false;
//int aimbot::targetAimbot = -1;
//
//static vec3_t* oldAngle = new vec3_t(0.f, 0.f, 0.f);
//static vec3_t angle = vec3_t(0.f, 0.f, 0.f);
//
//static bool IsInFov(const vec3_t* spot, float Fov)
//{
//	vec3_t pVecTarget = g::local->eye_pos();
//
//	if (math::GetFOV(oldAngle, math::CalcAngle(pVecTarget, *spot)) > Fov)
//		return false;
//
//	return true;
//}
//
//static player_t* GetClosestEnemy()
//{
//	float bestFov = FLT_MAX;
//
//	vec3_t pVecTarget = g::local->eye_pos();
//	player_t* enemy = nullptr;
//
//	int maxClient = csgo::engine->GetMaxClients();
//	for (int i = 0; i < maxClient; i++) {
//		player_t* player = (player_t*)csgo::entity_list->get_client_entity(i);
//
//		if (!player || player == g::local || !player->is_alive() || player->team() == g::local->team())
//			continue;
//
//		vec3_t cbVecTarget = player->eye_pos();
//
//		float cbFov = fabs(math::GetFOV(oldAngle, math::CalcAngle(pVecTarget, cbVecTarget)));
//
//		if (cbFov >= bestFov)
//			continue;
//
//		utils::info_print("Aimbot: best FOV = %f", cbFov);
//		bestFov = cbFov;
//		enemy = player;
//		aimbot::targetAimbot = i;
//		options::status::aimtarget = i;
//	}
//
//	return enemy;
//}
//
//static void GetClosestSpot(player_t* enemy, vec3_t& BestSpot)
//{
//	int boneID = BONE_HEAD;
//	vec3_t bone3D = enemy->GetBonePos(boneID);
//	BestSpot = bone3D;
//}
//
//static player_t* GetClosestPlayerAndSpot(c_cmd* cmd, vec3_t& bestSpot)
//{
//
//	player_t* player = nullptr;
//	player = GetClosestEnemy();
//
//	if (!player || !player->is_alive())
//	{
//		utils::error_print("Aimbot: Failed '!player || !player->is_alive()");
//		return nullptr;
//	}
//
//	vec3_t bone3d;
//	GetClosestSpot(player, bestSpot);
//
//	return player;
//}
//
//void aimbot::Run(c_cmd* cmd)
//{
//	miscfeatures::vaCmd = cmd->view_angles;
//
//	if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
//		return;
//
//	if (!g::local->is_alive())
//		return;
//
//	vec3_t bestSpot{};
//	vec3_t localEye = g::local->eye_pos();
//
//	angle = cmd->view_angles;
//	csgo::engine->GetViewAngles(oldAngle);
//
//	player_t* player = GetClosestPlayerAndSpot(cmd, bestSpot);
//
//	if (player)
//	{
//
//		options::status::aimtarget = player->ent_index();
//		options::status::bestspot = bestSpot;
//		aimbot::targetAimbot = player->ent_index();
//
//		angle = math::CalcAngle(localEye, bestSpot);
//
//		math::AngleNormalize(angle);
//		math::ClampAngles(angle);
//
//		csgo::engine->SetViewAngles(&angle);
//	}
//	else
//	{
//		utils::error_print("Aimbot failed to find a target ^y:(^!\n");
//	}
//
//}

namespace aimbot
{

	void AddHitpoints(std::vector<AimbotData>& vec, player_t* basePlayer)
	{
		matrix3x4_t matrix[MAXSTUDIOBONES];

		if (!basePlayer->setup_bones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
			return;

		studiohdr_t* hdr = csgo::model_info->get_studio_model(basePlayer->get_model());
		if (!hdr)
			return;

		mstudiohitboxset_t* hitboxSet = hdr->get_hitbox_set(basePlayer->hitbox_set());
		if (!hitboxSet)
			return;

		for (int hb = HITBOX_HEAD; hb < HITBOX_MAX; hb++)
		{
			mstudiobbox_t* untransformedBox = hitboxSet->get_hitbox(hb);
			if (!untransformedBox)
				continue;

			float mod = untransformedBox->m_flRadius != -1.f ? untransformedBox->m_flRadius : 0.f;
			vec3_t min, max;
			math::VectorTransform(untransformedBox->bbmax + mod, matrix[untransformedBox->bone], max);
			math::VectorTransform(untransformedBox->bbmin - mod, matrix[untransformedBox->bone], min);

			auto center = (min + max) * 0.5f;
			vec3_t curAngles = math::DirectionToAngle(center - g::local->eye_pos());
			vec3_t forward;
			math::AngleVectors(curAngles, &forward);

			

			vec3_t right = forward.cross(vec3_t(0, 0, 1));
			vec3_t left = vec3_t(-right.x, -right.y, right.z);
			vec3_t top = vec3_t(0, 0, 1);
			vec3_t bottom = vec3_t(0, 0, -1);

			vec.push_back(AimbotData(center, basePlayer));
			vec.push_back(AimbotData(right * (untransformedBox->m_flRadius * 0.6f), basePlayer));
			vec.push_back(AimbotData(left * (untransformedBox->m_flRadius * 0.6f), basePlayer));
		}
	}

	void Run(c_cmd* cmd/*, bool& bSendPacket*/)
	{
		if (!(GetKeyState(VK_RBUTTON) & 0x8000))
			return;

		if (!options::aim::aim_enable || !g::local->is_alive())
			return;

		/*base_weapon* weapon = (base_weapon*)csgo::entity_list->get_client_entity_from_handle(g::local->active_weapon());

		if (!weapon || weapon->knife())
			return;*/

		static std::vector<AimbotData> data;

		for (int i = 0; i < csgo::entity_list->get_highest_entity_index(); i++)
		{
			i_client_entity* clientEntity = csgo::entity_list->get_client_entity(i);

			if(i >= csgo::globals->max_clients)
				continue;

			if (!clientEntity || clientEntity == g::local)
				continue;

			entity_t* baseEntity = (entity_t*)clientEntity;
			if (!baseEntity || baseEntity->team() == g::local->team())
				continue;
			//if (!baseEntity || baseEntity->is_dormant() || /*!baseEntity->IsPlayer() ||*/ baseEntity->team() == g::local->team())
			//	continue;

			player_t* basePlayer = (player_t*)baseEntity;
			if (!basePlayer->is_alive())
				continue;

			AddHitpoints(data, basePlayer);
		}

		vec3_t aimpoint = vec3_t(0,0,0);
		float dist = 9999.0f;
		int fovdist = 9999;
		

		for (AimbotData dat : data)
		{
			vec3_t curAngle = cmd->view_angles;
			int w, h;
			csgo::engine->screen_size(w, h);

			vec3_t screenCenter = vec3_t((w * 0.5f), (h * 0.5f), 1.f);

			vec3_t eye = g::local->eye_pos();
			vec3_t point = dat.aimpoint;
			vec3_t tempAngle = math::DirectionToAngle(aimpoint - g::local->eye_pos());
			math::ClampAngles(tempAngle);

			vec3_t aimPointS;
			math::w2s(dat.aimpoint, aimPointS);

			float dst = math::Distance2d(screenCenter, aimPointS);

			if (dst < dist)
			{
				dist = dst;
				aimpoint = point;
				options::status::bestfov = dst;
			}


			/*float dst = sqrt(pow(point.y - eye.y, 2) + pow(point.x - eye.x, 2) + pow(point.z - eye.z, 2));

			if (dst < dist)
			{
				dist = dst;
				aimpoint = point;
			}*/
		}
		data.clear();
		dist = 9999.f;
		fovdist = 9999;

		

		if (aimpoint != vec3_t(0, 0, 0))
		{
			vec3_t angle = math::DirectionToAngle(aimpoint - g::local->eye_pos());
			math::ClampAngles(angle);
			csgo::engine->SetViewAngles(&angle);
		}
	}

}