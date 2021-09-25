#include "hooks.h"
#include <stdexcept>

namespace hooks {
	void init() {
		steam::hook<create_move>(get_vfunc(csgo::client_mode, 24));
		utils::color_print("[^g+^!] ^mcreate_move^!<^yclient_mode^![^b24^!] -~-> ^gHOOKED!^!\n");

		steam::hook<end_scene>(get_vfunc(csgo::d3_device, 42));
		utils::color_print("[^g+^!] ^mend_scene^!<^yd3_device^![^b42^!] -~-> ^gHOOKED!^!\n");

		steam::hook<override_view>(get_vfunc(csgo::client_mode, 18));
		utils::color_print("[^g+^!] ^moverride_view^!<^yclient_mode^![^b18^!] -~-> ^gHOOKED!^!\n");

		steam::hook<frame_stage_notify>(get_vfunc(csgo::client, 37));
		utils::color_print("[^g+^!] ^mframe_stage_notify^!<^yclient^![^b37^!] -~-> ^gHOOKED!^!\n");

		steam::hook<lock_cursor>(get_vfunc(csgo::surface, 67));
		utils::color_print("[^g+^!] ^mlock_cursor^!<^ysurface^![^b67^!] -~-> ^gHOOKED!^!\n");

		steam::hook<draw_model_execute>(get_vfunc(csgo::model_render, 21));
		utils::color_print("[^g+^!] ^mdraw_model_execute^!<^ymodel_render^![^b21^!] -~-> ^gHOOKED!^!\n");

		utils::color_print("[^m?^!] Scanning for ^msend_datagram^! signature...\n");
		steam::hook<send_datagram>(utils::pattern_scan("engine.dll", "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9 89 7C 24 18")); //thanks tyler
		utils::color_print("[^g+^!] ^msend_datagram^! -~-> ^gHOOKED^!\n");
	}
}