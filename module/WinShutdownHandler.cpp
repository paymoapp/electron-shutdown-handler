#include "WinShutdownHandler.h"

namespace PaymoWinShutdownHandler {
	// Internal objects
	HWND mainWindow = NULL;
	WNDPROC prevWndProc = NULL;
	Napi::ThreadSafeFunction tsfn;
	bool shouldBlockShutdown = false;

	void Init(Napi::Env env, Napi::Object exports) {
		exports.Set("setMainWindowHandle", Napi::Function::New<setMainWindowHandle>(env));
		exports.Set("insertWndProcHook", Napi::Function::New<insertWndProcHook>(env));
		exports.Set("removeWndProcHook", Napi::Function::New<removeWndProcHook>(env));
		exports.Set("acquireShutdownBlock", Napi::Function::New<acquireShutdownBlock>(env));
		exports.Set("releaseShutdownBlock", Napi::Function::New<releaseShutdownBlock>(env));
	}

	void setMainWindowHandle(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();

		if (info.Length() != 1) {
			Napi::TypeError::New(env, "Expected 1 argument").ThrowAsJavaScriptException();
			return;
		}

		if (!info[0].IsBuffer()) {
			Napi::TypeError::New(env, "Expected first argument to be a buffer").ThrowAsJavaScriptException();
			return;
		}

		mainWindow = *info[0].As<Napi::Buffer<HWND>>().Data();
		SetProcessShutdownParameters(0x3FF, 0);
	}

	Napi::Value insertWndProcHook(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();

		if (info.Length() != 1) {
			Napi::TypeError::New(env, "Expected 1 argument").ThrowAsJavaScriptException();
			return env.Null();
		}

		if (!info[0].IsFunction()) {
			Napi::TypeError::New(env, "Expected first argument to be a function").ThrowAsJavaScriptException();
			return env.Null();
		}

		if (mainWindow == NULL) {
			Napi::Error::New(env, "Should call setMainWindowHandle first").ThrowAsJavaScriptException();
			return env.Null();
		}

		if (prevWndProc != NULL) {
			// hook already created
			return Napi::Boolean::New(env, false);
		}

		tsfn = Napi::ThreadSafeFunction::New(env, info[0].As<Napi::Function>(), "Windows shutdown handler callback", 0, 1);

		prevWndProc = (WNDPROC)SetWindowLongPtr(mainWindow, GWLP_WNDPROC, (LONG_PTR)&WindowProcCb);

		return Napi::Boolean::New(env, true);
	}

	Napi::Value removeWndProcHook(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();

		if (mainWindow == NULL) {
			Napi::Error::New(env, "Should call setMainWindowHandle first").ThrowAsJavaScriptException();
			return env.Null();
		}

		if (prevWndProc == NULL) {
			// no hook set
			return Napi::Boolean::New(env, false);
		}

		SetWindowLongPtr(mainWindow, GWLP_WNDPROC, (LONG_PTR)prevWndProc);

		tsfn.Release();
		prevWndProc = NULL;

		return Napi::Boolean::New(env, true);
	}

	Napi::Value acquireShutdownBlock(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();

		if (info.Length() != 1) {
			Napi::TypeError::New(env, "Expected 1 argument").ThrowAsJavaScriptException();
			return env.Null();
		}

		if (!info[0].IsString()) {
			Napi::TypeError::New(env, "Expected first argument to be a string").ThrowAsJavaScriptException();
			return env.Null();
		}

		if (mainWindow == NULL) {
			Napi::Error::New(env, "Should call setMainWindowHandle first").ThrowAsJavaScriptException();
			return env.Null();
		}

		std::u16string u16reason = info[0].As<Napi::String>().Utf16Value();
		std::wstring wreason = std::wstring(u16reason.begin(), u16reason.end());

		BOOL result = ShutdownBlockReasonCreate(mainWindow, wreason.c_str());
		shouldBlockShutdown = true;

		return Napi::Boolean::New(env, result);
	}

	Napi::Value releaseShutdownBlock(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();

		if (mainWindow == NULL) {
			Napi::Error::New(env, "Should call setMainWindowHandle first").ThrowAsJavaScriptException();
			return env.Null();
		}

		BOOL result = ShutdownBlockReasonDestroy(mainWindow);
		shouldBlockShutdown = false;

		return Napi::Boolean::New(env, result);
	}

	LRESULT CALLBACK WindowProcCb(HWND hWindow, UINT event, WPARAM wParam, LPARAM lParam) {
		if (event == WM_QUERYENDSESSION) {
			tsfn.BlockingCall([](Napi::Env env, Napi::Function jsCallback) {
				jsCallback.Call({});
			});

			if (shouldBlockShutdown) {
				return FALSE;
			}

			return TRUE;
		}
		else if (event == WM_ENDSESSION) {
			if (wParam == FALSE) {
				return 0;
			}
		}

		return CallWindowProc(prevWndProc, hWindow, event, wParam, lParam);
	}
}
