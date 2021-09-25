#pragma once
#include "../csgo/globals.h"

namespace aimbot
{

	void Runn(c_cmd* cmd);
	void AimAtPlayer(entity_t* target, c_cmd* cmd);
	vec3_t fix_movement(c_cmd* cmd, c_cmd originalCMD);
	vec3_t GetSmoothAngle(const vec3_t _from, const vec3_t _to, float* _smooth, const float _decr);
	int GetBestTarget(c_cmd* cmd);

}