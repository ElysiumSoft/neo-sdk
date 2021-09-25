#include "aimbot.h"
#include "../csgo/misc/options.h"
#include "../csgo/globals.h"
#include "../csgo/sdk.h"

#include <cmath>
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
		if (!(GetKeyState(VK_RBUTTON) & 0x800))
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

			if (i >= csgo::globals->max_clients)
				continue;

			if (!clientEntity || clientEntity == g::local)
				continue;

			entity_t* baseEntity = clientEntity->GetBaseEntity();
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
		vec3_t curAngle{};
		csgo::engine->GetViewAngles(&curAngle);
		

		for (AimbotData dat : data)
		{
			int w, h;
			csgo::engine->screen_size(w, h);
			vec3_t screencenter{};
			screencenter.x = w * 0.5f;
			screencenter.y = h * 0.5f;
			vec3_t eye = g::local->eye_pos();
			vec3_t point = dat.aimpoint;
			vec3_t spoint{};
			math::w2s(point, spoint);
			vec3_t tempAngle = math::DirectionToAngle(aimpoint - g::local->eye_pos());
			
			float dst = math::Distance(screencenter, spoint);

			if (dst < dist)
			{
				dist = dst;
				aimpoint = point;
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
			csgo::engine->SetViewAngles(&angle);
		}
	}

}