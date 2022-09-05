#include "WinShutdownHandler.h"

namespace PaymoWinShutdownHandler {
	// Internal objects
	HWND mainWindow = NULL;
	WNDPROC prevWndProc = NULL;
	Napi::ThreadSafeFunction tsfn;
	bool shouldBlockShutdown = false;

	Napi::Object Init(Napi::Env env, Napi::Object exports) {
		exports.Set("setMainWindowHandle", Napi::Function::New(env, setMainWindowHandle));
		exports.Set("insertWndProcHook", Napi::Function::New(env, insertWndProcHook));
		exports.Set("removeWndProcHook", Napi::Function::New(env, removeWndProcHook));
		exports.Set("acquireShutdownBlock", Napi::Function::New(env, acquireShutdownBlock));
		exports.Set("releaseShutdownBlock", Napi::Function::New(env, releaseShutdownBlock));

		return exports;
	}

	void setMainWindowHandle(const Napi::CallbackInfo& info) {
		if (info.Length() != 1) {
			Napi::TypeError::New(info.Env(), "Expected 1 argument").ThrowAsJavaScriptException();
			return;
		}

		if (!info[0].IsBuffer()) {
			Napi::TypeError::New(info.Env(), "Expected first argument to be a buffer").ThrowAsJavaScriptException();
			return;
		}

		mainWindow = *info[0].As<Napi::Buffer<HWND>>().Data();
	}

	Napi::Boolean insertWndProcHook(const Napi::CallbackInfo& info) {
		if (info.Length() != 1) {
			Napi::TypeError::New(info.Env(), "Expected 1 argument").ThrowAsJavaScriptException();
			return Napi::Boolean::New(info.Env(), false);
		}

		if (!info[0].IsFunction()) {
			Napi::TypeError::New(info.Env(), "Expected first argument to be a function").ThrowAsJavaScriptException();
			return Napi::Boolean::New(info.Env(), false);
		}

		if (mainWindow == NULL) {
			Napi::Error::New(info.Env(), "Should call setMainWindowHandle first").ThrowAsJavaScriptException();
			return Napi::Boolean::New(info.Env(), false);
		}

		if (prevWndProc != NULL) {
			// hook already created
			return Napi::Boolean::New(info.Env(), false);
		}

		tsfn = Napi::ThreadSafeFunction::New(info.Env(), info[0].As<Napi::Function>(), "Windows shutdown handler callback", 0, 2);

		prevWndProc = (WNDPROC)SetWindowLongPtr(mainWindow, GWLP_WNDPROC, (LONG_PTR)&WindowProcCb);

		return Napi::Boolean::New(info.Env(), true);
	}

	Napi::Boolean removeWndProcHook(const Napi::CallbackInfo& info) {
		if (mainWindow == NULL) {
			Napi::Error::New(info.Env(), "Should call setMainWindowHandle first").ThrowAsJavaScriptException();
			return Napi::Boolean::New(info.Env(), false);
		}

		if (prevWndProc == NULL) {
			// no hook set
			return Napi::Boolean::New(info.Env(), false);
		}

		SetWindowLongPtr(mainWindow, GWLP_WNDPROC, (LONG_PTR)prevWndProc);

		tsfn.Release();
		prevWndProc = NULL;

		return Napi::Boolean::New(info.Env(), true);
	}

	Napi::Boolean acquireShutdownBlock(const Napi::CallbackInfo& info) {
		if (info.Length() != 1) {
			Napi::TypeError::New(info.Env(), "Expected 1 argument").ThrowAsJavaScriptException();
			return Napi::Boolean::New(info.Env(), false);
		}

		if (!info[0].IsString()) {
			Napi::TypeError::New(info.Env(), "Expected first argument to be a string").ThrowAsJavaScriptException();
			return Napi::Boolean::New(info.Env(), false);
		}

		if (mainWindow == NULL) {
			Napi::Error::New(info.Env(), "Should call setMainWindowHandle first").ThrowAsJavaScriptException();
			return Napi::Boolean::New(info.Env(), false);
		}

		std::u16string u16reason = info[0].As<Napi::String>().Utf16Value();
		std::wstring wreason = std::wstring(u16reason.begin(), u16reason.end());

		BOOL result = ShutdownBlockReasonCreate(mainWindow, wreason.c_str());
		shouldBlockShutdown = true;

		return Napi::Boolean::New(info.Env(), result);
	}

	Napi::Boolean releaseShutdownBlock(const Napi::CallbackInfo& info) {
		if (mainWindow == NULL) {
			Napi::Error::New(info.Env(), "Should call setMainWindowHandle first").ThrowAsJavaScriptException();
			return Napi::Boolean::New(info.Env(), false);
		}

		BOOL result = ShutdownBlockReasonDestroy(mainWindow);
		shouldBlockShutdown = false;

		return Napi::Boolean::New(info.Env(), result);
	}

	LRESULT CALLBACK WindowProcCb(HWND hWindow, UINT event, WPARAM wParam, LPARAM lParam) {
		if (event == WM_QUERYENDSESSION) {
			if (shouldBlockShutdown) {
				return FALSE;
			}

			return TRUE;
		}
		else if (event == WM_ENDSESSION) {
			if (wParam == TRUE) {
				tsfn.BlockingCall([](Napi::Env env, Napi::Function jsCallback) {
					jsCallback.Call({});
				});

				return 0;
			}
		}

		return CallWindowProc(prevWndProc, hWindow, event, wParam, lParam);
	}
}
