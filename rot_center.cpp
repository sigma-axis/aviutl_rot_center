#include <cstdint>
#include <numeric>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using byte = uint8_t;
#include <exedit.hpp>

////////////////////////////////
// 仕様書．
////////////////////////////////
#define PLUGIN_VERSION	"v1.01"
#define PLUGIN_AUTHOR	"sigma-axis"
#define FILTER_INFO_FMT(name, ver, author)	(name##" "##ver##" by "##author)
#define FILTER_INFO(name)	constexpr char filter_name[] = name, info[] = FILTER_INFO_FMT(name, PLUGIN_VERSION, PLUGIN_AUTHOR)
namespace rot_center
{
	FILTER_INFO("回転中心");

	// trackbars.
	constexpr const char* track_names[] = { "X", "Y", "Z" };
	constexpr int32_t
		track_denom[]		= {      10,      10,      10 },
		track_min[]			= { -999999, -999999, -999999 },
		track_min_drag[]	= {  -20000,  -20000,  -20000 },
		track_default[]		= {       0,       0,       0 },
		track_max_drag[]	= {  +20000,  +20000,  +20000 },
		track_max[]			= { +999999, +999999, +999999 };
	constexpr int track_link[] = { 1, 1, -2, };

	static_assert(
		std::size(track_names) == std::size(track_denom) &&
		std::size(track_names) == std::size(track_min) &&
		std::size(track_names) == std::size(track_min_drag) &&
		std::size(track_names) == std::size(track_default) &&
		std::size(track_names) == std::size(track_max_drag) &&
		std::size(track_names) == std::size(track_max) &&
		std::size(track_names) == std::size(track_link));

	static consteval size_t index_of_track(auto name) {
		auto ret = std::find_if(std::begin(track_names), std::end(track_names),
			[name](auto n) { return std::string_view{ n } == name; }) - track_names;
		if (ret < std::size(track_names)) return ret;
		std::unreachable();
	}

	// callbacks.
	BOOL func_proc(ExEdit::Filter* efp, ExEdit::FilterProcInfo* efpip);

	inline constinit ExEdit::Filter filter = {
		.flag = ExEdit::Filter::Flag::Effect |
			static_cast<ExEdit::Filter::Flag>(1 << 6), // アルファチャンネルなし対応．
		.name = const_cast<char*>(filter_name),
		.track_n = std::size(track_names),
		.track_name = const_cast<char**>(track_names),
		.track_default = const_cast<int*>(track_default),
		.track_s = const_cast<int*>(track_min),
		.track_e = const_cast<int*>(track_max),
		.func_proc = &func_proc,
		.information = const_cast<char*>(info),
		.track_scale = const_cast<int*>(track_denom),
		.track_link = const_cast<int*>(track_link),
		.track_drag_min = const_cast<int*>(track_min_drag),
		.track_drag_max = const_cast<int*>(track_max_drag),
	};
}

namespace align
{
	FILTER_INFO("上下左右揃え");

	// trackbars.
	constexpr const char* track_names[] = { "左右%", "上下%" };
	constexpr int32_t
		track_denom[]		= {    10,    10 },
		track_min[]			= { -1000, -1000 },
		track_min_drag[]	= { -1000, -1000 },
		track_default[]		= {     0,     0 },
		track_max_drag[]	= { +1000, +1000 },
		track_max[]			= { +1000, +1000 };

	static_assert(
		std::size(track_names) == std::size(track_denom) &&
		std::size(track_names) == std::size(track_min) &&
		std::size(track_names) == std::size(track_min_drag) &&
		std::size(track_names) == std::size(track_default) &&
		std::size(track_names) == std::size(track_max_drag) &&
		std::size(track_names) == std::size(track_max));

	// checks.
	constexpr const char* check_names[] = { "左右有効", "上下有効" };
	constexpr int32_t check_default[] = { 1, 1 }; // 0: unchecked, 1: checked, -1: button, -2: combo box.
	static_assert(std::size(check_names) == std::size(check_default));

	// callbacks.
	BOOL func_proc(ExEdit::Filter* efp, ExEdit::FilterProcInfo* efpip);
	BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, AviUtl::EditHandle* editp, ExEdit::Filter* efp);
	int32_t func_window_init(HINSTANCE hinstance, HWND hwnd, int y, int base_id, int sw_param, ExEdit::Filter* efp);

	inline constinit ExEdit::Filter filter = {
		.flag = ExEdit::Filter::Flag::Effect |
			static_cast<ExEdit::Filter::Flag>(1 << 6), // アルファチャンネルなし対応．
		.name = const_cast<char*>(filter_name),
		.track_n = std::size(track_names),
		.track_name = const_cast<char**>(track_names),
		.track_default = const_cast<int*>(track_default),
		.track_s = const_cast<int*>(track_min),
		.track_e = const_cast<int*>(track_max),
		.check_n = std::size(check_names),
		.check_name = const_cast<char**>(check_names),
		.check_default = const_cast<int*>(check_default),
		.func_proc = &func_proc,
		.func_WndProc = &func_WndProc,
		.information = const_cast<char*>(info),
		.func_window_init = &func_window_init,
		.track_scale = const_cast<int*>(track_denom),
		.track_drag_min = const_cast<int*>(track_min_drag),
		.track_drag_max = const_cast<int*>(track_max_drag),
	};
}
#undef PLUGIN_VERSION
#undef PLUGIN_AUTHOR
#undef FILTER_INFO_FMT
#undef FILTER_INFO


