#include "rcs.h"
#include "../csgo/classes/entities.h"

vec3_t oldPunch = { 0,0,0 };
int oldShotsFired = 0;

/// <summary>
/// Called once per <code>CreateMove()</code> call (hooked)
/// </summary>
/// <param name="cmd">Pointer to the c_cmd object passed through the CreateMove function</param>
void rcs::Run(c_cmd* cmd)
{

	if (!options::rcs::anti_recoil)
		return;

	if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
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

	vec3_t currentPunch = localPlayer->aim_punch();

	if (shotsFired > 1)
	{
		vec3_t newPunch = { currentPunch.x - oldPunch.x, currentPunch.y - currentPunch.y, 0.f };

		vec3_t viewAngle = cmd->view_angles;

		viewAngle.x -= newPunch.x * 2.0f;
		viewAngle.y -= newPunch.y * 2.0f;

		math::ClampAngles(viewAngle);
		csgo::engine->SetViewAngles(&viewAngle);
		oldShotsFired = shotsFired;
		oldPunch = currentPunch;
	}
	else
	{
		oldPunch = { 0.f, 0.f, 0.f };
		oldShotsFired = 0;
	}

	//if (shotsFired > 1)
	//{
	//	if (shotsFired != oldShotsFired)
	//	{
	//		vec3_t aimPunchAngle = localPlayer->aim_punch();
	//		vec3_t viewAngle = cmd->view_angles;
	//		vec3_t rcsAngle{};
	//		rcsAngle.y = (viewAngle.y + oldPunch.y) - (aimPunchAngle.y * 2.f);
	//		rcsAngle.x = (viewAngle.x + oldPunch.x) - (aimPunchAngle.x * 2.f);

	//		//math::AngleNormalize(rcsAngle);

	//		oldPunch.y = aimPunchAngle.y * 2.f;
	//		oldPunch.x = aimPunchAngle.x * 2.f;
	//		
	//		csgo::engine->SetViewAngles(&rcsAngle);
	//		oldShotsFired = shotsFired;

	//	}
	//}
	//else
	//{
	//	oldPunch = { 0,0,0 };
	//	oldShotsFired = 0;
	//}

}