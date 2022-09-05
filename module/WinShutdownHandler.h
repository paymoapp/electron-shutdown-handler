#include <windows.h>
#include <napi.h>
#include <string>

#ifndef _PAYMO_WINSHUTDOWNHANDLER_H
#define _PAYMO_WINSHUTDOWNHANDLER_H

namespace PaymoWinShutdownHandler {
	Napi::Object Init(Napi::Env env, Napi::Object exports);

	// Exported functions
	void setMainWindowHandle(const Napi::CallbackInfo& info);
	Napi::Boolean insertWndProcHook(const Napi::CallbackInfo& info);
	Napi::Boolean removeWndProcHook(const Napi::CallbackInfo& info);
	Napi::Boolean acquireShutdownBlock(const Napi::CallbackInfo& info);
	Napi::Boolean releaseShutdownBlock(const Napi::CallbackInfo& info);

	// Internal functions
	LRESULT CALLBACK WindowProcCb(HWND hWindow, UINT event, WPARAM wParam, LPARAM lParam);
}

#endif
