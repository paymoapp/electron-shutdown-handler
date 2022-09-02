#include "WinShutdownHandler.h"

namespace PaymoWinShutdownHandler {
	Napi::Object Init(Napi::Env env, Napi::Object exports) {
		exports.Set("insertWndProcHook", Napi::Function::New(env, insertWndProcHook));
		exports.Set("removeWndProcHook", Napi::Function::New(env, removeWndProcHook));

		return exports;
	}

	Napi::Boolean insertWndProcHook(const Napi::CallbackInfo& info) {
		if (info.Length() != 2) {
			Napi::TypeError::New(info.Env(), "Expected 2 arguments").ThrowAsJavaScriptException();
			return Napi::Boolean::New(info.Env(), false);
		}

		if (!info[0].IsFunction()) {
			Napi::TypeError::New(info.Env(), "Expected first argument to be a function").ThrowAsJavaScriptException();
			return Napi::Boolean::New(info.Env(), false);
		}

		if (!info[1].IsBuffer()) {
			Napi::TypeError::New(info.Env(), "Expected second argument to be a buffer").ThrowAsJavaScriptException();
			return Napi::Boolean::New(info.Env(), false);
		}

		if (mainWindow != NULL) {
			// hook is already created
			return Napi::Boolean::New(info.Env(), false);
		}

		mainWindow = *info[1].As<Napi::Buffer<HWND>>().Data();

		tsfn = Napi::ThreadSafeFunction::New(info.Env(), info[0].As<Napi::Function>(), "Windows shutdown handler callback", 0, 2);

		prevWndProc = (WNDPROC)SetWindowLongPtr(mainWindow, GWLP_WNDPROC, (LONG_PTR)&WindowProcCb);
	}

	Napi::Boolean removeWndProcHook(const Napi::CallbackInfo& info) {
		SetWindowLongPtr(mainWindow, GWLP_WNDPROC, (LONG_PTR)prevWndProc);

		prevWndProc = NULL;
		tsfn.Release();
		mainWindow = NULL;
	}

	LRESULT CALLBACK WindowProcCb(HWND hWindow, UINT event, WPARAM wParam, LPARAM lParam) {
		if (event == WM_ENDSESSION) {
			tsfn.NonBlockingCall([](Napi::Env env, Napi::Function jsCallback) {
				jsCallback.Call({});
			});
		}

		return CallWindowProc(prevWndProc, hWindow, event, wParam, lParam);
	}
}
