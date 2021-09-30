#pragma once

#include "../rendering/render.h"
#include "../csgo/misc/options.h"

namespace miscfeatures {
	class misc {
	public:
	};

	inline vec3_t vaCmd = vec3_t(0.f, 0.f, 0.f);
	inline vec3_t vaTgt = vec3_t(0.f, 0.f, 0.f);

	void watermark();
	void statuses();
}