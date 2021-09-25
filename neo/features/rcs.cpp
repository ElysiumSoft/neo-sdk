#include "rcs.h"
#include "../csgo/classes/entities.h"

vec3_t oldPunch = { 0,0,0 };
int oldShotsFired = 0;

void rcs::Run()
{

	if (!options::rcs::anti_recoil)
		return;

	/* =~=[ !!!!NEED TO FIX c_handle<base_weapon> FUNC!!!! ]=~= */
	/*static const int rejectedWeps[18] = {
		WEAPON_KNIFE, WEAPON_C4, WEAPON_DECOY, WEAPON_FLASHBANG, WEAPON_HEALTHSHOT, WEAPON_MOLOTOV,
		WEAPON_SMOKEGRENADE, WEAPON_GLOCK, WEAPON_DEAGLE, WEAPON_P250, WEAPON_TEC9, WEAPON_NOVA,
		WEAPON_XM1014, WEAPON_SAWEDOFF, WEAPON_SSG08, WEAPON_AWP, WEAPON_G3SG1
	};

	player_t* localPlayer = g::local;
	int activeWepID = localPlayer->active_weapon()->weapon_index;
	for (int rejW : rejectedWeps)
	{
		if (rejW == activeWepID)
			return;
	}*/

	player_t* localPlayer = g::local;

	int shotsFired = localPlayer->shots_fired();

	if (shotsFired >= 1)
	{
		if (shotsFired != oldShotsFired)
		{
			vec3_t aimPunchAngle = localPlayer->aim_punch();
			vec3_t viewAngle = csgo::client_s->viewangles;
			vec3_t rcsAngle{};
			rcsAngle.y = viewAngle.y + (oldPunch.y - aimPunchAngle.y * 2.f);
			rcsAngle.x = viewAngle.x + (oldPunch.x - aimPunchAngle.x * 2.f);

			viewAngle.normalize_in_place();

			oldPunch.y = aimPunchAngle.y * 2.f;
			oldPunch.x = aimPunchAngle.x * 2.f;
			
			csgo::engine->SetViewAngles(&rcsAngle);
			oldShotsFired = shotsFired;

		}
	}
	else
	{
		oldPunch = { 0,0,0 };
		oldShotsFired = 0;
	}

}