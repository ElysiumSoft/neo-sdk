#include <windows.h>
#include <cstdint>
#include "csgo/sdk.h"
#include "hooks/hooks.h"
#include "rendering/menu.h"
#include "csgo/misc/input.h"
#include "buildtype.h"
#include "csgo/misc/options.h"

HMODULE library;

std::string header = "^g\n \
 88888888b dP                   oo                     dP     dP                             \n^m \
 88        88                                          88     88                             \n^y \
a88aaaa    88 dP    dP .d8888b. dP dP    dP 88d8b.d8b. 88aaaaa88a .d8888b. dP.  .dP dP.  .dP \n^b \
 88        88 88    88 Y8ooooo. 88 88    88 88'`88'`88 88     88  88'  `88  `8bd8'   `8bd8'  \n^c \
 88        88 88.  .88       88 88 88.  .88 88  88  88 88     88  88.  .88  .d88b.   .d88b.  \n^r \
 88888888P dP `8888P88 `88888P' dP `88888P' dP  dP  dP dP     dP  `88888P8 dP'  `dP dP'  `dP \n^g \
                   .88                                                                       \n^m \
               d8888P                                                                        \n^y \
^!\n\n";

struct windata
{
	unsigned long procid;
	HWND winhandle;
};

void kill_haxx()
{
	utils::color_print("[^r!!^!] Module detacher is currently being fixed!\n");
	//csgo::client_mode->~i_client_mode();
	//FreeLibraryAndExitThread(library, EXIT_SUCCESS);
}

void hotkeys(LPVOID base) {
	input_system::register_hotkey(VK_F4, []()
		{
			menu::toggle();
		});

	/*input_system::register_hotkey(VK_F9, []()
		{
			kill_haxx();
		});*/
}

BOOL CALLBACK EnumWinProc(HWND hwnd, LPARAM lparam)
{
	auto& data = *reinterpret_cast<windata*>(lparam);

	unsigned long procid = 0;
	GetWindowThreadProcessId(hwnd, &procid);

	if (data.procid != procid)
		return TRUE;

	data.winhandle = hwnd;
	return FALSE;
}

HWND FindMainWindow()
{
	windata data{};

	data.procid = GetCurrentProcessId();
	data.winhandle = nullptr;
	EnumWindows(EnumWinProc, reinterpret_cast<LPARAM>(&data));

	return data.winhandle;
}

unsigned long __stdcall on_attach(void* base) {
#if debug
	utils::attach_console();
#endif
	csgo::init();
	csgo::dump();
	netvars.init();
	input_system::initialize();
	menu::init();
	hooks::init();
	hotkeys(base);

	HWND hwnd = FindMainWindow();
	RECT rc;
	if (GetWindowRect(hwnd, &rc))
	{
		options::clientinfo::screen_width = rc.right - rc.left;
		options::clientinfo::screen_height = rc.bottom - rc.top;
		utils::color_print("[^g+^!] Found window size: ^m%i^!px x ^m%i^!px\n");
	}
	else {
		utils::color_print("[^r!!^!] Could not get window size, assuming 1920x1080!!\n");
	}

	utils::color_print(header.c_str());
	utils::color_print("[^g+^!] Successfully initialized, welcome to ^mElysium^yHAXX^!\n");
	utils::color_print("ElysiumHaxx ver: ^g%s^!\t^y%s^!\n\n", __DATE__, __TIME__);

	utils::color_print("[^b?^!] ^mTIP^!: Press F4 to open the menu!\n");
	return true;
}

bool __stdcall DllMain(void* base, uint32_t reason_to_call, void* reserved) {
	if (reason_to_call == DLL_PROCESS_ATTACH)
	{
		library = (HMODULE)base;
		HANDLE tHnd = CreateThread(nullptr, 0, static_cast<LPTHREAD_START_ROUTINE>(on_attach), reserved, 0, nullptr);
	}
	return TRUE;
}