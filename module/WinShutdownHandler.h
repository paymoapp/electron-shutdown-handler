#include <windows.h>
#include <napi.h>
#include <string>

#ifndef _PAYMO_WINSHUTDOWNHANDLER_H
#define _PAYMO_WINSHUTDOWNHANDLER_H

namespace PaymoWinShutdownHandler {
	void Init(Napi::Env env, Napi::Object exports);

	// Exported functions
	void setMainWindowHandle(const Napi::CallbackInfo& info);
	Napi::Value insertWndProcHook(const Napi::CallbackInfo& info);
	Napi::Value removeWndProcHook(const Napi::CallbackInfo& info);
	Napi::Value acquireShutdownBlock(const Napi::CallbackInfo& info);
	Napi::Value releaseShutdownBlock(const Napi::CallbackInfo& info);

	// Internal functions
	LRESULT CALLBACK WindowProcCb(HWND hWindow, UINT event, WPARAM wParam, LPARAM lParam);
}

#endif
