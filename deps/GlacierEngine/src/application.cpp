//#include "engine.h"
//#include "service_provider.h"
//#include "windowing_service.h"
//#include "win32window.h"
//
//namespace Glacier
//{
//	Engine* Engine::_engine = nullptr;
//
//	bool Engine::_terminate = false;
//
//	Engine::~Engine()
//	{
//		delete _engine;
//	}
//
//	bool Engine::initialize(int* argc, char** argv)
//	{
//		ServiceProvider::register_service(new WindowingService);
//		return true;
//	}
//
//	//TODO: Implement a window factory, define bitmasks for window options
//	bool Engine::create_window(const std::string &title, const Vec2i &size, const Vec2i &position, const WindowFunctionCallbacks &cbs)
//	{
//		WindowingService* win_srv{static_cast<WindowingService*>(ServiceProvider::provide_service(ServiceType::SRV_WINDOWING))};
//
//		Window* win = new Win32Window(title,
//		                              size,
//		                              position,
//		                              win_srv->get_window_count(),
//		                              true,
//		                              false,
//		                              true,
//		                              true);
//
//		win->set_window_callbacks(cbs);
//		win_srv->add(win);
//
//		return true;
//	}
//
//	void Engine::main_loop()
//	{
//		MSG msg;
//
//		WindowingService *win_srv{ static_cast<WindowingService*>(ServiceProvider::provide_service(ServiceType::SRV_WINDOWING)) };
//
//		while(!_terminate) {
//
//			for(int i = 0; i < win_srv->get_window_count(); ++i) {
//				Window *win{ win_srv->get_window(i) };
//				const WindowFunctionCallbacks &cbs{ win->get_callbacks() };
//				
//				if(cbs._reshape_func && win->changed_size()) {
//					const Vec2i &win_size{ win->get_size() };
//					cbs._reshape_func(win_size.x, win_size.y);
//					win->set_changed_size(false);
//				}
//
//				if (cbs._idle_func || win->must_redisplay()) {
//					while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
//						TranslateMessage(&msg);
//						DispatchMessage(&msg);
//						switch (msg.message) {
//						case WM_APP:
//							if (WM_QUIT == msg.lParam) {
//								PostQuitMessage(msg.lParam);
//							}
//							break;
//						case WM_QUIT:
//							_terminate = true;
//						default:
//							break;
//						}
//
//						if (msg.message == WM_QUIT) {
//							return;
//						}
//					}
//
//					if (cbs._idle_func) { // checking again because a handler might have set this to 0
//						cbs._idle_func();
//					}
//				}
//				else {
//					if (!GetMessage(&msg, nullptr, 0, 0)) {
//						return;
//					}
//					TranslateMessage(&msg);
//					DispatchMessage(&msg);
//				}
//			}
//		}
//	}
//
//	Engine& Engine::get_instance()
//	{
//		if (!_engine) {
//			_engine = new Engine;
//		}
//
//		return *_engine;
//	}
//}
