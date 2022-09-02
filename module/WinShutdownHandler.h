#include <windows.h>
#include <napi.h>

#ifndef _PAYMO_WINSHUTDOWNHANDLER_H
#define _PAYMO_WINSHUTDOWNHANDLER_H

namespace PaymoWinShutdownHandler {
	Napi::Object Init(Napi::Env env, Napi::Object exports);

	// Exported functions
	Napi::Boolean insertWndProcHook(const Napi::CallbackInfo& info);
	Napi::Boolean removeWndProcHook(const Napi::CallbackInfo& info);

	// Internal objects
	HWND mainWindow = NULL;
	Napi::ThreadSafeFunction tsfn;
	WNDPROC prevWndProc = NULL;

	// Internal functions
	LRESULT CALLBACK WindowProcCb(HWND hWindow, UINT event, WPARAM wParam, LPARAM lParam);
}

#endif
