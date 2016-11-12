#ifndef GLACIER_WINDOWING_TYPES_H_
#define GLACIER_WINDOWING_TYPES_H_

namespace Glacier
{
	using Display_func = void(*)();
	using Idle_func = void(*)();
	using Reshape_func = void(*)(int, int);
	using Keyboard_func = void(*)(unsigned char, int, int);
	using Keyboard_up_func = void(*)(unsigned char, int, int);
	using Special_func = void(*)(int, int, int);
	using Special_up_func = void(*)(int, int, int);
	using Mouse_func = void(*)(int, int, int, int);
	using Motion_func = void(*)(int, int);
	using Passive_motion_func = void(*)(int, int);

	struct WindowFunctionCallbacks {
		Display_func		_display_func = nullptr;
		Idle_func			_idle_func = nullptr;
		Reshape_func		_reshape_func = nullptr;
		Keyboard_func		_keyboard_func = nullptr;
		Keyboard_up_func	_keyboard_up_func = nullptr;
		Special_func		_special_func = nullptr;
		Special_up_func		_special_up_func = nullptr;
		Mouse_func			_mouse_func = nullptr;
		Motion_func			_motion_func = nullptr;
		Passive_motion_func _passive_motion_func = nullptr;
	};
}

#endif //GLACIER_WINDOWING_TYPES_H_
