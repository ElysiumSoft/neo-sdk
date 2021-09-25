#pragma once
#include "../csgo/globals.h"

namespace aimbot
{

	struct AimbotData
	{
		vec3_t aimpoint;
		player_t* player;

		AimbotData(vec3_t point, player_t* _player)
		{
			aimpoint = point;
			player = _player;
		}
	};

	void AddHitpoints(std::vector<AimbotData>&, player_t*);
	void Run(c_cmd*/*, bool&*/);

}