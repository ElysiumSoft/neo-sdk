#include "../hooks.h"
#include "../../rendering/imgui/imgui.h"
#include "../../rendering/menu.h"
#include "../../features/esp.h"
#include "../../features/misc.h"
#include "../../features/aimbot.h"

long __stdcall hooks::end_scene::hook(IDirect3DDevice9* device) {
	IDirect3DStateBlock9* pixel_state = NULL;
	IDirect3DVertexDeclaration9* vertDec;
	IDirect3DVertexShader9* vertShader;

	device->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state);
	device->GetVertexDeclaration(&vertDec);
	device->GetVertexShader(&vertShader);

	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);

	ImGui_ImplDX9_NewFrame();

	g::draw_list = ImGui::GetForegroundDrawList();

	menu::render();
	ImGui::GetStyle().AntiAliasedLines = true; //seems to look slightly better (maybe just my eyes fooling me)
	ImGui::GetStyle().AntiAliasedFill = true;
	ImGui::GetStyle().WindowRounding = 3.f;
	esp::draw_list();
	miscfeatures::watermark();
	miscfeatures::statuses();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	pixel_state->Apply();
	pixel_state->Release();
	device->SetVertexDeclaration(vertDec);
	device->SetVertexShader(vertShader);
	return original(device);
}