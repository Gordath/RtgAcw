#ifndef GLACIER_ENGINE_WIN32UTILS_H_
#define GLACIER_ENGINE_WIN32UTILS_H_

#include <Windows.h>

template<typename T>
void set_window_long_ptr(HWND handle, int index, T* val)
{
	SetWindowLongPtr(handle, index, reinterpret_cast<LONG_PTR>(val));
}

template<typename T>
T* get_window_long_ptr(HWND handle, int index)
{
	return reinterpret_cast<T*>(GetWindowLongPtr(handle, index));
}

#endif //GLACIER_ENGINE_WIN32UTILS_H_