////////////////////////////////
// フィルタ処理．
////////////////////////////////
BOOL rot_center::func_proc(ExEdit::Filter* efp, ExEdit::FilterProcInfo* efpip)
{
	efpip->obj_data.cx += (static_cast<int64_t>(efp->track[0]) << 12) / track_denom[0];
	efpip->obj_data.cy += (static_cast<int64_t>(efp->track[1]) << 12) / track_denom[1];
	efpip->obj_data.cz += (static_cast<int64_t>(efp->track[2]) << 12) / track_denom[2];
	return TRUE;
}

BOOL align::func_proc(ExEdit::Filter* efp, ExEdit::FilterProcInfo* efpip)
{
	if (efp->check[0] != 0)
		efpip->obj_data.cx = (static_cast<int64_t>(efpip->obj_w * efp->track[0]) << 11) / track_max[0];
	if (efp->check[1] != 0)
		efpip->obj_data.cy = (static_cast<int64_t>(efpip->obj_h * efp->track[1]) << 11) / track_max[1];
	return TRUE;
}


////////////////////////////////
// ウィンドウ状態の保守．
////////////////////////////////
/*
efp->exfunc->get_hwnd(efp->processing, i, j):
	i = 0:
		j 番目のスライダーの中央ボタン．

	i = 1:
		j 番目のスライダーの左トラックバー．

	i = 2:
		j 番目のスライダーの右トラックバー．

	i = 3:
		j 番目のチェック枠のチェックボックス．

	i = 4:
		j 番目のチェック枠のボタン．(def = -1 で設定した場合)

	i = 5, 7:
		j 番目のチェック枠の右にある static (テキスト).

	i = 6:
		j 番目のチェック枠のコンボボックス．(def = -2 で設定した場合)

	i > 6 -> nullptr.
*/
inline static void update_extendedfilter_wnd(ExEdit::Filter* efp)
{
	constexpr auto invalid_button = "----";

	// "左右%" ボタン．
	::SetWindowTextA(efp->exfunc->get_hwnd(efp->processing, 0, 0),
		efp->check[0] != 0 ? align::track_names[0] : invalid_button);

	// "上下%" ボタン．
	::SetWindowTextA(efp->exfunc->get_hwnd(efp->processing, 0, 1),
		efp->check[1] != 0 ? align::track_names[1] : invalid_button);
}

BOOL align::func_WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, AviUtl::EditHandle* editp, ExEdit::Filter* efp)
{
	switch (message) {
	case ExEdit::ExtendedFilter::Message::WM_EXTENDEDFILTER_COMMAND:

		switch (auto cmd = wparam & 0xffff) {
		case ExEdit::ExtendedFilter::CommandId::EXTENDEDFILTER_UPDATE_CHECK:

			// チェックボックスが更新を受けた．
			auto id = wparam >> 16;
			switch (id = wparam >> 16) {
			case 0: case 1: 
				update_extendedfilter_wnd(efp);
				return TRUE;
			}

			// まずここには来ない．
			break;
		}

		break;
	}

	return FALSE;
}

int32_t align::func_window_init(HINSTANCE hinstance, HWND hwnd, int y, int base_id, int sw_param, ExEdit::Filter* efp)
{
	// フィルタが折りたたまれていて非表示の場合は更新しない．
	// 折りたたみ状態の更新でもこの関数は呼ばれる．
	if (sw_param != 0)
		update_extendedfilter_wnd(efp);
	return 0;
}


////////////////////////////////
// 初期化．
////////////////////////////////
BOOL WINAPI DllMain(HINSTANCE hinst, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		::DisableThreadLibraryCalls(hinst);
		break;
	}
	return TRUE;
}


////////////////////////////////
// エントリポイント．
////////////////////////////////
EXTERN_C __declspec(dllexport) ExEdit::Filter* const* __stdcall GetFilterTableList() {
	constexpr static ExEdit::Filter* filter_list[] = {
		&rot_center::filter,
		&align::filter,
		nullptr,
	};

	return filter_list;
}
