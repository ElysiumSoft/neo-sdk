#pragma once

namespace options {
	namespace clientinfo {
		inline int screen_width = 1920;
		inline int screen_height = 1080;
	}

	namespace move {
		inline bool bhop = false;
	}

	namespace legit {
		inline bool backtrack = false;
		inline bool fake_latency = false;
	}

	namespace rcs {
		inline bool anti_recoil = false;
	}

	namespace esp {
		inline bool box_esp = false;
		inline color box_color = color(55, 152, 237, 255);
		inline float box_weight = 1.5f;
		inline bool lines = false;
		inline color line_color = color(55, 152, 237, 255);
		inline float lines_weight = 1.5f;
		inline bool bones_esp = false;
		inline color bones_color = color(0, 255, 36, 255);
		inline float bones_weight = 1.5f;
		inline bool show_names = true;
		inline color name_color = color(0, 255, 255, 255);
		inline bool health_esp = false;
		inline color health_color = color(68, 230, 50, 255);
		inline bool weapon_esp = false;
		inline color weapon_color = color(55, 152, 237, 255);
		inline bool ammo_esp = false;
		inline color ammo_color = color(55, 152, 237, 255);
		inline bool scoped_esp = false;
		inline color scoped_color = color(55, 152, 237, 255);
	}

	namespace aim {
		inline bool aim_enable = false;
	}

	namespace chams {
		inline bool on = false;
		inline bool xqz = false;
		inline color visible = color(55, 152, 237, 255);
		inline color invisible = color(88, 52, 235, 255);
	}

	namespace misc {
		inline bool watermark = true;
		inline color watermark_color = color(55, 152, 237, 255);
	}
}